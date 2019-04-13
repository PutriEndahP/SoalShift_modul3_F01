#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// define port yang akan digunakan
#define PORT 9999

// define ip ddress yang akan digunakan
#define IP_ADDR "127.0.0.1"
  
int main(int argc, char const *argv[]) {
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  // create socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }
  
  // check if server_jual_status has connection
  key_t key2 = 1234;
  int *server_jual_status;
  int shmid2 = shmget(key2, sizeof(int), IPC_CREAT | 0666);
  server_jual_status = shmat(shmid2, NULL, 0);

  if (*server_jual_status > 0){
    printf("Server Sibuk :(\n");
    return 0;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
    
  if(inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  int stat= connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (stat < 0 ) {
    printf("\nConnection Failed \n");
    return -1;
  }
  else {
    printf("\nConnection Success \n");
  }

  char string[100];
  while(1){
    memset(string, 0, 100);
    scanf("%s",string);
    send(sock , string , strlen(string) , 0 );
  }
  
  return 0;
}
