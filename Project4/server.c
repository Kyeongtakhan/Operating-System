// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

typedef struct mesg_buffer {
	long mesg_type;
	int flags;
	char mesg_text[1024];
} mymsg;

struct shm{
	key_t keysh;
};

void print_message() {
  printf("Enter 1 to choose client 1\n");
  printf("      2 to choose client 2\n");
  printf("      3 to broadcast a message\n");
  printf("      0 to exit\n\n");
  printf("Enter Choice: ");
}


void firstClient() {
	int msgid;
	key_t key = 777;
	mymsg *msg1 = (mymsg*) malloc(sizeof(mymsg));
	mymsg *msg2 = (mymsg*) malloc(sizeof(mymsg));

	int shmid = shmget (100,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid < 0){
		exit(1);
	}
	struct shm *sh = (struct shm*) shmat(shmid,0,0);
	sh->keysh = key;
	if(shmdt(sh) < 0){
		exit(1);
	}

	msgid = msgget(key, IPC_CREAT|0666);
	if(msgid < 0){
		exit(1);
	}
	if(msgid < 0 ){
		exit(1);
	}
	msg1->mesg_type = 1;
	msg1->flags = 1;
	printf("Write message : ");
	//----Server asks the user to enter message---
	scanf("%*c%[^\n]",msg1->mesg_text);
	if (msgsnd(msgid, msg1, sizeof(mymsg),0) < 0){
		exit(1);
	}
	printf("\n");
  // receiving back the message
  if (msgrcv(msgid, msg2, sizeof(mymsg), 2, 0) <0){
		exit(1);
	}
  printf("Message Received : %s", msg2->mesg_text);
	free(msg1);
	free(msg2);
}

void secondClient() {
	int msgid;
	key_t key = 888;
	mymsg *msg1 = (mymsg*) malloc(sizeof(mymsg));
	mymsg *msg2 = (mymsg*) malloc(sizeof(mymsg));
	int shmid = shmget (200,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid < 0){
		exit(1);
	}
	struct shm *sh = (struct shm*) shmat(shmid,0,0);
	sh->keysh = key;
	if(shmdt(sh) < 0){
		exit(1);
	}
	msgid = msgget(key, IPC_CREAT|0666);
	if(msgid < 0){
		exit(1);
	}
	msg1->mesg_type = 3;
	msg1->flags = 1;
	printf("Write message : ");
	scanf("%*c%[^\n]",msg1->mesg_text);
	if(msgsnd(msgid, msg1, sizeof(mymsg),0) < 0){
		exit(1);
	}
	printf("\n");
  // receiving back the message
  if(msgrcv(msgid, msg2, sizeof(mymsg), 4, 0) < 0){
		exit(1);
	}
  printf("Message Received : %s", msg2->mesg_text);
	free(msg1);
	free(msg2);
}

void broadcasting() {
	int msgid, msgid2;
	key_t key1 = 777;
	key_t key2 = 888;
	mymsg *msg1 = (mymsg*) malloc(sizeof(mymsg));
	mymsg *msg2 = (mymsg*) malloc(sizeof(mymsg));
	int shmid = shmget (100,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid < 0){
		exit(1);
	}
	int shmid2 = shmget (200,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid2 < 0){
		exit(1);
	}
	struct shm *sh = (struct shm*) shmat(shmid,0,0);
	struct shm *sh1 = (struct shm*) shmat(shmid2,0,0);
	sh->keysh = key1;
	sh1->keysh = key2;
	if(shmdt(sh) < 0){
		exit(1);
	}
	if(shmdt(sh1) < 0){
		exit(1);
	}
	msgid = msgget(key1, IPC_CREAT|0666);
	if(msgid < 0){
		exit(1);
	}
	msgid2 = msgget(key2, IPC_CREAT|0666);
	if(msgid2 < 0){
		exit(1);
	}
	msg1->mesg_type = 1;
	msg1->flags = 0;
	msg2->mesg_type = 3;
	msg2->flags = 0;
	printf("Write message to broadcast: ");
	scanf("%*c%[^\n]",msg1->mesg_text);
	printf("%s",msg1->mesg_text );
	strcpy(msg2->mesg_text,msg1->mesg_text);
	if(msgsnd(msgid2,msg2,sizeof(mymsg),0) < 0){
		exit(1);
	}
	if(msgsnd(msgid, msg1, sizeof(mymsg),0) < 0){
		exit(1);
	}
	printf("\n");
	free(msg1);
	free(msg2);
}

void p_exit(){
	int msgid, msgid2;
	key_t key1 = 777;
	key_t key2 = 888;
	mymsg *msg1 = (mymsg*) malloc(sizeof(mymsg));
	mymsg *msg2 = (mymsg*) malloc(sizeof(mymsg));
	int shmid = shmget (100,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid < 0){
		exit(1);
	}
	int shmid2 = shmget (200,sizeof(struct shm),0666|IPC_CREAT);
	if(shmid2 < 0){
		exit(1);
	}
	struct shm *sh = (struct shm*) shmat(shmid,0,0);
	struct shm *sh1 = (struct shm*) shmat(shmid2,0,0);
	sh->keysh = key1;
	sh1->keysh = key2;
	if(shmdt(sh) < 0){
		exit(1);
	}
	if(shmdt(sh1) < 0){
		exit(1);
	}
	msgid = msgget(key1, IPC_CREAT|0666);
	if(msgid < 0){
		exit(1);
	}
	msgid2 = msgget(key2, IPC_CREAT|0666);
	if(msgid2 < 0){
		exit(1);
	}
	msg1->mesg_type = 1;
	msg1->flags = 44;
	msg2->mesg_type = 3;
	msg2->flags = 44;
	if(msgsnd(msgid2,msg2,sizeof(mymsg),0) < 0){
		exit(1);
	}
	if(msgsnd(msgid, msg1, sizeof(mymsg),0) < 0){
		exit(1);
	}
	free(msg1);
	free(msg2);
}

int main() {
  while(1){
		printf("\n");
		print_message();
    int response;
		scanf("%d",&response);
    if(response == 0) {
      printf("Exiting the server\n");
			p_exit();
      exit(1);
    } else if (response == 1) {
      firstClient();
    } else if (response == 2) {
      secondClient();
    } else if (response == 3) {
			broadcasting();
    } else {
      printf("Entered KEY! %d", response);
      printf("Response Error\n");
    }
  }
}
