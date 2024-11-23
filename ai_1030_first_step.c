#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


void init_number(int* layer_number,int** node_number  ){
    printf("레이어 갯수 입력:");
    scanf("%d",layer_number);

    *node_number = (int*)malloc(*layer_number*sizeof(int));

    for(int i = 0 ; i< *layer_number; i++){
        printf("%d번째 레이어의 노드수 입력:",i);
        scanf("%d", &node_number[0][i] );
    }
}


void init_node_matrix(int layer_number,int* node_number,float*** node){

    *node = (float**)malloc(layer_number*sizeof(float*));

    for(int i = 0 ; i<layer_number; i++){

        (*node)[i] = (float*)malloc(node_number[i]*sizeof(float));
    }

}


void init_weight_matrix(int layer_number,int* node_number,float**** weight){

    *weight = (float***)malloc(layer_number*sizeof(float**));

    for(int i = 0 ; i<layer_number; i++){

        (*weight)[i] = (float**)malloc(node_number[i]*sizeof(float*));        
    }

    
    for(int i =0; i<node_number[0]; i++){

        (*weight)[0][i] = (float*)malloc(sizeof(float));
    }

    for(int i = 1; i<layer_number; i++){

        for(int j = 0; j<node_number[i]; j++){
            (*weight)[i][j] = (float*)malloc(node_number[i-1]*sizeof(float));
        }
    }

}


void set_weight(int layer_number,int* node_number,float*** weight){

    srand(time(0));

    for(int i = 0 ; i<node_number[0]; i++ ){
        weight[0][i][0] = 1.0;
    }

    // for(int i = 1; i<layer_number; i++){

    //     for(int j = 0; j<node_number[i]; j++){

    //         for(int k = 0; k<node_number[i-1]; k++){
         
    //             weight[i][j][k]= (float)rand()/RAND_MAX;
    //         }
    //     }
    // }


 for(int i = 1; i<layer_number; i++){

        for(int j = 0; j<node_number[i]; j++){

            for(int k = 0; k<node_number[i-1]; k++){
                printf("L%d(N%d) -> L%d(N%d) 가중치 입력:",i-1,k,i,j);
                scanf("%f",&weight[i][j][k]);
            }
        }
    }


}



void input(int node_number0,float** input_value){

    *input_value = (float*)malloc(node_number0*sizeof(float));

    for(int i = 0 ; i<node_number0; i++){
        printf("%d번째 노드 입력값: ",i);
        scanf("%f", &input_value[0][i]);
    }

}


float sigmoid_function(float x){ 

    return 1.0 / ( 1.0 + exp(-1.0 * x) );
}



void forward(int layer_number,int* node_number,float* input_value,float** node,float*** weight){


for(int i = 0 ; i<node_number[0]; i++){
    node[0][i] = input_value[i]*weight[0][i][0];
}


for(int i = 1 ; i<layer_number; i++){

    for(int j= 0; j<node_number[i]; j++){
        
        node[i][j]=0;
        for(int k =0; k< node_number[i-1]; k++){

            node[i][j] += weight[i][j][k]*node[i-1][k];

        }

        node[i][j] = sigmoid_function(node[i][j]);
    }
}


}

void output(int layer_number,int* node_numer,float** node){

    for(int i = 0; i<node_numer[layer_number-1]; i++){

        printf("%f ", node[layer_number-1][i]);

    }

}





int main(){

int layer_number;
int* node_number;

float** node;
float* input_value;
float*** weight;


init_number(&layer_number,&node_number);

init_node_matrix(layer_number,node_number,&node);

init_weight_matrix(layer_number,node_number,&weight);

set_weight(layer_number,node_number,weight);


input(node_number[0],&input_value);

forward(layer_number,node_number,input_value,node,weight);


output(layer_number, node_number,node);





}

