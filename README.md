# SoalShift_modul3_F01

1. Putri Endah Puspitasari (05111740000039)<br>
2. Garda Negara Wisnumurti (05111740000153)

## Soal 1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
    
    ./faktorial 5 3 4
    
    3! = 6
    
    4! = 24
    
    5! = 120

### Jawaban
```javascript
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
```

### Penjelasan
Di line 41 terdapat parameter yang digunakan untuk menampung argumen. Line 42-43 untuk mengubah argumen ke dalam bentuk tipe data int. Line 44 digunakan untuk menyortir argumen dari kecil ke yang terbesar melalui fungsi sort yang terdapat pada line 27-29. Di line 46 terdapat looping yang digunakan untuk mengakses fungsi fakt untuk perhitungan faktorial dan terdapat thread join yang digunakan agar proses berjalan bergantian.

## Soal 2
Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:

a) Terdapat 2 server: server penjual dan server pembeli

b) 1 server hanya bisa terkoneksi dengan 1 client

c) Server penjual dan server pembeli memiliki stok barang yang selalu sama

d) Client yang terkoneksi ke server penjual hanya bisa menambah stok

 * Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1

e) Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok

  * Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1

f) Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok

  * Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”

  * Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”

g) Server penjual akan mencetak stok saat ini setiap 5 detik sekali

h) Menggunakan thread, socket, shared memory

### Jawaban 


### Penjelasan

## Soal 3
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
 
a) Terdapat 2 karakter Agmal dan Iraj

b) Kedua karakter memiliki status yang unik
    
   * Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
   
   * Iraj memiliki Spirit_Status, di awal program memiliki status 100
    
   * Terdapat 3 Fitur utama
        
        * All Status, yaitu menampilkan status kedua sahabat
        
            Ex: Agmal WakeUp_Status = 75 
            
            Iraj Spirit_Status = 30
        
        * “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
        
        * “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
    
   * Terdapat Kasus yang unik dimana:
        
        * Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik           (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
        
        * Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
    
   * Program akan berhenti jika Salah Satu :
        
        * WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
        
        * Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)

   * Syarat Menggunakan Lebih dari 1 Thread

### Jawaban
```javascript
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
//int timer_ayobangun=0;
//int timer_ayotidur=0;

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

      sleep(10);

      //timer_ayobangun=10;
      //while (timer_ayobangun > 0){
        //sleep(1);
        //timer_ayobangun--;
      //}

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

      sleep(10);

      //timer_ayotidur=10;
      //while (timer_ayotidur > 0){
        //sleep(1);
        //timer_ayotidur--;
     // }

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
      printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
      //printf("Fitur Agmal Ayo Bangun disabled %d s\n", timer_ayobangun);
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
      printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
      //printf("Fitur Iraj Ayo Tidur disabled %d s\n", timer_ayotidur);
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
```

### Penjelasan
* Pertama-tama deklarasikan dahulu variabel variabel yang akan dibutuhkan untuk fungsi fungsi yang digunakan, seperti :
```javascript
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
//int timer_ayobangun=0;
//int timer_ayotidur=0;

// cek status program
int system_on= 1;
```
* Program akan berjalan selama syarat  ```while(WakeUpStatus < 100 && SpiritStatus > 0 && system_on)``` terpenuhi

* Kami menggunaan 6 thread, satu thread untuk setiap 1 fungsi yaitu thread untuk main system, all status, ayo bangun, ayo tidur, disable ayo bangun dan disable ayo tidur.

* Kita masukkan masing-masing string input ke dalam variabel, seperti pada source code berikut ini :
```javascript
  char argv1[]="All Status";
  char argv2[]="Agmal Ayo Bangun";
  char argv3[]="Iraj Ayo Tidur";
```
* Kemudian masukkan inputnya, bandingkan input yang di masukkan apakah sama dengan salah satu yang ada di variabel yang sudah di deklarasikan sebelumnya, misalnya
```javascript
 if(strcmp(argv, argv1) == 0){
      func_allstatus=1;
      sleep(0.01);
      continue;
```
misalnya yang di inputkan itu sama dengan argv[1] yang berisi string "all status", maka fungsi func_allstatus di panggil, begitu seterusnya.

* jika terdapat kondisi seperti berikut ini 
```javascript
  if (WakeUpStatus >= 100) printf("Agmal terbangun, mereka bangun pagi dan berolahraga\n");
  else if (SpiritStatus <= 0) printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
  else printf("System error\n");
 
  system_on=0;\n");
  ``` 
maka apabila WakeUpStatus nya sudah lebih dari 100 maka akan mencetak tulisan "Agmal terbangun, mereka bangun pagi dan berolahraga" tetapi jika SpiritStatus nya kurang dari atau sama dengan 0 maka mencetak "Iraj ikut tidur, dan bangun kesiangan bersama Agmal"    
kemudian system akan mati

* Kemudian di masukkan fungsi fungsi lainnya yang dibutuhkan.

* Buat ke enam thread yang akan di butuhkan, dan joinkan thread nya dengan syntax berikut ini :
```javascript
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

```
## Soal 4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 

* Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama

* Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama

* Ketika Mengekstrak file .zip juga harus secara bersama-sama

* Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”

* Wajib Menggunakan Multithreading

* Boleh menggunakan system

### JAwaban
```javascript
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
```

### Penjelasan

* Dalam kasus ini kami menggunakan 2 thread yaitu satu untuk folder proses 1 dan satunya lagi untuk folder proses 2.

* Source code pada main berarti kami melakukan perulangan untuk membuat thread, dalam kasus ini kedua thread ini meng akses satu alamat poiter yang sama. Sehingga pada source code di main seperti berikut :
```javascript
int max_thread=2, i;
  pthread_t tid[max_thread];

  // buat thread sejumlah max thread, parsing i sebagai id
  for(i=1; i<=max_thread; i++){
    pthread_create(&tid[i], NULL, main_system, &i);
    // butuh break sejenak buat menyimpan parsing parameter ke local variable tiap thread
    sleep(0.000000000000000001);
```
berarti kita meng inisialisasi maksimal thread yaitu 2, dengan i = 1, kemudian ada sleep karena kedua thread ini akan mengakses satu alamat poiter yang sama, maka apabila perulangan kedua i nya berganti menjadi 2, maka perulangan pertama akan ikut menjadi dua. Alhasil variabel local thread yaitu 2 dan 2, oleh karena itu perlu diberi sedikit sleep agar masing-masing thread bisa menyimpan parsing ke variabel local masing-masing thread.

* Yang pertama dilakukan yaitu men deklarasikan variabel-variabel yang akan digunakan nantinya, seperti :
```javascript
char dir_name[100];
  char file_name[50];
  char zip_name[50];
  sprintf(dir_name, "~/Documents/FolderProses%d", id);
  sprintf(file_name, "SimpanProses%d.txt", id);
  sprintf(zip_name, "KompresProses%d.zip", id);
  //printf("%s\n%s\n%s\n", dir_name, file_name, zip_name);
```

* Kemudian awalnya kita membuat fungsi command delete yang berfungsi agar saat kita menjalankan program untuk kedua kalinya, maka folder sebelumnya akan terhapus terlebih dahulu, jadi tidak perlu menghapus manual, dengan menggunakan syntax berikut ini :
```javascript
char command_delete0[100];
  sprintf(command_delete0, "rm -rf %s &> /dev/null", dir_name);
  // printf("%s\n", command_delete0);
  printf("menghapus %s \n", dir_name);
  system(command_delete0);
```
/dev/null ini digunakan agar jika terjadi error atau apapun tidak terlihat di terminal.

* Buat folder untuk menympan file zip sesuai dengan perintah yang ada di soal dengan syntax :
```javascript
 char command0[100];
  sprintf(command0, "mkdir -p %s", dir_name);
  // printf("%s\n", command0);
  printf("membuat folder %s\n", dir_name);
  system(command0);
```
kami menggunakan system untuk memanggil suatu fungsi agar lebih mudah.

* Kemudian kami menggunakan fungsi-fungsi seperti biasa dengan struktur yang sama untuk fungsi lainnya dengan tetap menggunakan system. Program akan berhenti selama 15 detik kemudian akan melanjutkan mengerjakan fungsi berikutnya yaitu unzip.

## Soal 5
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta.

a) Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.

b) Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.

c) Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).

d) Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.

e) Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 

f) Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.

* Pembeli (terintegrasi dengan game)

    * Dapat mengecek stok makanan yang ada di toko.

    * Jika stok ada, pembeli dapat membeli makanan.

* Penjual (terpisah)

    * Bisa mengecek stok makanan yang ada di toko

    * Penjual dapat menambah stok makanan.
    
    Spesifikasi program:

A. Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)

B. Program terdiri dari 3 scene yaitu standby, battle, dan shop.

C. Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :

    Standby Mode
    Health : [health status]
    Hunger : [hunger status]
    Hygiene : [hygiene status]
    Food left : [your food stock]
    Bath will be ready in [cooldown]s
    Choices
    1. Eat
    2. Battle
    3. Battle
    4. Shop
    5. Exit

D. Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :

    Battle Mode
    Monster’s Health : [health status]
    Enemy’s Health : [enemy health status]
    Choices
    1. Attack
    2. Run

E. Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :

        Shop Mode
        Shop food stock : [shop food stock]
        Your food stock : [your food stock]
        Choices
        1. Buy
        2. Back

F. Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :

    Shop
    Food stock : [shop food stock]
    Choices
    1. Restock
    2. Exit

G. Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

### Jawaban

### Penjelasan
Jawaban masih belum rampung, masih dalam pengerjaan. Program saya masih tampilan dan mampu mendeteksi input berupa key press
