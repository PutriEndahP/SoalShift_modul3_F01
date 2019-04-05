#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void* main_system(void *argv){
  int *arg=  (int *) argv;
  int id= *arg;

  // declare variable name
  char dir_name[100];
  char file_name[50];
  char zip_name[50];
  sprintf(dir_name, "~/Documents/FolderProses%d", id);
  sprintf(file_name, "SimpanProses%d.txt", id);
  sprintf(zip_name, "KompresProses%d.zip", id);
  //printf("%s\n%s\n%s\n", dir_name, file_name, zip_name);

  // hapus folder ( di echo ke /dev/null biar gak muncul error di terminal)
  char command_delete0[100];
  sprintf(command_delete0, "rm -rf %s &> /dev/null", dir_name);
  // printf("%s\n", command_delete0);
  printf("menghapus %s \n", dir_name);
  system(command_delete0);

  // buat folder
  char command0[100];
  sprintf(command0, "mkdir -p %s", dir_name);
  // printf("%s\n", command0);
  printf("membuat folder %s\n", dir_name);
  system(command0);
        
  // catat proses ke dalam file (lihat proses | ambil 10 baris pertama > catat ke file)
  char command1[100];
  sprintf(command1, "ps -aux | head -n10 > %s/%s", dir_name, file_name);
  // printf("%s\n", command1);
  printf("mencatat 10 baris pertama proses ke dalam %s/%s\n", dir_name, file_name);
  system(command1);

  // compress file ke zip
  char command2[100];
  sprintf(command2, "zip -r %s/%s %s/%s > /dev/null", dir_name, zip_name, dir_name, file_name);
  // printf("%s\n", command2);
  printf("menkompress %s menjadi %s\n", file_name, zip_name);
  system(command2);

  // hapus file
  char command_delete1[100];
  sprintf(command_delete1, "rm %s/%s &> /dev/null", dir_name, file_name);
  // printf("%s\n", command_delete1);
  printf("menghapus file %s\n", file_name);
  system(command_delete1);

  // tunggu 15 detik
  printf("tunggu 15 detik\n");
  sleep(15);

  // extract file zip
  char command3[100];
  sprintf(command3, "unzip -j %s/%s -d %s > /dev/null", dir_name, zip_name, dir_name);
  // printf("%s\n", command3);
  printf("mengekstrak file %s\n", zip_name);
  system(command3);

  // exit
  printf("program thread %d selesai\n", id);
  return 0;
}

int main(void){
  int max_thread=2, i;
  pthread_t tid[max_thread];

  // buat thread sejumlah max thread, parsing i sebagai id
  for(i=1; i<=max_thread; i++){
    pthread_create(&tid[i], NULL, main_system, &i);
    // butuh break sejenak buat menyimpan parsing parameter ke local variable tiap thread
    sleep(0.000000000000000001);
  }

  // join thread
  for(i=1; i<=max_thread; i++){
    pthread_join(tid[i], NULL);
  }

  return 0;
}
