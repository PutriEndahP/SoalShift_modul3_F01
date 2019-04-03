#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

// thread id
pthread_t tid1, tid2, tid3, tid4, tid5, tid6;

// main status
int WakeUpStatus= 0;
int SpiritStatus= 100;

// status fungsi dipanggil atau tidak
int func_allstatus=0;
int func_ayobangun=0;
int func_ayotidur=0;

// function counter
int count_ayobangun=0;
int count_ayotidur=0;

// apakah function bisa diakses atau tidak
int status_func_ayotidur=1;
int status_func_ayobangun=1;

// timer
int timer_ayobangun=0;
int timer_ayotidur=0;

// cek status program
int system_on= 1;

void* mainsystem(void *arg){
  // input string var
  char argv[256];
  // all input string
  char argv1[]="All Status";
  char argv2[]="Agmal Ayo Bangun";
  char argv3[]="Iraj Ayo Tidur";
  
  while(WakeUpStatus < 100 && SpiritStatus > 0 && system_on) {
    sleep(0.01);

    // mengosongkan array argv
    memset(argv, 0, 256);

    // masukan  input
    printf("Masukan argument : ");
    if (fgets(argv, sizeof argv, stdin) != NULL) {
      // menghitung panjang string
      size_t len = strlen(argv);
      // menghapus newline
      if (len > 0 && argv[len-1] == '\n') {
        argv[--len] = '\0';
      }
    }

    //printf("%s\n%s\n%s\n%s\n", argv, argv1, argv2, argv3);
    if(strcmp(argv, argv1) == 0){
      func_allstatus=1;
      sleep(0.01);
      continue;
    }
    else if(strcmp(argv, argv2) == 0){
      func_ayobangun=1;
      sleep(0.01);
      continue;
    }
    else if(strcmp(argv, argv3) == 0){
      func_ayotidur=1;
      sleep(0.01);
      continue;
    }
    else {
      printf("Invalid Input\n");
      sleep(0.01);
      continue;
    }
  }

  if (WakeUpStatus >= 100) printf("Agmal terbangun, mereka bangun pagi dan berolahraga\n");
  else if (SpiritStatus <= 0) printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
  else printf("System error\n");
 
  system_on=0;
  return NULL;
}

void* disable_ayobangun(void *arg){

  while(system_on){
    sleep(0.01);

    if (count_ayotidur > 0 && count_ayotidur%3==0){
      status_func_ayobangun=0;

      //sleep(10);

      timer_ayobangun=10;
      while (timer_ayobangun > 0){
        sleep(1);
        timer_ayobangun--;
      }

      status_func_ayobangun=1;
      count_ayotidur=0;
      continue;
    }
  }

  return NULL;
}

void* disable_ayotidur(void *arg){

  while(system_on){
    sleep(0.01);

    if (count_ayobangun > 0 && count_ayobangun%3==0){
      status_func_ayotidur=0;

      // sleep(10);

      timer_ayotidur=10;
      while (timer_ayotidur > 0){
        sleep(1);
        timer_ayotidur--;
      }

      status_func_ayotidur=1;
      count_ayobangun=0;
      continue;
    }
  }

  return NULL;
}

void* all_status(void *arg){

  while (system_on){
    sleep(0.01);

    if (func_allstatus){
      func_allstatus=0;
      printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", WakeUpStatus, SpiritStatus);
      continue;
    }
  }

  return NULL;
}

void* ayobangun(void *arg){

  while(system_on){
    sleep(0.01);

    if (!status_func_ayobangun && func_ayobangun){
      //printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
      printf("Fitur Agmal Ayo Bangun disabled %d s\n", timer_ayobangun);
      func_ayobangun=0;
    }
    else if(func_ayobangun){
      count_ayobangun++;
      WakeUpStatus+=15;
      func_ayobangun=0;
    }
  }

  return NULL;
}

void* ayotidur(void *arg){

  while(system_on){
    sleep(0.01);

    if(!status_func_ayotidur && func_ayotidur){
      //printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
      printf("Fitur Iraj Ayo Tidur disabled %d s\n", timer_ayotidur);
      func_ayotidur=0;
    }
    else if(func_ayotidur){
      count_ayotidur++;
      SpiritStatus-=20;
      func_ayotidur=0;
    }
  }

  return NULL;
}

int main(void) {
  pthread_create(&(tid1), NULL, ayotidur, NULL);
  pthread_create(&(tid2), NULL, ayobangun, NULL);
  pthread_create(&(tid3), NULL, all_status, NULL);
  pthread_create(&(tid4), NULL, disable_ayobangun, NULL);
  pthread_create(&(tid5), NULL, disable_ayotidur, NULL);
  pthread_create(&(tid6), NULL, mainsystem, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
  pthread_join(tid5, NULL);
  pthread_join(tid6, NULL);

  return 0;
}
