#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


void init_number(int* layer_number, int** node_number  ){

    printf("레이어 갯수 입력:");
    scanf("%d", layer_number);

    *node_number = (int*)malloc(*layer_number*sizeof(int));

    for(int i = 0 ; i < *layer_number; i++){

        printf("%d번째 레이어의 노드수 입력:" ,i );
        scanf("%d", &node_number[0][i] );
    }
}


void init_node_matrix( int layer_number, int* node_number, float*** node ){

    *node = (float**)malloc(layer_number*sizeof(float*));

    for(int i = 0 ; i < layer_number; i++){

        (*node)[i] = (float*)malloc( node_number[i]*sizeof(float) );
    }

}


void init_weight_matrix( int layer_number,int* node_number,float**** weight ){

    *weight = (float***)malloc( layer_number*sizeof(float**) );

    for(int i = 0 ; i < layer_number; i++){

        (*weight)[i] = (float**)malloc( node_number[i] * sizeof(float*) );        
    }


    for(int i = 0; i<layer_number-1; i++){

        for(int j = 0; j<node_number[i]; j++){
            (*weight)[i][j] = (float*)malloc( node_number[i+1] * sizeof(float) );
        }
    }

}


void set_weight( int layer_number, int* node_number, float*** weight ){

    // srand(time(0));

    // for(int i = 0; i < layer_number-1; i++){

    //     for(int j = 0; j < node_number[i]; j++){

    //         for(int k = 0; k < node_number[i+1]; k++){
         
    //             weight[i][j][k]= (float)rand() / RAND_MAX;
    //         }
    //     }
    // }

 for(int i = 0; i<layer_number-1; i++){

        for(int j = 0; j < node_number[i]; j++){

            for(int k = 0; k < node_number[i+1]; k++){

                printf("L%d(N%d) -> L%d(N%d) 가중치 입력:" ,i ,j ,i+1 ,k );
                scanf("%f", &weight[i][j][k] );
            }
        }
    }


}


void input(int node_number0,float** node){

    for(int i = 0 ; i<node_number0; i++){
        printf("%d번째 노드 입력값: ",i);
        scanf("%f", &node[0][i]);
    }

}


float sigmoid_function( float x ){ 

    return 1.0 / ( 1.0 + exp( -1.0 * x ) );
}



void forward(int layer_number, int* node_number, float** node, float*** weight){



    for(int i = 0 ; i < layer_number-1; i++){

        for(int j= 0; j < node_number[i+1]; j++){
            
            node[i+1][j] = 0.0;

            for(int k = 0; k < node_number[i]; k++){

                node[i+1][j] += weight[i][k][j] * node[i][k];
            }

            node[i+1][j] = sigmoid_function( node[i+1][j] );
        }
    }


}

void output(int layer_number, int* node_numer, float** node){

    for(int i = 0; i < node_numer[layer_number - 1]; i++){

        printf("%f ", node[layer_number - 1][i]);

    }

}



void free_weight_memory( int layer_number, int* node_number, float *** weight ){

    for(int i = 0; i< layer_number-1; i++){
        for(int j = 0; j<node_number[i]; j++){
            
                free( weight[i][j] );
        }
    }

    for(int i = 0 ; i<layer_number; i++){
        free( weight[i] );
    }
    
    free( weight );

}

void free_node_memory(int layer_number, int* node_number, float ** node ){

    for(int i = 0 ; i<layer_number; i++){

        free( node[i] );
    }

    free( node );
    
}



int main(){

int layer_number;
int* node_number;

float** node;
float*** weight;


init_number( &layer_number, &node_number );

init_node_matrix( layer_number, node_number, &node );

init_weight_matrix( layer_number, node_number, &weight );

set_weight( layer_number, node_number, weight );



input( node_number[0], node );

forward( layer_number, node_number, node, weight );

output( layer_number, node_number, node );

free_weight_memory( layer_number, node_number, weight );
free_node_memory( layer_number, node_number, node );
free( node_number );

}

