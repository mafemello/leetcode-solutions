#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"


#define COMM_CHUNK_SIZE_TAG 0

#define COMM_CHUNK_TAG 1


int  main(int argc, char *argv[]) {

    int n_process, my_rank;

    int chunk_size;
    MPI_Status status;

    int arr_size,k;
    int *arr;
    int *resp;
    resp = NULL;
    int resp_size = 0;
    int maior;

    //incializando
    MPI_Init(&argc,&argv);


    MPI_Comm_size(MPI_COMM_WORLD,&n_process);

    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    if(my_rank == 0){
        //leitura de dados
        scanf("%d",&arr_size);
        scanf("%d",&k);

        arr = (int *) malloc(sizeof(int)*arr_size);

        for(int i=0;i<arr_size;i++){
            scanf("%d",&arr[i]);
        }


        //quantos números serão passados para cada processo
        chunk_size = arr_size/n_process;

        //enviar cada chunk para cada processo
        for(int i=1;i<n_process;i++){
            //printf("Processo 0 enviando %d dados para o processo %d\n",chunk_size,i);

            //enviando numero de dados
            MPI_Send(&chunk_size ,1, MPI_INT, i, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD);

            //enviar chunk
            MPI_Send(arr+(i-1)*chunk_size ,chunk_size, MPI_INT, i, COMM_CHUNK_TAG, MPI_COMM_WORLD);
            
            
            //enviar arr[k]
            MPI_Send(&arr[k] ,1, MPI_INT, i, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD);
        }

        //printf("%d",(n_process-1)*chunk_size);


        //o processo 0 processa o fim do array
        int * local_resp;local_resp = NULL;
        int local_size = 0;
        for(int i =(n_process-1)*chunk_size;i<arr_size;i++){
            if(arr[i]>arr[k]){
                local_size++;
                local_resp = (int *)realloc(local_resp,local_size);
                local_resp[local_size-1] = i;
                //printf("%d %d\n",local_resp[local_size-1],my_rank);
            }
        }


        //receber a resposta

        int aux;
        for(int i=1;i<n_process;i++){
            MPI_Recv(&aux ,1, MPI_INT, i, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD, &status);
            if( aux > 0 ){

                printf("%d\n",aux);
                
                resp = (int*)realloc(resp,resp_size+aux);
                MPI_Recv(resp+resp_size ,aux, MPI_INT, i, COMM_CHUNK_TAG, MPI_COMM_WORLD, &status);
                resp_size+=aux;
            }

        }

        printf("%d\n",resp_size+local_size);
        for(int i=0;i<resp_size;i++){
            printf("%d ",resp[i]);
        }

        for(int i=0;i<local_size;i++){
            printf("%d ",local_resp[i]);
        }
        printf("\n");



        free(local_resp);
        free(arr);
        free(resp);
    }


    else{

        //recebendo tamanho do array associado
        MPI_Recv(&chunk_size ,1, MPI_INT, 0, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD, &status);


        //alocando e recebendo o array
        arr = (int *) malloc(sizeof(int)*chunk_size);
        MPI_Recv(arr ,chunk_size, MPI_INT, 0, COMM_CHUNK_TAG, MPI_COMM_WORLD, &status);

        //rebendo arr[k] 
        MPI_Recv(&maior ,1, MPI_INT, 0, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD, &status);


        //calculando os maiores
        for(int i=0;i<chunk_size;i++){
            if(arr[i]>maior){
                resp_size++;
                resp = (int *)realloc(resp,resp_size);
                //levando em conta em que processo estou para obter o indice absoluto
                resp[resp_size-1] = i+(my_rank-1)*chunk_size;
                //printf("%d %d\n",resp[resp_size-1],my_rank);
            }
        }

        MPI_Send(&resp_size ,1, MPI_INT, 0, COMM_CHUNK_SIZE_TAG, MPI_COMM_WORLD);

        if(resp_size >0)
            MPI_Send(resp,resp_size, MPI_INT, 0, COMM_CHUNK_TAG, MPI_COMM_WORLD);


        free(resp);
        free(arr);
    }





    if(MPI_Finalize() == MPI_SUCCESS){
        printf("All good\n");
    }




    return 0;
}