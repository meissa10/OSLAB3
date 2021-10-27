#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include  <time.h>

void  deposit(int *x);
void  request(int *x);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int *bankAccount;
		 int *turn;
	
   

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of 2 integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0;
     ShmPTR[1] = 0;
	

     bankAccount = &ShmPTR[0];
	   turn = &ShmPTR[1];
	
     pid = fork();
	
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
       int i = 0;
			 
			 while(i<25){
				 int sleepTime = (rand()%5);
			 
				 sleep(sleepTime);
				 
				 while(*turn ==1);
				 
				 request(bankAccount);
				 *turn =1;
				 i++;
			 }
       exit(0);
     }
	
	
		 else{
			 
			 
			 int i = 0;
			 
			 while(i<25){
				 int sleepTime = (rand()%5);
			 
				 sleep(sleepTime);
				 
				 while(*turn ==0);
				 
				 if (*bankAccount <= 100){
					deposit(bankAccount);
					}
				 
				 else {
					printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", *bankAccount);
					}
				 
				 *turn =0;
				 i++;
			 }
			 
			 
			 
			
		 }

     wait(&pid);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  deposit(int *x){
	int depositAmount = (rand() % 100);
	
	if ((depositAmount%2) ==0){
		*x+=depositAmount;
		printf("Dear old Dad: Deposits $%d / Balance = $%d\n", depositAmount, *x);
	}
	
	else{
		printf("Dear old Dad: Doesn't have any money to give\n");
	}
	
}
void  request(int *x){	
	int requestAmount = (rand() % 50);
	printf("Poor Student needs $%d\n", requestAmount);
	if (requestAmount <= *x) {
		*x -= requestAmount;
		printf("Poor Student: Withdraws $%d / Balance = $%d\n", requestAmount, *x);
	} 
	else {
		printf("Poor Student: Not Enough Cash ($%d)\n", *x);
	}
}