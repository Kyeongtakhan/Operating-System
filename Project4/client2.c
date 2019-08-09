#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>

typedef struct mesg_buffer {
	long mesg_type;
	int flags;
	char mesg_text[1024];
} mymsg;

struct shm{
	key_t keysh;
};

int main()
{
	int msgid;
	key_t key;
	mymsg *msg1 = (mymsg*) malloc(sizeof(mymsg));
	mymsg *msg2 = (mymsg*) malloc(sizeof(mymsg));
	int shmid;
	struct shm *sh;
	while(1){
		shmid = shmget(200,sizeof(struct shm),0666|IPC_CREAT);
		if(shmid < 0){
			exit(1);
		}
		sh = (struct shm*) shmat(shmid,(void*)0,0);
		key = sh->keysh;
		msgid = msgget(key, IPC_CREAT | 0666);
		if(msgid < 0){
			exit(1);
		}
		if(msgrcv(msgid,msg1,sizeof(mymsg),3,0) < 0){
			exit(1);
		}
		if(msg1->flags < 2){
			printf("Message Received : %s\n", msg1->mesg_text);
			if(msg1->flags != 0){
				msg2->mesg_type = 4;
				msg2->flags = 1;
				printf("Write Response : ");
				fgets(msg2->mesg_text,1024,stdin);
				if(msgsnd(msgid, msg2, sizeof(mymsg), 0) < 0){
					exit(1);
				}
			}
		}
		else{
			printf("Server closed connection\n");
			exit(1);
		}
		if(msgctl(msgid, IPC_RMID,NULL) < 0){
			exit(1);
		}
	}
	if(shmdt(sh) < 0){
		exit(1);
	}
	if(shmctl(shmid,IPC_RMID,NULL) < 0){
		exit(1);
	}
	free(msg1);
	free(msg2);
  return 0;
}
