#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int sort(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
void* fakt(void *a){ 
	int f=1;
	int an=*((int *)a);
	int ab=an;
	while(an>0){
	f=f*an; an=an-1;
	}
	printf("%d!= %d\n", ab, f);
       }
int main(int argc, char** argv)
{
    int angka[100];
    for (int i = 1; i < argc; ++i)
    	angka[i-1]=atoi(argv[i]);
    qsort(angka,argc-1,sizeof(int),sort);
    
    for(int i=0; i < argc-1; i++){
	pthread_t tid;
	pthread_create(&tid,NULL,fakt,&angka[i]);
	pthread_join(tid,NULL);

	}
	

    return 0;
}



