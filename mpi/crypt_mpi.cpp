#include<iostream>
#include <stdio.h>
#include <mpi.h>
#include<string.h>
#include<crypt.h>
#include<math.h>
#include <sys/time.h>



#define BUFSIZE 1024


double wtime(){
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + t.tv_usec / 1000000.0;
}

using namespace std;
int main(int argc,char *argv[]){
    MPI_Status status;
    int size, rank;
    int a = 100,  tag=0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);

    char key[BUFSIZE] = "ffffffff";          

    double start_time, end_time;

    int tamanho = strlen(key);
    char salt_origin[BUFSIZE] = "aa";     
    char salt[BUFSIZE];

    struct crypt_data data;                 // struct pro crypt_r
    data.initialized = 0;                   

    sprintf(salt, "$1$%s", salt_origin);    

    crypt_r(key, salt, &data);

    char keyHash [BUFSIZE];
    strcpy(keyHash, data.output);

    //char chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char chars[] ={'a','b','c','d','e','f'};
    int password[tamanho]={0};

    long long int i=0;
    int j=0;
    int k=0;
    int l=0;
    int m=0;

    char temp[tamanho];
    char temp2[tamanho];
    int teste;

    int chars_length = sizeof(chars) / sizeof(char);
    int password_length = sizeof(password) / sizeof(int);
    float totalCombinacoes = pow(chars_length, password_length);
    if(rank == 0){
        printf("tamanho: %d\n", tamanho);
        printf("total de combinações: %.0f\n", totalCombinacoes);
        printf("\n\n");
    }

    int tid;
    char **matriz =  (char**)malloc((long long int)totalCombinacoes  * sizeof(char*));
    for(int i = 0; i < (long long int)totalCombinacoes ; i++){
        matriz[i] = (char *)malloc(tamanho * sizeof(char));
    }
    printf("malloc: check \n");
    
    long long int palavrasNaMatriz=0;
    long long int palavrasRestantes = (long long int)totalCombinacoes;
    int stop = 0;
    
    start_time = wtime();
        for (i = 0; i <  (long long int)totalCombinacoes; i++){     //passa por todas as combinações
            for (j=0; j < password_length; j++) {                   //passa por todas as variáveis no array
                if (password[j] == chars_length) {                  //se a senha em password[] é a última variável no array "chars"        
                    password[j + 1]++;                              //aumenta pra nova variável no vetor password[]
                    password[j] = 0;                                //volta a variável pra 0
                }
            }
            for (int j = 0; j < password_length; j++) {
                temp[j] = chars[password[j]];
            }
            temp[tamanho] ='\0';   

            strcpy(matriz[m],temp);
            m=m+1;
            palavrasNaMatriz = palavrasNaMatriz+1;
            palavrasRestantes = palavrasRestantes-1;

            if ((palavrasNaMatriz == totalCombinacoes)){
                        palavrasNaMatriz = 0;
                        m=0;
                        {
                            for(k = rank*(totalCombinacoes/size); k < (rank+1)*(totalCombinacoes/size); k++){ 
                                for(int l = 0; l < tamanho; l++){ 
                                    temp2[l] = matriz[k][l];
                                }
                                temp2[tamanho] = '\0';
                                crypt_r(temp2, salt, &data);
                                if (strcmp(keyHash, data.output) == 0)
                                {      
                                    printf("rank: %d | encontrou a chave: \'%s\' | hash: \'%s\' \n", rank, temp2, data.output);
                                    stop = 1;                                   
                                }
                                
                                if(stop == 1){
                                    k = (long long int)totalCombinacoes; 
                                    i = (long long int)totalCombinacoes;
                                    printf("rank: %d parou\n", rank);
                                    MPI_Abort(MPI_COMM_WORLD, 0);
                                }
                            }
                        }
                    }
                    password[0]++;
        }
    printf("\n\n");

	end_time = wtime();
		cout << end_time - start_time << endl;
}
