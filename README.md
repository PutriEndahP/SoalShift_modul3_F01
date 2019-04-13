# Laporan Resmi Modul 3

1. Putri Endah Puspitasari (05111740000039)<br>
2. Garda Negara Wisnumurti (05111740000153)

## Modul 3 Sistem Operasi

### Thread dan IPC

### Objectives
1. Peserta mengetahui thread
2. Peserta memahami bagaimana thread bekerja
3. Peserta memahami bagaimana cara membuat thread

- [Thread dan IPC](#thread-dan-ipc)
  - [Objectives](#objectives)
  - [1. Thread](#1-thread)
    - [1.1 Thread](#11-thread)
    - [1.2 Join Thread](#12-join-thread)
    - [1.3 Mutual Exclusion](#13-mutual-exclusion)
  - [2. IPC (Interprocess Communication)](#2-ipc-interprocess-communication)
    - [2.1 IPC](#21-ipc)
    - [2.2 Pipes](#22-pipes)
    - [2.3 Sockets](#23-sockets)
    - [2.4 Message Queues](#24-message-queues)
    - [2.5 Semaphores](#25-semaphores)
    - [2.6 Shared Memory](#26-shared-memory)
  - [Appendix](#appendix)
    - [Libraries documentation (and functions)](#libraries-documentation-and-functions)
  - [Soal Latihan](#soal-latihan)
      - [Latihan 1](#latihan-1)
      - [Latihan 2](#latihan-2)
      - [Latihan 3](#latihan-3)
    - [References](#references)


### 1. Thread 
#### 1.1 Thread
Thread merupakan unit terkecil dalam suatu proses yang dapat dijadwalkan oleh sistem operasi. Thread biasanya terbentuk oleh `fork` yang berjalan pada suatu script atau program untuk sebuah proses. Minimal terdapat sebuah thread yang berjalan dalam suatu proses, walau biasanya terdapat lebih dari satu thread dalam proses tersebut. Thread akan berbagi memori dan menggunakan informasi (nilai) dari variabel-variabel pada suatu proses tersebut. Penggambaran thread pada sebuah proses dapat dilihat sebagai berikut.

![thread](img/thread2.png)

Untuk melihat thread yang sedang berjalan, gunakan perintah :
```bash
top -H
```

Thread dapat dibuat menggunakan fungsi pada program berbahasa C :
```c
#include <pthread.h> //library thread

int pthread_create(pthread_t *restrict tidp,
                   const pthread_attr_t *restrict attr,
                   void *(*start_rtn)(void *),
                   void *restrict arg);

/* Jika berhasil mengembalikan nilai 0, jika error mengembalikan nilai 1 */
```
Penjelasan Syntax:
- Pointer `tidp` digunakan untuk menunjukkan alamat memori dengan thread ID dari thread baru.
- Argumen `attr` digunakan untuk menyesuaikan atribut yang digunakan oleh thread. nilai `attr` di-set `NULL` ketika thread menggunakan atribut *default*.
- Thread yang baru dibuat akan berjalan dimulai dari fungsi `start_rtn` dalam fungsi thread.
- Pointer `arg` digunakan untuk memberikan sebuah argumen ke fungsi `start_rtn`, jika tidak diperlukan argumen, maka `arg` akan di-set `NULL`.

Contoh membuat program tanpa menggunakan thread [playtanpathread.c](playtanpathread.c):

```c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t child;
	int i, stat;
	char *argv1[] = {"clear", NULL};
	char *argv2[] = {"xlogo", NULL};
	child = fork();
	if (child==0) {
		execv("/usr/bin/clear", argv1);
	}
	else
	{
		for(i=0;i<6;i++)
		{
			printf("%d\n",i);
			fflush(stdout);
			sleep(1);
		}
		execv("/usr/bin/xlogo", argv2);
	}
	
}

```

Contoh membuat program menggunakan thread [playthread.c](playthread.c) :
> compile dengan cara `gcc -pthread -o [output] input.c`

```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread

int length=5; //inisialisasi jumlah untuk looping
void* playandcount(void *arg)
{
	char *argv1[] = {"clear", NULL};
	char *argv2[] = {"xlogo", NULL};
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	if(pthread_equal(id,tid[0])) //thread untuk menjalankan counter
	{
		for(iter=0;iter<6;iter++)
		{
			printf("%d\n",iter);
			fflush(stdout);
			sleep(1);
		}
	}
	else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
	{
		execv("/usr/bin/xlogo", argv2);
	}
	else if(pthread_equal(id,tid[2])) // thread menutup gambar
	{
		execv("/usr/bin/pkill", argv2);
	}

	return NULL;
}

int main(void)
{
	int i=0;
	int err;
	while(i<2) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&playandcount,NULL); //membuat thread
		if(err!=0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		}
		i++;
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	exit(0);
	return 0;
}


```

**Kesimpulan** :
Terlihat ketika program menggunakan thread dapat menjalankan dua task secara bersamaan (task menampilkan gambar dan task timer).

#### 1.2 Join Thread
Join thread adalah fungsi untuk melakukan penggabungan dengan thread lain yang telah berhenti (*terminated*). Bila thread yang ingin di-join belum dihentikan, maka fungsi ini akan menunggu hingga thread yang diinginkan berstatus **`Terminated`**. Fungsi `pthread_join()` ini dapat dikatakan sebagai fungsi `wait()` pada proses, karena program (*task*) utama akan menunggu thread yang di-join-kan pada program utama tersebut. Kita tidak mengetahui program utama atau thread yang lebih dahulu menyelesaikan pekerjaannya.

Contoh program C Join_Thread [thread_join.c](thread_join.c):

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread

void *print_message_function( void *ptr );

int main()
{
    pthread_t thread1, thread2;//inisialisasi awal
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2;

    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1); //membuat thread pertama
    if(iret1) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);//membuat thread kedua
    if(iret2)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }

    printf("pthread_create() for thread 1 returns: %d\n",iret1);
    printf("pthread_create() for thread 2 returns: %d\n",iret2);

    // pthread_join( thread1, NULL);
    // pthread_join( thread2, NULL); 

    exit(EXIT_SUCCESS);
}

void *print_message_function( void *ptr )
{
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);

    for(int i=0;i<10;i++){
        printf("%s %d \n", message, i);
    }
}

```

Keterangan :
- Pada program di atas, jika kita *comment* baris `pthread_join`, maka hasil yang didapat tidak akan memunculkan tulisan **Thread 1** dan **Thread 2**.
- Jika pemanggilan fungsi `pthread_join` di-uncomment, maka program yang kita buat akan memunculkan tulisan **Thread 1** dan **Thread 2**.

**Kesimpulan** :
Pada program pertama tidak menjalankan fungsi `print_message_function` karena sebelum kedua thread dijadwalkan, program utama (kemungkinan) telah selesai dieksekusi sehingga tidak menjalankan fungsi bawaan pada thread. Pada percobaan kedua, fungsi `pthread_join()` digunakan untuk membuat program utama menunggu thread yang *join* hingga target thread selesai dieksekusi, dengan fungsi ini program utama di-suspend hingga target thread selesai dieksekusi.
- Fungsi untuk terminasi thread
  ```c
  #include <pthread.h>
  void pthread_exit(void *rval_ptr);
  ```
  Argumen `rval_ptr` adalah pointer yang digunakan yang dapat diakses oleh fungsi `pthread_join()` agar dapat mengetahui status thread tersebut

- Fungsi untuk melakukan join thread 
  ```c
  int pthread_join(pthread_t thread, void **rval_ptr);
  /* Jika berhasil mengembalikan nilai 0, jika error mengembalikan nilai 1 */
  ```
  Fungsi akan menunda pekerjaan sampai status pointer `rval_ptr` dari fungsi `pthread_exit()` mengembalikan nilainya.

#### 1.3 Mutual Exclusion
Disebut juga sebagai **Mutex**, yaitu suatu cara yang menjamin jika ada pekerjaan yang menggunakan variabel atau berkas digunakan juga oleh pekerjaan yang lain, maka pekerjaan lain tersebut akan mengeluarkan nilai dari pekerjaan sebelumnya.

Contoh program Simple Mutual_Exclusion [threadmutex.c](threadmutex.c):
```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
 
pthread_t tid1, tid2;
int status;
int nomor;
 
void* tulis(void *arg)
{
    status = 0;
 
    printf("Masukan nomor : ");
    scanf("%d", &nomor);
 
    status = 1;
 
    return NULL;
}

void* baca(void *arg)
{
    while(status != 1)
    {

    }

    printf("Nomor %d\n", nomor);
}
 
int main(void)
{

    pthread_create(&(tid1), NULL, tulis, NULL);
    pthread_create(&(tid2), NULL, baca, NULL);
 
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
 
    return 0;
}

```

Keterangan :
- Terdapat 2 buah thread yang berjalan dengan fungsi yang berbeda.
- Sumber daya (variabel) yang digunakan kedua thread untuk mengeksekusi pekerjaannya **sama**.
- Variabel `status` adalah contoh simple untuk mengendalikan jalannya thread.

**Kesimpulan** :
Karena kita tidak mengetahui *thread* mana yang lebih dahulu mengeksekusi sebuah variable atau sumber daya pada program, kegunaan dari **Mutex** adalah untuk menjaga sumber daya suatu thread agar tidak digunakan oleh thread lain sebelum ia menyelesaikan pekerjaannya.


### 2. IPC (Interprocess Communication)
#### 2.1 IPC
IPC (*Interprocess Communication*) adalah cara atau mekanisme pertukaran data antara satu proses dengan proses lain, baik pada komputer yang sama atau komputer jarak jauh yang terhubung melalui suatu jaringan.

#### 2.2 Pipes
Pipe merupakan komunikasi sequensial antar proses yang saling terelasi. Kelemahannya, hanya dapat digunakan untuk proses yang saling berhubungan dan secara sequensial.
Terdapat dua jenis pipe:
- unnamed pipe : Komunikasi antara parent dan child proses.
- named pipe : Biasa disebut sebagai FIFO, digunakan untuk komunikasi yang berjalan secara independen. **Hanya bisa digunakan jika kedua proses menggunakan *filesystem* yang sama.**
```
$ ls | less
```
Diagram :

![alt](img/pipe.png "Diagram Pipe")  

Syntax in C languange :
```
int pipe(int fds[2]);

Parameters :
fd[0] will be the fd(file descriptor) for the 
read end of pipe.
fd[1] will be the fd for the write end of pipe.
Returns : 0 on Success.
-1 on error.
```
Example :  
[pipe1.c](pipe1.c)

```c
// C program to illustrate 
// pipe system call in C 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

#define MSGSIZE 16 
char* msg1 = "hello, world #1"; 
char* msg2 = "hello, world #2"; 
char* msg3 = "hello, world #3"; 

int main() 
{ 
	char inbuf[MSGSIZE]; 
	int p[2], i; 

	if (pipe(p) < 0) 
		exit(1); 

	/* continued */
	/* write pipe */

	write(p[1], msg1, MSGSIZE); 
	write(p[1], msg2, MSGSIZE); 
	write(p[1], msg3, MSGSIZE); 

	for (i = 0; i < 3; i++) { 
		/* read pipe */
		read(p[0], inbuf, MSGSIZE); 
		printf("%s\n", inbuf); 
	} 
	return 0; 
} 

```  
Output :  
```
hello, world #1
hello, world #2
hello, world #3
```  
Pipe dengan fork  
Diagram :  
![alt](img/pipe-fork.png)  

Example :  
[pipe-fork](pipe-fork.c)  
```c
// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
	// We use two pipes 
	// First pipe to send input string from parent 
	// Second pipe to send concatenated string from child 

	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	char fixed_str[] = "forgeeks.org"; 
	char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	scanf("%s", input_str); 
	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		char concat_str[100]; 

		close(fd1[0]); // Close reading end of first pipe 

		// Write input string and close writing end of first 
		// pipe. 
		write(fd1[1], input_str, strlen(input_str)+1); 
		close(fd1[1]); 

		// Wait for child to send a string 
		wait(NULL); 

		close(fd2[1]); // Close writing end of second pipe 

		// Read string from child, print it and close 
		// reading end. 
		read(fd2[0], concat_str, 100); 
		printf("Concatenated string %s\n", concat_str); 
		close(fd2[0]); 
	} 

	// child process 
	else
	{ 
		close(fd1[1]); // Close writing end of first pipe 

		// Read a string using first pipe 
		char concat_str[100]; 
		read(fd1[0], concat_str, 100); 

		// Concatenate a fixed string with it 
		int k = strlen(concat_str); 
		int i; 
		for (i=0; i<strlen(fixed_str); i++) 
			concat_str[k++] = fixed_str[i]; 

		concat_str[k] = '\0'; // string ends with '\0' 

		// Close both reading ends 
		close(fd1[0]); 
		close(fd2[0]); 

		// Write concatenated string and close writing end 
		write(fd2[1], concat_str, strlen(concat_str)+1); 
		close(fd2[1]); 

		exit(0); 
	} 
} 

```


#### 2.3 Sockets
*Socket* merupakan sebuah *end-point* dalam sebuah proses yang saling berkomunikasi. Biasanya *socket* digunakan untuk komunikasi antar proses pada komputer yang berbeda, namun dapat juga digunakan dalam komputer yang sama.

Diagram :   
![alt](img/socket.png "implementasi socket C")

Example : [socket-server.c](socket-server.c) [socket-client.c](socket-client.c)

Server
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}
```
Client
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
```
Jalankan proses server dulu, kemudian jalankan clientnya. Dan amati apa yang terjadi.

#### 2.4 Message Queues
Merupakan komunikasi antar proses dimana proses tersebut menciptakan internal linked-list pada alamat kernel Sistem Operasi. Pesannya disebut sebagai *queue* sedangkan pengenalnya disebut *queue* ID. *Queue* ID berguna sebagai *key* untuk menandai pesan mana yang akan dikirim dan tujuan pengiriman pesannya.

#### 2.5 Semaphores
Semaphore berbeda dengan jenis-jenis IPC yang lain. Pada pengaplikasiannya, semaphore merupakan sebuah counter yang digunakan untuk controlling resource oleh beberapa proses secara bersamaan.
- Jika suatu counter block memory memiliki nilai positif, semaphore dapat menggunakan resource untuk prosesnya, dan mengurangi nilai counter block dengan 1 untuk menandai bahwa suatu block memory tengah digunakan.
- Sebaliknya, jika semaphore bernilai 0, proses akan masuk pada mode sleep sampai semaphore bernilai lebih besar dari 0.

#### 2.6 Shared Memory
Sebuah mekanisme *mapping area(segments)* dari suatu blok *memory* untuk digunakan bersama oleh beberapa proses. Sebuah proses akan menciptakan *segment memory*, kemudian proses lain yang diijinkan dapat mengakses *memory* tersebut. *Shared memory* merupakan cara yang efektif untuk melakukan pertukaran data antar program.

Example: [Proses 1](proses1.c) [Proses 2](proses2.c)

Proses 1
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        *value = 10;

        printf("Program 1 : %d\n", *value);

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```
Proses 2
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        printf("Program 1 : %d\n", *value);
	*value = 30;

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```
Jalankan proses 1 terlebih dahulu, lalu proses 2. 
Hasilnya
Proses 1
```
Program 1 : 10
Program 1 : 30
```
Proses 2
```
Program 1 : 10
Program 1 : 30
```

### Appendix
#### Libraries documentation (and functions)
```
$ man {anything-you-want-to-know}
$ man mkfifo
$ man fcntl
```

### Soal Latihan 

##### Latihan 1
Buatlah sebuah program multithreading yang dapat menyalin isi file baca.txt ke dalam file salin1.txt. Kemudian menyalin isi dari file salin1.txt ke dalam file salin2.txt!

##### Latihan 2
Buatlah sebuah program multithreading yang dapat menampilkan bilangan prima dari 1-N. program akan dieksekusi menggunakan thread sebanyak T dimana setiap thread akan melakukan pencarian bilangan prima dengan range N/T (range tiap thread berbeda), kemudian tiap thread akan menampilkan hasilnya.

misalkan N = 100 dan T=2; jadi thread 1 akan mencari bilangan prima dari 1-50 dan thread 2 akan mencari dari 51-100

##### Latihan 3  
Buatlah sebuah program untuk menampilkan file diurutan ketiga dari sebuah direktori. Dengan ketentuan :  
- menggunakan pipe.
- menggunakan command ls, head, tail.
<!-- diganti soal pipe -->

#### References 
https://notes.shichao.io/apue/  
https://www.gta.ufrj.br/ensino/eel878/sockets/index.html  
http://advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf  
https://www.geeksforgeeks.org/socket-programming-cc/  
https://www.geeksforgeeks.org/pipe-system-call/  

### Jawaban Latihan 

#### Latihan 1
Buatlah sebuah program multithreading yang dapat menyalin isi file baca.txt ke dalam file salin1.txt. Kemudian menyalin isi dari file salin1.txt ke dalam file salin2.txt!

##### Jawaban Latihan 1
```javascript
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int status;

void* t1(void *ptr) // buat fungsi unt menyalin isi dari file baca ke salin1
{
        status = 0;

        FILE *baca, *salin1;
        baca=fopen("baca.txt", "r");
        salin1=fopen("salin1.txt", "w");
        char a;
        while((a=fgetc(baca))!=EOF)
        fputc(a,salin1);

        fclose(baca);
        fclose(salin1);

        status = 1;
        return NULL;
}

void* t2(void *ptr) // buat fungsi unt menyalin isi dari file salin1 ke salin2
{
        while(status != 1)
        {

        }

        FILE *salin1, *salin2;
        salin1=fopen("salin1.txt", "r");
        salin2=fopen("salin2.txt", "w");
        char a;
        while((a=fgetc(salin1))!=EOF)
        fputc(a,salin2);

        fclose(salin1);
        fclose(salin2);
}

int main()
{ // buat thread nya
        pthread_t tid1, tid2;
        int sesuatu=0;

//      (void) pthread_create(&tid2,NULL,t2,&sesuatu);
        (void) pthread_create(&tid1,NULL,t1,&sesuatu);
        (void) pthread_create(&tid2,NULL,t2,&sesuatu);
        printf("Thread created\n");

        (void) pthread_join(tid1,NULL);
        (void) pthread_join(tid2,NULL);
        return 0;
}
```
##### Penjelasan
* Disini kami membuat dua thread, thread yang pertama untuk menyalin isi dari file baca ke salin1, sedangkan thread yang kedua untuk menyalin isi dari file salin1 ke salin2.

* Awalnya kita deklarasikan terlebih dahulu variabel bernama status dengan type data integer, yang nantinya akan g=digunakan di while agar program berjalan dengan urutan nya, artinya program kedua tidak akan berjalan sebelum program satu selesai.

* Fungsi pertama yang kita buat yaitu 
```javascript
void* t1(void *ptr) // buat fungsi unt menyalin isi dari file baca ke salin1
{
        status = 0;

        FILE *baca, *salin1;
        baca=fopen("baca.txt", "r");
        salin1=fopen("salin1.txt", "w");
        char a;
        while((a=fgetc(baca))!=EOF)
        fputc(a,salin1);

        fclose(baca);
        fclose(salin1);

        status = 1;
        return NULL;
}
```
Berdasarkan program diatas, kita membuka sekaligus membaca dulu file bernama `baca` kemudian isi file baca itu dimasukkan ke variabel bernama baca. Kemuadian buat variabel slain1 yang akan me write file yang bernama salin1.txt. Deklarasikan variabel a dengan tipe data character untuk menampung isi dari baca kemudian dari variabel a itu di masukkan ke file salin1. Yang terakhir, file baca dan salin1 di tutup.

* Fungsi kedua yang kita buat yaitu 
```javascript
void* t2(void *ptr) // buat fungsi unt menyalin isi dari file salin1 ke salin2
{
        while(status != 1)
        {

        }

        FILE *salin1, *salin2;
        salin1=fopen("salin1.txt", "r");
        salin2=fopen("salin2.txt", "w");
        char a;
        while((a=fgetc(salin1))!=EOF)
        fputc(a,salin2);

        fclose(salin1);
        fclose(salin2);
}
```
Diawal fungsi kedua ada ```while status !=1``` itu termasuk mutex yang digunakan agar jalan nya program berurutan, sama halnya seperti wait. Disini kita membuka atau nge read file salin1.txt yang dimasukkan ke variabel salin1 kemudian membuat file salin2.txt yang dimasukkan ke dalam variabel salin2. kemudian buat lagi variabel a bertipe data characters dimana a ini berisi isi dari file salin1 yang dibaca sampai end of file kemdudian a di berikan ke file salin2. yang terakhir tutup file salin1 dan salin2.

#### Latihan 2
Buatlah sebuah program multithreading yang dapat menampilkan bilangan prima dari 1-N. program akan dieksekusi menggunakan thread sebanyak T dimana setiap thread akan melakukan pencarian bilangan prima dengan range N/T (range tiap thread berbeda), kemudian tiap thread akan menampilkan hasilnya.

misalkan N = 100 dan T=2; jadi thread 1 akan mencari bilangan prima dari 1-50 dan thread 2 akan mencari dari 51-100

##### Jawaban Latihan 2

#### Latihan 3
Buatlah sebuah program untuk menampilkan file diurutan ketiga dari sebuah direktori. Dengan ketentuan :  
- menggunakan pipe.
- menggunakan command ls, head, tail.
<!-- diganti soal pipe -->

##### Jawaban Latihan 3
```javascript
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
        pid_t pid1, pid2;
        int pipe1[2], pipe2[2];
        int wait1, wait2;
        pipe(pipe1);
        pipe(pipe2);

        if((pid1 = fork())== 0) // buka daftar file
        {
                close(pipe1[0]);
                dup2(pipe1[1], STDOUT_FILENO);//output ls disimpan di pipe 1 write, STDOUT=1
                char *argv[] = {"ls", NULL};
                execv("/bin/ls", argv);
        }

        else
        {
                while((wait(&wait1)) > 0);
                if((pid2 = fork())== 0) //diambil file 3 baris paling atas
                {
                        close(pipe1[1]);
                        dup2(pipe1[0], STDIN_FILENO);// output ls dijadikan input, STDIN=0
                        close(pipe1[0]);
                        dup2(pipe2[1], STDOUT_FILENO);//output head disimpan di pipe 2 write
                        char *argv[] = {"head", "-n 3", NULL};
                        execv("/usr/bin/head", argv);
                }
                else
                {
                        while((wait(&wait2)) >0); // dari 3 baris teratas tadi diambil 1 paling bawah
                        close(pipe2[1]);
                        dup2(pipe2[0], STDIN_FILENO);//output head jadi input
                        char *argv[] = {"tail", "-n 1", NULL};
                        execv("/usr/bin/tail", argv);
                }
        }
        return 0;
}
```

##### Penjelasan
* Kita disini menggunakan 2 pipe yang terdiri dari 1 dan 0, 1 untuk output sedangkan 0 untuk input. kami disini juga menggunakan wait agar program berjalan secara berurutan, artinya program kedua akan berjalan jika program pertama sudah selesai.

```javacsript
if((pid1 = fork())== 0) // buka daftar file
        {
                close(pipe1[0]);
                dup2(pipe1[1], STDOUT_FILENO);//output ls disimpan di pipe 1 write, STDOUT=1
                char *argv[] = {"ls", NULL};
                execv("/bin/ls", argv);
```
* Source code diatas digunakan untuk membuka daftar file dengan perintah `ls` kemudian output dari ls tersebut disimpan di pipe yang pertama yang 1 untuk menyimpan outputnya dan akan digunakan sebagai inputan di proses berikutnya.

```javascript
else
        {
                while((wait(&wait1)) > 0);
                if((pid2 = fork())== 0) //diambil file 3 baris paling atas
                {
                        close(pipe1[1]);
                        dup2(pipe1[0], STDIN_FILENO);// output ls dijadikan input, STDIN=0
                        close(pipe1[0]);
                        dup2(pipe2[1], STDOUT_FILENO);//output head disimpan di pipe 2 write
                        char *argv[] = {"head", "-n 3", NULL};
                        execv("/usr/bin/head", argv);
                }
```
* Menggunakan wait agar proses kedua bisa berjalan jika proses pertama sudah selesai. Kemudian pada proses ini kita mengambil input dari output proses sebelumnya melalui pipe pertama yang 0. Kemudian agar dari daftar file tersebut yang kita ambil bisa hanya 3 saja yang diambil yaitu dengan menggunakan `head -n 3` kemudian output head disimpan di pipe dua yang write(1) agar bisa digunakan untuk input proses selanjutnya.

```javascript
else
                {
                        while((wait(&wait2)) >0); // dari 3 baris teratas tadi diambil 1 paling bawah
                        close(pipe2[1]);
                        dup2(pipe2[0], STDIN_FILENO);//output head jadi input
                        char *argv[] = {"tail", "-n 1", NULL};
                        execv("/usr/bin/tail", argv);
                }
```
* Menggunakan wait agar proses ketiga bisa berjalan jika proses kedua sudah selesai. Kemudian pada soal kan kita diminta untuk menampilkan file yang ada di urutan ketiga, itu berarti kita hanya mengambil 1 file saja diurutan terakhir berdasarkan output dari proses dua tadi, berarti kita mengambil file yang paling bawah yaitu dengan `tail -n 1`.

## SoalShift_modul3_F01

### Soal 1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
    
    ./faktorial 5 3 4
    
    3! = 6
    
    4! = 24
    
    5! = 120

#### Jawaban
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

#### Penjelasan
Di line 41 terdapat parameter yang digunakan untuk menampung argumen. Line 42-43 untuk mengubah argumen ke dalam bentuk tipe data int. Line 44 digunakan untuk menyortir argumen dari kecil ke yang terbesar melalui fungsi sort yang terdapat pada line 27-29. Di line 46 terdapat looping yang digunakan untuk mengakses fungsi fakt untuk perhitungan faktorial dan terdapat thread join yang digunakan agar proses berjalan bergantian.

### Soal 2
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

#### Jawaban 


#### Penjelasan

### Soal 3
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

#### Jawaban
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

#### Penjelasan
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
### Soal 4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 

* Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama

* Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama

* Ketika Mengekstrak file .zip juga harus secara bersama-sama

* Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”

* Wajib Menggunakan Multithreading

* Boleh menggunakan system

#### JAwaban
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

#### Penjelasan

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

### Soal 5
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

#### Jawaban

#### Penjelasan
Jawaban masih belum rampung, masih dalam pengerjaan. Program saya masih tampilan dan mampu mendeteksi input berupa key press dan menambahkan nama monster
