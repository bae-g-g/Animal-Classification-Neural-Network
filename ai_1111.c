#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stb_image.h"

#define input_size 4096
#define output_size 4
#define data_number 400

char *image_path[data_number];
unsigned char *img_temp[data_number];
float *img[data_number];
float target[data_number][output_size]={0};





typedef struct Layer{

    int node_number;  //row
    int weight_number; //
    float** node;
    float** weight;
    float** error;
}Layer;


typedef struct Network{

    int layer_number;
    float learning_rate;
    Layer * layer;

}Network;

/////// data load
void resize_bilinear(int index) {
    img[index] = (float* )malloc(64 * 64 * sizeof(float));
    float scale_x = (float)256 / 64.0;
    float scale_y = (float)256 / 64.0;

    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            float src_x = x * scale_x;
            float src_y = y * scale_y;

            int x1 = (int)src_x;
            int y1 = (int)src_y;
            int x2 = (x1 + 1 < 256) ? x1 + 1 : x1;
            int y2 = (y1 + 1 < 256) ? y1 + 1 : y1;

            float dx = src_x - x1;
            float dy = src_y - y1;

            float top = (1.0 - dx) * (float)img_temp[index][y1 * 256 + x1] + dx * (float)img_temp[index][y1 * 256 + x2];
            float bottom = (1.0 - dx) * (float)img_temp[index][y2 * 256 + x1] + dx * (float)img_temp[index][y2 * 256 + x2];

            float value = (1.0 - dy) * top + dy * bottom;
            img[index][y * 64 + x] =value/255.0f; 
        }
    }
}


void data_load(){

int width = 256;
int height = 256;
int  channels;

    int index = 0;
    for(int i = 0; i<data_number/output_size; i++){

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/cat_images/cat_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/dog_images/dog_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/tiger_images/tiger_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    image_path[index] = (char *)malloc(50 * sizeof(char));
    sprintf( image_path[index], "./image_data/hyena_images/hyena_%d.jpg", i);
    img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
    resize_bilinear(index);
    index++;

    }


    for(int i = 0 ; i<data_number; i++){
        target[i][i%output_size] = 1.0; 
    }


}


////////////////////////////////////////////////////////////////////// matrix

float** create_matrix( int _row, int _col ){
    
    float** matrix = (float**)calloc(_row,sizeof(float*));
    
    for (int i = 0; i < _row; i++) {
        
        matrix[i] = (float*)calloc(_col,sizeof(float));
    }

    return matrix;
}

void free_matrix(float** _matrix, int _row, int _col){

    for(int i = 0; i < _row; i++){
        
        free(_matrix[i]);
    }
    free(_matrix);
}

float**  multiply_matrix(float** _matrix_A,float** _matrix_B,int _row_A,int _col_A,int _row_B,int _col_B ){

    if(_col_A != _row_B){
        printf("error\n");
        return 0;
    }

    float ** ans  = create_matrix(_row_A, _col_B);

    for( int i = 0; i< _row_A; i++){

        for( int j = 0; j < _col_B; j++){
            
            ans[i][j] = 0;

            for( int k = 0; k < _col_A ; k++ ){

                ans[i][j] += _matrix_A[i][k]*_matrix_B[k][j]; 
            }
        }
    }

    return ans;

}

float**  transpose_matrix(float** _matrix,int _row,int _col ){

    float **ans  = create_matrix(_col, _row);

    for(int i= 0; i<_row; i++){

        for(int j = 0; j<_col; j++){

            ans[j][i]=_matrix[i][j];
        }
    }

    return ans;
}



void print_matrix(float** _matrix, int _row,int _col){

    for(int i = 0; i<_row; i++){

        for(int j = 0 ; j < _col; j++){

            printf("%f ",_matrix[i][j]);
        }
        printf("\n");
    }


}

/////////////////////////////////////////////////////////////////////////////  init

void init_layer(Network *_network ){

    printf("레이어 갯수 입력 :");
    scanf("%d", &(_network -> layer_number) );

    _network -> layer = (Layer*)calloc( _network -> layer_number, sizeof(Layer) );
    
}

void init_node(Network* _network){

    for(int i = 1 ; i < _network -> layer_number-1; i++  ){

        printf("%d레이어 노드수 입력 :",i);
        scanf("%d", &(_network -> layer[i].node_number) );

    } 

    _network ->layer[0].node_number= input_size;
    _network -> layer[0].node = create_matrix( input_size, 1 );

    _network -> layer[_network -> layer_number-1].node_number = output_size;
    _network -> layer[_network -> layer_number-1].node = create_matrix( output_size, 1 );
}


void init_weight(Network* _network){

   
    for(int i = 0 ; i< _network->layer_number-1; i++ ){

        _network -> layer[i].weight_number = _network -> layer[i+1].node_number;

        _network -> layer[i].weight = create_matrix( _network -> layer[i].weight_number,_network -> layer[i].node_number);
    
    }

}

void init_network( Network* _network ){

    init_layer( _network );
    init_node( _network );
    init_weight( _network );

}

////////////////////////////////////////////////////////////////////////////// weight

void assign_weight(Network* _network,int _p_layer, int _row, int _col ,float _value){

    _network -> layer[ _p_layer ].weight[ _row ][ _col ] = _value ;

}

// void set_weight(Network* _network){

    // for(int i = 0 ; i< _network-> layer_number-1; i++ ){

    //     for(int j = 0; j< _network->layer[i].weight_number; j++ ){

    //         for(int k = 0 ; k < _network -> layer[i].node_number; k++ ){

    //             float value;
    //             printf("L%d(%d)  ->  L%d(%d) 가중치 입력:" ,i, k, i+1, j );
    //             scanf("%f", &value );

    //             assign_weight( _network, i, j, k, value );

    //         }
            
    //     }
    // } 



    // srand(time(0));

    // for(int i = 0 ; i< _network->layer_number-1; i++ ){

            
    //     float stddev = sqrt(2.0 /_network->layer[i].node_number);

    //      for(int j = 0; j<_network->layer[i].node_number; j++ ){
            
    //         for(int k = 0 ; k < _network -> layer[i].weight_number; k++ ){
    //          assign_weight( _network, i, k, j, stddev * ((float)rand() / RAND_MAX * 2.0 - 1.0) ); 
    //         }
                
    //     }
    // }


//     for (int i = 0; i < _network->layer_number - 1; i++) {
//     // float stddev = sqrt(2.0 / (float)_network->layer[i].node_number);
//     float stddev = 2.0/ sqrt((float)_network->layer[i].node_number);

//     for (int j = 0; j < _network->layer[i].node_number; j++) {
//         for (int k = 0; k < _network->layer[i].weight_number; k++) {
//             // Generate a positive random value in the range (0, 1)
//             float random_value = (stddev * ((float)(rand()-rand()) / RAND_MAX)); // (0, 1)
//             assign_weight(_network, i, k, j, random_value);
//         }
//     }
// }



//   for(int i = 0 ; i< _network->layer_number-1; i++ ){

//          for(int j = 0; j<_network->layer[i].node_number; j++ ){

//             for(int k = 0 ; k < _network -> layer[i].weight_number; k++ ){

//              assign_weight( _network, i, k, j, 0.5 ); 
//             }
                
//         }
//     } 

// }




// 함수: 정규분포 난수를 생성 (박스-뮬러 변환)
float generate_normal_random(float mean, float stddev) {
    float u1, u2;
    float pi = 3.1415926535;

    // u1 값을 0이 아닌 (0, 1)로 제한
    do {
        u1 = (float)rand() / RAND_MAX;
    } while (u1 <= 0.0);

    u2 = (float)rand() / RAND_MAX;

    // 박스-뮬러 변환
    float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * pi * u2);
    return z0 * stddev + mean;
}


void set_weight(Network* _network) {
    srand(time(0));
    for (int i = 0; i < _network->layer_number - 1; i++) {
        // 유효성 검사 추가
        if (_network->layer[i].node_number <= 0) {
            printf("Error: Invalid node number\n");
            exit(1);
        }

        // 표준편차 계산 (He Initialization)
        float stddev = sqrt(2.0 / (float)_network->layer[i].node_number);

        for (int j = 0; j < _network->layer[i].node_number; j++) {
            for (int k = 0; k < _network->layer[i].weight_number; k++) {
                // 정규분포 난수 생성
                float random_value = generate_normal_random(0.0, stddev);
                assign_weight(_network, i, k, j, random_value/10.0);
            }
        }
    }
}




/////////////////////////////////////////////////////////////////////////////////////// forward_propagation


float sigmoid_function( float x ){ 
    
    return 1.0 / ( 1.0 + exp( -1.0 * x ) );
    
}


void apply_sigmoid( Network* _network, int _p_layer){

    for(int i = 0 ; i< _network->layer[_p_layer].node_number; i++ ){
        _network->layer[_p_layer].node[i][0] = sigmoid_function( _network->layer[_p_layer].node[i][0] );
    }
    
    
}


void forward_propagation( Network* _network  ){
 
  
    for(int i = 0 ; i < _network -> layer_number-1 ; i++){

        // free_matrix(_network->layer[i+1].node,_network->layer[i+1].node_number,1);
        
        _network->layer[i+1].node = multiply_matrix(_network->layer[i].weight,_network->layer[i].node,_network->layer[i].weight_number,_network->layer[i].node_number,_network->layer[i].node_number,1);    

        apply_sigmoid(_network, i+1);
        
    }

}

////////////////////////////////////////////// backwoard_propagation


void error_update( Network* _network,int data_num){

//처음엔 데이터셋의 타겟값과 출력레이어의 노드값들을 통해서 오차계산
//그러면 출력단의 에러가 구해짐

//그럼 방금 에러를 구한 레이어의 전 레이어의 가중치를 전치행렬로 방금구한 레이어와 곱연산의 결과를 전 레이어의 에러에 넣어주면 됨
// 이것을 첫 레이어가 출력레이어가 되면 멈춰줌 <-입력 레이어의 노드는 입력값임으로 에러를 구할 필요가 없음


//최종출력값에러구하기

 _network -> layer[_network->layer_number -1].error = create_matrix( _network-> layer[ _network->layer_number - 1 ].node_number , 1 );

for(int i = 0; i<_network-> layer[ _network->layer_number - 1 ].node_number; i++){ // 출력단의 노드만큼 반복하면서 각 노드에 에러값 초기화 해주기

//  _network -> layer[_network->layer_number -1].error[i][0] = 2*( target[data_num][i] - _network-> layer[_network->layer_number -1].node[i][0]);
    _network -> layer[_network->layer_number -1].error[i][0] =  ( target[data_num][i] - _network-> layer[_network->layer_number -1].node[i][0] ) * (  target[data_num][i] - _network -> layer[_network->layer_number -1].node[i][0] ); 

}


for(int i = _network->layer_number -2; i >= 1; i-- ){ // 출력레이어는 위에서 구했음으로 그 이전레이어 "-2"부터 입력레이어 앞 ">=1"까지 

    float** _transpose_matrix = transpose_matrix(_network->layer[i].weight,_network->layer[i].weight_number,_network->layer[i].node_number );
    
     _network -> layer[i].error = multiply_matrix( _transpose_matrix,_network -> layer[i+1].error,_network->layer[i].node_number,_network->layer[i].weight_number,_network-> layer[ i+1 ].node_number,1);
   //_network->layer[i].weight_number,   i<_network-> layer[ i+1 ].node_number
    free_matrix(_transpose_matrix,_network->layer[i].node_number,_network->layer[i].weight_number); //전치행렬은 더이상 필요없으니 해제

}

}

//
float sigmoid_prime(float x){
    
        return  x * ( 1.0 - x ) ;

}


void learn( Network* _network, int _p_layer, int _p_weight, int _p_node ){

float l_error           =   _network->layer[_p_layer+1].error[_p_weight][0];
float l_sigmoid_prime   =    sigmoid_prime(_network -> layer[_p_layer+1].node[_p_weight][0]);
float l_node            =   _network -> layer[_p_layer].node[_p_node][0];

_network -> layer[_p_layer].weight[ _p_weight ][ _p_node ] -=  _network -> learning_rate * ( l_error * l_sigmoid_prime * l_node);

}




void backward_propagation( Network* _network,int _data_num){

    error_update( _network,_data_num);

    for(int i = _network->layer_number -2; i >= 0; i-- ){ // 출력레이어로 가는 가중치  "-2"부터 입력레이어 앞 ">=1"까지 


        for(int j = 0; j<_network->layer[i].weight_number; j++){

            for(int k =0; k<_network->layer[i].node_number; k++){
                learn(_network,i,j,k);
            }
        }
        free_matrix(_network->layer[i+1].error,_network->layer[i+1].node_number,1);
    }

}




///////////////////////////////////////////// input,output
// void input( Network* _network, int _data_num ){


//     for(int i = 0; i<input_size; i++){
//         _network -> layer[0].node[i][0] = ( ( (float)img[ _data_num ][i] ) / 255.0 );
//     }

// }


void input(Network* _network, int _data_num) {
    for (int i = 0; i < input_size; i++) {

        // Normalize the pixel value and assign it to the input layer's nodes
        _network->layer[0].node[i][0] = img[_data_num][i];

    }
}


void output( Network* _network ){


    // printf("레이어 갯수 %d\n",_network->layer_number);
    
    // for(int i = 0; i<_network->layer_number; i++){
    //     printf("노드 갯수 %d\n",_network->layer[i].node_number);
    // }

    // printf("가중치 정보\n\n");
    // for(int i = 0; i<_network->layer_number-1; i++){

    //      printf("------ %d레이어-------\n",i+1);
    //      print_matrix( _network -> layer[i].weight, _network -> layer[i].weight_number, _network -> layer[i].node_number );
        

    // }

    for(int i= 0; i<_network->layer[_network->layer_number-1].node_number; i++){
       printf("%f  ", _network->layer[_network->layer_number-1].node[i][0]);
    }
}


void free_network( Network* _network ){
//
}




int main(){

    data_load();



    Network network1;
    init_network( &network1 );
    set_weight( &network1 );
    network1.learning_rate = 0.05;


    
    // // printf("%f %f %f", img[0][1],img[1][1],img[2][1]);

        // input( &network1,0);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("-1\n");   
        // backward_propagation(&network1,0);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("---------------\n\n");        


        // input( &network1,1);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("-2\n");   
        // backward_propagation(&network1,1);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("----------------\n\n");     


        // input( &network1,2);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("-3\n");   
        // backward_propagation(&network1,2);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("-------------\n\n");     

        // input( &network1,3);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("-4\n");   
        // backward_propagation(&network1,3);
        // forward_propagation( &network1 );
        // print_matrix(network1.layer[network1.layer_number-1].node,network1.layer[network1.layer_number-1].node_number,1);
        // printf("---------------\n\n");   


for(int j = 0; j<10; j++){

    for(int i = 0; i<data_number; i++){
 
        input( &network1,i );
        forward_propagation( &network1 );
        backward_propagation( &network1,i );
    }

        

    printf("%d\n",j);
    for(int i = 0 ; i<9; i++){
        input(&network1,i);
        forward_propagation(&network1);

        printf(" %f, %f, %f, %f ",network1.layer[network1.layer_number-1].node[0][0],network1.layer[network1.layer_number-1].node[1][0],network1.layer[network1.layer_number-1].node[2][0],network1.layer[network1.layer_number-1].node[3][0]);    

        
        printf("\n");
    }


}
    

    

    printf("종료!\n");

    


}  