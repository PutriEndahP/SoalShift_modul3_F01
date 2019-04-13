#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>

//using namespace std;
char input;
int keluar;

//set food stock
	key_t key = 1234;
	int *food_stock = 0;

int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

void *display(void *ptr){
	while(1){
		system("clear");
		printf("Shop\n");
		printf("Food stock : %d\n", *food_stock);
		printf("Choices\n");
		printf("1. Restock\n");
		printf("2. Exit\n");
		if(input == '1'){
			*food_stock = *food_stock + 1;
		}else if(input == '2'){
			keluar =1;
			return 0;
		}
		input = '0';
		sleep(1);
	}	
}

void *inputgame(void *ptr){
	while(1){
		if(keluar){break;}
		input = getch();		
	}
}

int main()
{
	pthread_t thread1, thread2;
	int iret1,iret2;
	input = '0';	

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	food_stock = shmat(shmid, NULL, 0);

	iret1 = pthread_create( &thread1, NULL, display, NULL); //membuat thread pertama
    if(iret1) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

	iret2 = pthread_create( &thread2, NULL, inputgame, NULL);//membuat thread kedua
    if(iret2)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
		
		

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	shmdt(food_stock);
        shmctl(shmid, IPC_RMID, NULL);

return 0;
}
