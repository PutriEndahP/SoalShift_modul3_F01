#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>

//using namespace std;

char nama_monster[11];
char pnjng[1], input;
int food, hunger_status, hygiene_status, health_status, time_hunger, time_hygiene;
int time_health,st,sh,ba, bath, lose, lawan, keluar;

//set food stock
	key_t key = 1234;
	int *food_stock;


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

void get_nama(){
	printf("input panjang nama(1-9 karakter) : ");
	pnjng[0] = getch();
	system("clear");
	printf("masukkan nama dengan panjang %c karakter\n",pnjng[0]);
	int i, n = atoi(pnjng);
	for(i=0;i<(n);i++){
		nama_monster[i] = getch(); 
		printf("%c",nama_monster[i]);
	}
	printf("\n");
	nama_monster[i] = '\0';
	system("clear");


return;
}

void *display(void *ptr){
	sh = 0;
	st = 1;
	ba = 0;
	time_hunger = 0;time_hygiene = 0;time_health = 0;bath = 0;food = 0;
	while(1){
		system("clear");
		if(time_hunger == 10){
			time_hunger = 0;
			hunger_status -= 5;
			if(hunger_status < 1){
				lose = 1;
				printf("game over\n%s mati kelaparan\n", nama_monster);
				
				return 0;
			}
		}
		if(time_hygiene == 1){
			time_hygiene = 0;
			hygiene_status -= 10;
			if(hygiene_status < 1){
				lose = 1;
				printf("game over\n%s mati membusuk karena tidak pernah mandi\n", nama_monster);
			
				return 0;
			}
		}
		if(time_health == 10){
			time_health = 0;
			if(st == 1){
				health_status += 5;
			}
			if(health_status > 300){health_status = 300;}
		}
		if(ba == 1){
			time_hunger = 0;time_hygiene = 0;time_health = 0;
		}
		time_hunger++;time_hygiene++;time_health++;

		if(st == 1 ){
			printf("Standby Mode\n");
			printf("Health : %d\n", health_status);
			printf("Hunger : %d\n", hunger_status);
			printf("Hygiene : %d\n", hygiene_status);
			printf("Food left : %d\n", food); 
			if(bath == 0){
				printf("Bath is ready\n");
			}else{
				printf("Bath will be ready in %ds\n",bath);
				bath--;
			}
			printf("Choices\n");
			printf("1. Eat\n");
			printf("2. Bath\n");
			printf("3. Battle\n");
			printf("4. Shop\n");
			printf("5. Exit\n");
			
			if(input == '1'){
				if(food>0){
					food--;
					hunger_status += 15;
					if(hunger_status > 200){hunger_status = 200;}
				}
			}else if(input == '2'){
				if(bath == 0){
					hygiene_status += 30;
					bath = 20;
					if(hygiene_status > 100){hygiene_status = 100;}
				}
			}else if(input =='3'){
				sh = 0;
				st = 0;
				ba = 1;
				lawan = 100;
			}else if(input == '4'){
				sh = 1;
				st = 0;
				ba = 0;
			}else if(input == '5'){
				keluar =1;
				return 0;
			}
		}else if(ba == 1){
			printf("Battle Mode\n");
			printf("Monster's Health : %d\n", health_status);
			printf("Enemy's Health : %d\n", lawan);
			printf("Choices\n");
			printf("1. Attack\n");
			printf("2. Run\n");
			if(input == '1'){
				health_status -= 20;
				lawan -= 20;
				if(health_status < 1){
					//("game over\n%s mati pada saat tawuran\n", nama_monster);
					sh = 0; st = 1; ba = 0;
					health_status =0 ;
				}
				if(lawan < 1){
					sh = 0; st = 1; ba = 0;
				}
			}else if(input == '2'){
				sh = 0; st = 1; ba = 0;
			}
		}else if(sh == 1){
			printf("Shop Mode\n");
			printf("Shop food stock : %d\n", *food_stock);
			printf("Your food stock : %d\n", food);
			printf("Choices\n");
			printf("1. Buy\n");
			printf("2. Back\n");
			if(input == '1'){
				if(*food_stock > 0){
					food++;
					*food_stock = *food_stock - 1;
				}
			}else if(input == '2'){
				sh = 0; st = 1; ba = 0;
			}
		}
		
		input = '0';
		sleep(1);
	}
}

void *inputgame(void *ptr){
	while(1){
		if(lose || keluar ){break;}
		input = getch();		
	}
}

int main()
{
	pthread_t thread1, thread2;
	int iret1,iret2;
	input = 0;	

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	food_stock = shmat(shmid, NULL, 0);

	//set status awal
	hunger_status = 200;
	hygiene_status = 100;
	health_status = 300;
	food = 0;
	
	//beri nama
	get_nama();

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
