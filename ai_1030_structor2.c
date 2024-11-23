#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct Layer{

    int node_number;  //row
    int weight_number; //
    float** node;
    float** weight;
    float** error;
}Layer;


typedef struct Network{

    int layer_number;
    Layer * layer;

}Network;

////////////////////////////////////////////////////////////////////// matrix

float** create_matrix(int _row, int _col) {
    
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

    for(int i = 0 ; i < _network -> layer_number; i++  ){

        printf("%d레이어 노드수 입력 :",i);
        scanf("%d", &(_network -> layer[i].node_number) );

    } 

    _network -> layer[0].node = create_matrix( _network -> layer[0].node_number, 1 );
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

void set_weight(Network* _network){

    for(int i = 0 ; i< _network-> layer_number-1; i++ ){

        for(int j = 0; j< _network->layer[i].weight_number; j++ ){

            for(int k = 0 ; k < _network -> layer[i].node_number; k++ ){

                float value;
                printf("L%d(%d)  ->  L%d(%d) 가중치 입력:" ,i, k, i+1, j );
                scanf("%f", &value );

                assign_weight( _network, i, j, k, value );

            }
            
        }
    } 

    // srand(time(0));

    // for(int i = 0 ; i< _network->layer_number-1; i++ ){

    //      for(int j = 0; j<_network->layer[i].node_number; j++ ){

    //         for(int k = 0 ; k < _network -> layer[i].weight_number; k++ ){

    //          assign_weight( _network, i, k, j, (float)rand() / RAND_MAX ); 
    //         }
                
    //     }
    // } 

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
 
  
    for(int i = 0 ; i < _network -> layer_number -1; i++){

        _network->layer[i+1].node = multiply_matrix(_network->layer[i].weight,_network->layer[i].node,_network->layer[i].weight_number,_network->layer[i].node_number,_network->layer[i].node_number,1);    

        apply_sigmoid(_network, i+1);
    }

}

////////////////////////////////////////////// backwoar_propagation





void error_update( Network* _network ){

    //각 레이별 에러를 업데이트

}

//

void learn( Network* _network, int _p_layer_number, int _p_node_number, int _p_weight_number ){


}




void backward_propagation( Network* _network ){

    error_update( _network );

    for(int i = _network->layer_number-1 ; i >= 0; i--){

            //weigh만큼
            //learn 돌리셈
            //첫 시작은 어케 할꺼임?

    }


}




///////////////////////////////////////////// input,output

void input( Network* _network ){


    for( int i = 0 ; i < _network -> layer[0].node_number; i++ ){

        printf("노드%d 입력값 :" ,i);
        scanf("%f", &( _network -> layer[0].node[i][0] ) );
    }

}



void output( Network* _network ){

    for(int i = 0 ; i< _network->layer[ _network->layer_number -1 ].node_number; i++ ){

        printf("%f\n" ,_network -> layer[_network->layer_number -1].node[i][0] );
    }
}


void free_network( Network* _network ){
//
}

int main(){

    Network network1;

    init_network( &network1 );
    set_weight( &network1 );




    input( &network1 );
    forward_propagation( &network1 );
    backward_propagation( &network1 );



    // for(int i = 0; i<network1.layer_number-1; i++){

    //     printf("------ %d레이어-------\n",i+1);
    //     print_matrix( network1.layer[i].weight, network1.layer[i].weight_number,network1.layer[i].node_number );
        

    // }


}  