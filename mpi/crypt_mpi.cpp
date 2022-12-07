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
    
    // printf("sou %d de %d\n", rank, size);
    // if (rank == 0){
    //     a = 1000;
    // }

    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // printf("%d terminei com %d\n", rank, a);


//===============================================================

char key[BUFSIZE] = "cccccccccccc";          

    double start_time, end_time;

    int tamanho = strlen(key);
    char salt_origin[BUFSIZE] = "aa";     //Salt.
    char salt[BUFSIZE];

    struct crypt_data data;                 //Structure required for hash result storage
    data.initialized = 0;                   // crypt_r()Must be done before using the function.

    sprintf(salt, "$1$%s", salt_origin);    //id specification((See below)

    crypt_r(key, salt, &data);

    //printf("%s\n", data.output);

    char keyHash [BUFSIZE];
    strcpy(keyHash, data.output);
    //printf("%s\n", data.output);                    //The output member variable
                                            //String"$5$example$(Hashed key)"Is stored

    //VARIABLES
    //char chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char chars[] ={'a','b','c'};
    int password[tamanho]={0};

    long long int i=0;
    int j=0;
    int k=0;
    int l=0;
    int m=0;

    char temp[tamanho];
    char temp2[tamanho];
    int teste;

    
    //SIZES OF VERIABLES
    int chars_length = sizeof(chars) / sizeof(char);
    int password_length = sizeof(password) / sizeof(int);
    float totalCombinacoes = pow(chars_length, password_length);
    if(rank == 0){
        printf("tamanho: %d\n", tamanho);
        printf("total de combinações: %.0f\n", totalCombinacoes);
        //printf("numThreads: %d | execucoesPorThread: %d | totalExecucoes: %d \n", numthreads, execucoesPorThread, totalExecucoes);
        printf("\n\n");
    }

    int tid;
    //printf("chegou aqui");
    //char matriz[(int)totalCombinacoes][tamanho];
    char **matriz =  (char**)malloc((long long int)totalCombinacoes  * sizeof(char*));
    for(int i = 0; i < (long long int)totalCombinacoes ; i++){
        matriz[i] = (char *)malloc(tamanho * sizeof(char));
        //printf("%d\n", matriz[i]);
    }
    printf("malloc: check \n");
    
    //char matriz[(long long int)totalCombinacoes][tamanho];
    //printf("numthreads: ")
    long long int palavrasNaMatriz=0;
    long long int palavrasRestantes = (long long int)totalCombinacoes;
    int stop = 0;
    

    start_time = wtime();

    //printf("%lli | %lli \n",i, (long long int)totalCombinacoes);
        for (i = 0; i <  (long long int)totalCombinacoes; i++){ //CYCLE TROUGH ALL OF THE COMBINATIONS
        //if (rank == 0){
            for (j=0; j < password_length; j++) {//CYCLE TROUGH ALL OF THE VERIABLES IN ARRAY
                if (password[j] == chars_length) {//IF VERIABLE IN "PASSWORD" ARRAY IS THE LAST VERIABLE IN CHAR "CHARS" ARRRAY
                    password[j + 1]++;//THEN INCREMENT THE NEXT VERIABLE IN "PASSWORD" ARRAY
                    password[j] = 0;//AND RESET THE VERIABLE BACK TO ZERO
                }
            }
            //PRINT OUT FIRST COMBINATION
            //std::cout << i << ": ";
            for (int j = 0; j < password_length; j++) {
                //std::cout << chars[password[j]] << "";
                temp[j] = chars[password[j]];
            }
            //std::cout << "\n";
            temp[tamanho] ='\0';   
            //printf("%d: %s  \n",rank, temp);

            strcpy(matriz[m],temp);
            m=m+1;
            palavrasNaMatriz = palavrasNaMatriz+1;
            palavrasRestantes = palavrasRestantes-1;
        //}

            if ((palavrasNaMatriz == totalCombinacoes)){
                        //printf("palavrasNaMatriz %d == palavrasNaMatriz %d  \n" , (int)totalCombinacoes, palavrasNaMatriz);
                        palavrasNaMatriz = 0;
                        m=0;
                        //printf("%d entrou\n", rank);
                        //#pragma omp parallel shared(stop, i) private(k, l, rank) firstprivate(execucoesPorThread, totalExecucoes, temp, temp2, salt, data, tamanho, matriz, totalCombinacoes, numthreads, keyHash)  num_threads(numthreads)
                        {
                            //rank = omp_get_thread_num();
                            for(k = rank*(totalCombinacoes/size); k < (rank+1)*(totalCombinacoes/size); k++){ 
                            //for(k = 0; k < (int)totalCombinacoes; k++){ 
                            //printf("rank = %d | k= %d | k < %d\n", rank,rank*execucoesPorThread,(rank+1)*execucoesPorThread);
                                for(int l = 0; l < tamanho; l++){ 
                                    temp2[l] = matriz[k][l];
                                    //printf("k: %d | l: %d | caractere: %c \n", k, l,  matriz[k][l]);
                                }
                                temp2[tamanho] = '\0';
                                //printf("tamanho: %d | palavra: %s \n",tamanho, temp2);
                                crypt_r(temp2, salt, &data);
                                //printf("%d: %s - %s\n",rank, temp2, data.output);
                                if (strcmp(keyHash, data.output) == 0)
                                {      
                                    //int rank = omp_get_thread_num();
                                    //printf("%d = ", rank);
                                    printf("rank: %d | encontrou a chave: \'%s\' | hash: \'%s\' \n", rank, temp2, data.output);
                                    // i = (int)totalCombinacoes;
                                    stop = 1;                                   
                                }
                                
                                if(stop == 1){
                                    k = (long long int)totalCombinacoes; //(rank+1)*(totalCombinacoes/size)
                                    i = (long long int)totalCombinacoes;
                                    //printf("%d\n", i);
                                    printf("rank: %d parou\n", rank);
                                    MPI_Abort(MPI_COMM_WORLD, 0);
                                }
                            }
                        }
                    }
                    password[0]++;
        }
        // printf("matriz\n");
        // if(rank==0)
        // for(int h=0; h <(long long int)totalCombinacoes;h++)
        //     printf("%s \n", matriz[h]);
    printf("\n\n");
    

	end_time = wtime();
		cout << end_time - start_time << endl;
    
 

}

/*
    if (rank == 0){
        
        MPI_Send(&a, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD, &status);



    }   else{
            MPI_Recv(&a, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
            a+=rank;
            int next = rank + 1;
            if (next == size){
                next = 0;
            }
            
            printf("sou %d e recebi %d\n", rank, a);

    }


    MPI_Finalize();
    
    return 0;

}
*/