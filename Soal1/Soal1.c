#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int fac[100];
int hasil[100];
int x=0;
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
        fac[x]=ab;
	hasil[x]=f;
	x++;
      }
int main(int argc, char** argv)
{
    int angka[100];
    for (int i = 1; i < argc; ++i)
    	angka[i-1]=atoi(argv[i]);
    
    pthread_t tid[100];
    for(int i=0; i < argc-1; i++){
	pthread_create(&tid[i],NULL,fakt,&angka[i]);

	}
    for(int i=0; i < argc-1; i++){
	pthread_join(tid[i],NULL);

	}
    qsort(fac,argc-1,sizeof(int),sort);
    qsort(hasil,argc-1,sizeof(int),sort);
    for(int i=0; i < argc-1; i++){
	printf("%d!= %d\n", fac[i], hasil[i]);

	}

    return 0;
}


