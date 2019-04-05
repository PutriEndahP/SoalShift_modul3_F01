#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

int health,hunger,hygiene,food_left,bath,monshealth, enehealth, foostock,yofoostock;

void standby(){
	printf("Standby Mode\n");	
	printf("Health : %d\n", health);
	printf("Hunger : %d\n", hunger);
	printf("Hygiene : %d\n", hygiene);
	printf("Food left : %d\n", food_left);
	if(bath==0){
	printf("Bath is ready\n");
	}
	else{
	printf("Bath will be ready in\n");
	}
	printf("Choices\n");
	printf("1. Eat    \n");
	printf("2. Bath   \n");
	printf("3. Battle \n");
	printf("4. Shop   \n");
	printf("5. Exit   \n");
	}
void battle(){
	printf("Battle Mode\n");	
	printf("Monster's Health : %d\n", monshealth);
	printf("Enemy's Health   : %d\n", enehealth);
	printf("Choices\n");
	printf("1. Attack\n");
	printf("2. Run   \n");
	}
void shop(){
	printf("Shop Mode\n");	
	printf("Shop food stock  : %d\n", foostock);
	printf("your food stock  : %d\n", yofoostock);
	printf("Choices\n");
	printf("1. Buy\n");
	printf("2. Back\n");
	}
	
int main(){
	char input[1], nama[10];
	printf("panjang nama(1-9) : ");
	input[0] = getch();
	int i, namas = (atoi(input));
	for(i=0;i<namas; i++){
		nama[i]=getch();
		
	}
	nama[i]='\0';
	printf("nama : %s\n", nama);
	hunger = 200;
	hygiene = 100;
	health = 300;
	food_left = 0;
	
	standby();
	battle();
	shop();

	}
	
