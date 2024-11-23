#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stb_image.h"

#define input_size 4096
#define output_size 4
#define data_number 3600
#define test_number 120

char *image_path[data_number];
unsigned char *img_temp[data_number];
float *img[data_number];
float target[data_number][output_size]={0};
char *data_class[output_size]={"cat","dog","tiger","hyena"};




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

            float dx = (float)(src_x - x1);
            float dy = (float)(src_y - y1);

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

        for(int j = 0 ; j<output_size; j++){
            image_path[index] = (char *)malloc(50 * sizeof(char));
            sprintf( image_path[index], "./image_data/%s_images/%s_%d.jpg",data_class[j],data_class[j],i);
            img_temp[index]= stbi_load(image_path[index], &width, &height, &channels, 1);
            resize_bilinear(index);
            index++;
        }

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




void set_weight(Network* _network) {
    srand(time(0));
    for (int i = 0; i < _network->layer_number - 1; i++) {
      
  
        for (int j = 0; j < _network->layer[i].node_number; j++) {
            for (int k = 0; k < _network->layer[i].weight_number; k++) {
                float random_value = (float)rand()/(float)RAND_MAX;
           
                assign_weight(_network, i, k, j, random_value);
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


 _network -> layer[_network->layer_number -1].error = create_matrix( _network-> layer[ _network->layer_number - 1 ].node_number , 1 );

for(int i = 0; i<_network-> layer[ _network->layer_number - 1 ].node_number; i++){ // 출력단의 노드만큼 반복하면서 각 노드에 에러값 초기화 해주기


    _network -> layer[_network->layer_number -1].error[i][0] =  (  _network-> layer[_network->layer_number -1].node[i][0]-target[data_num][i] ); 

}


for(int i = _network->layer_number -2; i >= 1; i-- ){

    float** _transpose_matrix = transpose_matrix(_network->layer[i].weight,_network->layer[i].weight_number,_network->layer[i].node_number );
    
     _network -> layer[i].error = multiply_matrix( _transpose_matrix,_network -> layer[i+1].error,_network->layer[i].node_number,_network->layer[i].weight_number,_network-> layer[ i+1 ].node_number,1);
    free_matrix(_transpose_matrix,_network->layer[i].node_number,_network->layer[i].weight_number); //전치행렬은 더이상 필요없으니 해제

}

}


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


void input(Network* _network, int _data_num) {
    for (int i = 0; i < input_size; i++) {
        _network->layer[0].node[i][0] = img[_data_num][i];

    }
}




void train(Network* _network){
    for(int i = 0; i<data_number-test_number; i++){
 
        input( _network,i );
        forward_propagation( _network );
        backward_propagation( _network,i );
    }

}
void test(Network* _network){


    int class_ans[4] = {0};
    float class_error[4] = {0};

    int ans_num = 0;
    for(int i = data_number-test_number; i<data_number; i++){
        input( _network,i);
        forward_propagation( _network );
        
        float res;
        int index = 0;

        for(int j=0 ; j<output_size; j++){
            if(j == 0){
                res = _network->layer[_network->layer_number-1].node[0][0]; 
            }
            if(res < _network->layer[_network->layer_number-1].node[j][0] ){
                index = j;
                res = _network->layer[_network->layer_number-1].node[j][0];
            }
           
        }

        for(int k=0; k<output_size; k++){
            class_error[i%output_size] += (target[k][0] -_network->layer[_network->layer_number-1].node[k][0])*(target[k][0] -_network->layer[_network->layer_number-1].node[k][0])/4.0;
        }
        
        

        if(index == i%output_size){
            ans_num++;
            class_ans[index]++;
        }
    }    
    printf("%f ", ((float)ans_num/(float)test_number)*100.0 );
    // printf("error mean = %f \n",(class_error[0]+class_error[1]+class_error[2]+class_error[3]) / (float)test_number );
    // printf("---\n");
    for(int i = 0 ; i < output_size; i++ ){
        printf("%f ",(float)class_ans[i]/ ((float)test_number/(float)output_size)*100.0 );
    }
    // printf("---\n");
    // for(int i = 0 ; i < output_size; i++ ){
    //     printf("%s error mean = %f  \n",data_class[i],class_error[i]/ ((float)test_number/(float)output_size) );
    // }

}

void epoch(Network* _network,int num){
    
    printf("0 ");
    test( _network );
    printf("\n");
    for(int i = 0 ; i<num; i++){
    
    printf("%d ",i+1);
    train( _network );    
    test( _network );
    printf("\n");

    }


}

int main(){

    data_load();

    Network network1;
    init_network( &network1 );
    set_weight( &network1 );
    network1.learning_rate = 0.0001;


    epoch(&network1,10);



    printf("end\n");

    for(int i = 0 ; i<4; i++){

        printf("%f ",network1.layer[network1.layer_number-1].node[i][0]);
        
    }
    


}  