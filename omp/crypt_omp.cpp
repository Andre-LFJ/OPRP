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


//#pragma omp parallel //firstprivate(temp, salt, data, keyHash)
int testaCrypt(char temp[], char salt[], crypt_data data, char keyHash[] ){
    
    if(crypt_r(temp, salt, &data) == NULL){
        printf("deu ruim");
        return -1;
        //break;
    }
    if (strcmp(keyHash, data.output) == 0)
    {      
        int tid = omp_get_thread_num();
        printf("%d = ", tid);

        //
        printf("%d: chave encontrada = %s \n", tid, data.output);
        //std::cout << data.output << "\n";
        return 1;

        //break;
    }

    
    return 0;
}

double wtime(){
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + t.tv_usec / 1000000.0;
}



using namespace std;
int main(void){
    //char key[BUFSIZE] = "cabbbccbbbab";          //The string you want to hash
    char key[BUFSIZE] = "cccccccccccc";          

    double start_time, end_time;

    int tamanho = strlen(key);
    printf("tamanho: %d\n", tamanho);
    char salt_origin[BUFSIZE] = "aa";     //Salt.
    char salt[BUFSIZE];

    struct crypt_data data;                 //Structure required for hash result storage
    data.initialized = 0;                   // crypt_r()Must be done before using the function.

    sprintf(salt, "$1$%s", salt_origin);    //id specification((See below)

    crypt_r(key, salt, &data);

    printf("%s\n", data.output);

    char keyHash [BUFSIZE];
    strcpy(keyHash, data.output);
    //printf("%s\n", data.output);                    //The output member variable
                                            //String"$5$example$(Hashed key)"Is stored



    //VARIABLES
    //char chars[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char chars[] ={'a','b','c','d'};
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
    int execucoesPorThread = 48;
    int totalExecucoes = numthreads * execucoesPorThread;
    
    //SIZES OF VERIABLES
    int chars_length = sizeof(chars) / sizeof(char);
    int password_length = sizeof(password) / sizeof(int);
    float totalCombinacoes = pow(chars_length, password_length);
    
    printf("total de combinações: %.0f\n", totalCombinacoes);
    printf("numThreads: %d | execucoesPorThread: %d | totalExecucoes: %d \n", numthreads, execucoesPorThread, totalExecucoes);
    printf("\n\n");


    int tid;
    //char matriz[(int)totalCombinacoes][tamanho];
    char matriz[totalExecucoes][tamanho];
//printf("numthreads: ")
    int palavrasNaMatriz=0;
    long long int palavrasRestantes = (long long int)totalCombinacoes;
int stop = 0;
start_time = wtime();

    //#pragma omp parallel for private(j,teste, temp) shared(i) firstprivate(chars, chars_length,tamanho,totalCombinacoes,keyHash,password_length,password)  num_threads(numthreads)
    //printf("%lli",(long long int)totalCombinacoes);
            printf("%d | %lli \n",i, (long long int)totalCombinacoes);
            for (i = 0; i <  (long long int)totalCombinacoes; i++){ //CYCLE TROUGH ALL OF THE COMBINATIONS

            //printf("comeco: %d | fim: %d \n", (int)(omp_get_thread_num()*(totalCombinacoes/numthreads)), (int)((omp_get_thread_num()+1)*(totalCombinacoes/numthreads)));
                //printf("%d = ", tid);
                
                //printf("tid = %d | i = %d | min = %d | max = %d | ", tid,  i,(int)((omp_get_thread_num())*(totalCombinacoes/numthreads)), (int)((1+omp_get_thread_num())*(totalCombinacoes/numthreads)));
                //continue;
                //#pragma omp critical
                for (j=0; j < password_length; j++) {//CYCLE TROUGH ALL OF THE VERIABLES IN ARRAY
                    if (password[j] == chars_length) {//IF VERIABLE IN "PASSWORD" ARRAY IS THE LAST VERIABLE IN CHAR "CHARS" ARRRAY
                        password[j + 1]++;//THEN INCREMENT THE NEXT VERIABLE IN "PASSWORD" ARRAY
                        password[j] = 0;//AND RESET THE VERIABLE BACK TO ZERO
                    }
                }


                //printf("%d\n", tamanho);
                //char temp[tamanho];

                //PRINT OUT FIRST COMBINATION
                //std::cout << i << ": ";
                //#pragma omp critical
                for (int j = 0; j < password_length; j++) {
                    //std::cout << chars[password[j]] << "";
                    temp[j] = chars[password[j]];
                }
                //printf("%s == \n", temp);
                //std::cout << "\n";
                temp[tamanho] ='\0';   

                strcpy(matriz[m],temp);
                m=m+1;
                palavrasNaMatriz = palavrasNaMatriz+1;
                palavrasRestantes = palavrasRestantes-1;

                    if ((palavrasNaMatriz == totalExecucoes) || (palavrasRestantes<execucoesPorThread)){
                        palavrasNaMatriz = 0;
                        m=0;
                        
                        //printf("palavrasNaMatriz %d == numthreads %d  \n" , palavrasNaMatriz, numthreads);
                        #pragma omp parallel shared(stop, i) private(k, l, tid) firstprivate(execucoesPorThread, totalExecucoes, temp, temp2, salt, data, tamanho, matriz, totalCombinacoes, numthreads, keyHash)  num_threads(numthreads)
                        {
                            tid = omp_get_thread_num();
                            for(k = tid*execucoesPorThread; k < (tid+1)*execucoesPorThread; k++){ 
                            //printf("tid = %d | k= %d | k < %d\n", tid,tid*execucoesPorThread,(tid+1)*execucoesPorThread);
                                for(int l = 0; l < tamanho; l++){ 
                                    temp2[l] = matriz[k][l];
                                    //printf("k: %d | l: %d | caractere: %c \n", k, l,  matriz[k][l]);
                                }
                                temp2[tamanho] = '\0';
                                //printf("tamanho: %d | palavra: %s \n",tamanho, temp2);
                                crypt_r(temp2, salt, &data);
                                //printf("%s", data.output);
                                if (strcmp(keyHash, data.output) == 0)
                                {      
                                    //int tid = omp_get_thread_num();
                                    //printf("%d = ", tid);
                                    printf("tid: %d | encontrou a chave: \'%s\' | hash: \'%s\' \n", tid, temp2, data.output);
                                    // i = (int)totalCombinacoes;
                                    stop = 1;                                   
                                }
                                
                                if(stop == 1){
                                    k = numthreads*execucoesPorThread;
                                    i = (long long int)totalCombinacoes;
                                    //printf("%d\n", i);
                                    printf("tid: %d parou\n", tid);
                                }

    //printf("saiu");
                                //testaCrypt(temp, salt, data, keyHash);
                                //printf("\n");
                            }
                        }
                    }

                //printf("thread num: %d\n", omp_get_thread_num());





                
                //teste = testaCrypt(temp, salt, data, keyHash);
                
                
                //printf("%d\n", teste);





                //#pragma omp parallel

                //std::cout << " ";



                //char * cryptOut = crypt_r(temp, salt, &data);
                //std::cout << salt <<"\n";
                //std::cout << data.output << "\n";
                // if(crypt_r(temp, salt, &data) == NULL){
                //     printf("deu ruim");
                //     //break;
                // }
                // if (strcmp(keyHash, data.output) == 0)
                // {
                //     printf("chave encontrada\n");
                //     std::cout << data.output << "\n";
                //     //break;
                // }
                

                //printf("data.output: %s\n", data.output);
                //printf("keyHash: %s\n", keyHash);
                //if(strcmp(keyHash, data.output) == 0){
                //     printf("chave encontrada\n");
                //     std::cout << data.output << "\n";
                //     //break;
                    
                // }

                //INCREMENT THE FIRST VERIABLE IN ARRAY
                password[0]++;
                //printf("password %d ", password[]);
            
        }
    
    
    //unsigned int microsecond = 1000000;
    //usleep(5 * microsecond);

    //printf("==============\n");
    //printf("%s\n", data.output);
    printf("\n\n");
    

	end_time = wtime();
		cout << end_time - start_time << endl;
    


    //======TESTE=======
    // char encrypted[BUFSIZE]; //For storing results  
    // strcpy(encrypted, crypt(key, salt)); //The encrypted variable
    // printf("%s\n", encrypted);    

}


























/*

#include<crypt.h>
#include<stdio.h>
#include<string.h>
#define BUFSIZE 1024

int main(void){
  char key[BUFSIZE] = "carro"; //The string you want to hash
  char salt_origin[BUFSIZE] = "aa"; //Salt string
  char salt[BUFSIZE]; 
  char encrypted[BUFSIZE]; //For storing results

  sprintf(salt, "$1$%s", salt_origin); //Salt shaping, id specification (described later)
  printf("%s\n", salt);
  strcpy(encrypted, crypt(key, salt)); //The encrypted variable
                                       //String"$6$example$(Hashed key)"Is stored
  
  printf("%s\n", encrypted);
}
*/