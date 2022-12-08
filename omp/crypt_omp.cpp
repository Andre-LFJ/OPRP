//https://www.linuxtut.com/en/c9f9e87cf9bcdd31be61/

//http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/intro_openmp-Fernando-Silva.pdf

//https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c

/*
ID = 
1 	MD5
5 	SHA-256
6 	SHA-512
*/

//gcc crypt.c -lcrypt


#define _GNU_SOURCE // crypt_r()To use the function, use this macro definition
                    //Must be listed before any file include
#include<crypt.h>
#include<stdio.h>
#include<stdlib.h>     

#include<iostream>
#include<math.h>
#include<omp.h>
#include<string.h>

#include <sys/time.h>

#include<unistd.h>

#define BUFSIZE 1024



double wtime(){
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + t.tv_usec / 1000000.0;
}



using namespace std;
int main(void){
    char key[BUFSIZE] = "ffffffff";          

    double start_time, end_time;

    int tamanho = strlen(key);
    printf("tamanho: %d\n", tamanho);
    char salt_origin[BUFSIZE] = "aa";    
    char salt[BUFSIZE];

    struct crypt_data data;                 // struct pro crypt_r
    data.initialized = 0;                   

    sprintf(salt, "$1$%s", salt_origin);    

    crypt_r(key, salt, &data);

    printf("%s\n", data.output);

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
    int numthreads = 12;
    int execucoesPorThread = 48;                //quantidade de palavras que serão armazenadas pra execução em cada thread
    int totalExecucoes = numthreads * execucoesPorThread;
    
    int chars_length = sizeof(chars) / sizeof(char);
    int password_length = sizeof(password) / sizeof(int);
    float totalCombinacoes = pow(chars_length, password_length);
    
    printf("total de combinações: %.0f\n", totalCombinacoes);
    printf("numThreads: %d | execucoesPorThread: %d | totalExecucoes: %d \n", numthreads, execucoesPorThread, totalExecucoes);
    printf("\n\n");

    int tid;
    char matriz[totalExecucoes][tamanho];
    int palavrasNaMatriz=0;
    long long int palavrasRestantes = (long long int)totalCombinacoes;
    int stop = 0;
    start_time = wtime();

    printf("%d | %lli \n",i, (long long int)totalCombinacoes);
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
            if ((palavrasNaMatriz == totalExecucoes) || (palavrasRestantes<execucoesPorThread)){ //executa os testes
                palavrasNaMatriz = 0;
                m=0; 
                #pragma omp parallel shared(stop, i) private(k, l, tid) firstprivate(execucoesPorThread, totalExecucoes, temp, temp2, salt, data, tamanho, matriz, totalCombinacoes, numthreads, keyHash)  num_threads(numthreads)
                {
                    tid = omp_get_thread_num();
                    for(k = tid*execucoesPorThread; k < (tid+1)*execucoesPorThread; k++){ 
                        for(int l = 0; l < tamanho; l++){ 
                            temp2[l] = matriz[k][l];   
                        }
                        temp2[tamanho] = '\0';
                        //printf("%s\n", temp2);
                        crypt_r(temp2, salt, &data);
                        if (strcmp(keyHash, data.output) == 0)
                        {      
                            printf("tid: %d | encontrou a chave: \'%s\' | hash: \'%s\' \n", tid, temp2, data.output);
                            stop = 1;                                   
                        }
                        if(stop == 1){                          //se uma thread encontrou a senha, para todas
                            k = numthreads*execucoesPorThread;
                            i = (long long int)totalCombinacoes;
                            //printf("%d\n", i);
                            printf("tid: %d parou\n", tid);
                        }
                    }
                }
            }
        //incrementa a primeira variável no array
        password[0]++;
    
    }
    printf("\n\n");

	end_time = wtime();
		cout << end_time - start_time << endl;
}
