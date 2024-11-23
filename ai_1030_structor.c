#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>



typedef struct Layer{

    int node_number;
    int weight_number;

    float* node;
    float** weight;

}Layer;


typedef struct Network{

    int layer_number;
    Layer * layer;

}Network;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////// init
void init_layer(Network *_network ){

    printf("레이어 갯수 입력 :");
    scanf("%d", &(_network -> layer_number) );

    _network -> layer = (Layer*)calloc( _network -> layer_number, sizeof(Layer) * _network -> layer_number );
    
}

void init_node(Network* _network){

    for(int i = 0 ; i < _network -> layer_number; i++  ){

        printf("%d레이어 노드수 입력 :",i);
        scanf("%d", &(_network -> layer[i].node_number) );

        _network -> layer[i].node = (float*)calloc( _network -> layer[i].node_number,sizeof(float) );
    } 
    
}

void init_weight(Network* _network){

   
    for(int i = 0 ; i< _network->layer_number-1; i++ ){

        _network -> layer[i].weight_number = _network -> layer[i+1].node_number;
        _network -> layer[i].weight = (float**)calloc( _network-> layer[i].node_number , sizeof(float*) );

        for(int j = 0; j<_network->layer[i].node_number; j++ ){

            _network -> layer[i].weight[j] = (float*)calloc( _network-> layer[i].weight_number , sizeof(float) );
            
        }
    } 

}

void init_network( Network* _network ){

    init_layer( _network );
    init_node( _network );
    init_weight( _network );   

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////// set_weight

void assign_weight(Network* _network,int _layer_num, int _from, int _to ,float _value){

    _network -> layer[_layer_num].weight[_from][_to] = _value ;

}

void set_weight(Network* _network){

    for(int i = 0 ; i< _network-> layer_number-1; i++ ){

        for(int j = 0; j< _network->layer[i].node_number; j++ ){

            for(int k = 0 ; k < _network -> layer[i].weight_number; k++ ){

                float value;
                printf("L%d(%d)  ->  L%d(%d) 가중치 입력:" ,i, j, i+1, k );
                scanf("%f", &value );

                assign_weight( _network, i, j, k, value );

            }
            
        }
    } 

    // srand(time(0));

    // for(int i = 0 ; i< _network->layer_number-1; i++ ){

    //      for(int j = 0; j<_network->layer[i].node_number; j++ ){

    //         for(int k = 0 ; k < _network -> layer[i].weight_number; k++ ){

    //          assign_weight( _network, i, j, k, (float)rand() / RAND_MAX ); 
    //         }
                
    //     }
    // } 

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////// input,forward


float sigmoid_function( float x ){ 

    return 1.0 / ( 1.0 + exp( -1.0 * x ) );
}



void forward( Network* _network  ){
 
    for(int i = 0; i< _network->layer_number-1; i++){

        for(int j= 0; j< _network->layer[i+1].node_number; j++){

            for(int k = 0 ; k < _network->layer[i].node_number; k++ ){

                _network->layer[i+1].node[j] +=  _network -> layer[i].node[k] * _network->layer[i].weight[k][j];
            }

            _network->layer[i+1].node[j] = sigmoid_function( _network->layer[i+1].node[j] );
        }
    }
}

void input( Network* _network ){


    for( int i = 0 ; i < _network -> layer[0].node_number; i++ ){

        printf("노드%d 입력값 :" ,i);
        scanf("%f", &(_network ->layer[0].node[i]) );
    }

}



void output( Network* _network ){

    for(int i = 0 ; i< _network->layer[ _network->layer_number -1 ].node_number; i++ ){

        printf("%f\n",_network->layer[_network->layer_number -1].node[i]);
    }
}

void free_memory(){

}


int main(){


Network network1;

init_network( &network1 );
set_weight(&network1);

input( &network1 );
forward( &network1 );
output( &network1 );


}