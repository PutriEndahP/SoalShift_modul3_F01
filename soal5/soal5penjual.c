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

int foostock;

void shop(){
	printf("Shop Mode\n");	
	printf("Food stock  : %d\n", foostock);
	printf("Choices\n");
	printf("1. Restock\n");
	printf("2. Exit\n");
	}
	
int main(){
	char input;
	input=getch();
	printf("hasil input %c", input);

	foostock = 0;
	
	shop();

	}
	
