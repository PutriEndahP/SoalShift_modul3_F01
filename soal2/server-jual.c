#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

// define port yang akan digunakan
#define PORT 9999

// thread id
pthread_t tid[100];

// global var
int *stock;
int new_socket;
char message_buffer[1024] = {0};

void* check_message(void* arg) {
  // jika server menerima message tambah dari client, maka stock ditambah 1
  if (strcmp(message_buffer, "tambah") == 0) *stock+=1;

  // kosongkan message_buffer
  memset(message_buffer, 0, 1024);

  return NULL;
}

void* print_current_stock(void* arg){
  // cetak stock setiap 5 detik
  while(1){
    printf("stock: %d\n",*stock);
    sleep(5);
  }
}

int main (int argc, char const *argv[]){
  // get ip address
  struct sockaddr_in address;
  // panjang address
  int addrlen = sizeof(address);
  int opt = 1;
  int server_jual, valread;

  // init stock on share mem
  key_t key=4321;
  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  stock = shmat(shmid, NULL, 0);
  *stock = 0;

  // init server status on share mem
  key_t key2 = 1234;
  int *server_jual_status;
  int shmid2 = shmget(key2, sizeof(int), IPC_CREAT | 0666);
  server_jual_status = shmat(shmid2, NULL, 0);
  *server_jual_status=0;

  // create thread untuk print
  pthread_create(&(tid[0]),NULL,print_current_stock,NULL);

  // create socket server_jual
  if ((server_jual = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // set server jual untuk menerima multiple connection
  if (setsockopt(server_jual, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // type of socket created 
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  // buka socket di localhost dengan port 8888
  if (bind(server_jual, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  // try to specify maximum of 3 pending connections for the master socket  
  if (listen(server_jual, 3) < 0){   
    perror("listen"); 
    exit(EXIT_FAILURE);
  }

  // accept the incoming connection  
  puts("Waiting for connections ...");

  // accept only one connection
  if ((new_socket = accept(server_jual, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  else {
    // tell other that server has connection
    *server_jual_status=1;
  }

  //inform user of socket number - used in send and receive commands  
  printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , PORT);

  int index=1;
  while(1){
    valread = read( new_socket , message_buffer, 1024);
    if (valread == 0){ 
      printf("Client disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , PORT);           
      close( new_socket );
      exit(EXIT_FAILURE);
    }
    else {
      pthread_create(&(tid[index]),NULL,check_message,NULL);
      pthread_join(tid[index],NULL);
      index++;
    }
  }

  return 0;
}
