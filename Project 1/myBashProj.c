#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int is_parallel(int size, char** argv){//Checks to see if parallel
    int i;
    for(i =1; i < size; i++){
        if(strcmp(argv[i], "-p") ==0)//returns 1 if parallel
            return 1;
        else if (strcmp(argv[i], "-P") ==0)//0 if sequential
          return 1;
    }
    return 0;
}

int number_of_executions (int size, char** argv){//Counts number of processes to be executed i.e. counts '&'
    int count = 0;
    for(int i =1; i < size; i++){
        if(strcmp(argv[i], "&") ==0)
            count++;
    }
    return count;
}

int* makeindex(int start_index,int size,char** argv){//Makes an array of the start and end indices of the process
  static int arr[2];
  // index point to "&" start index automately skip the "&"
  if(strcmp(argv[start_index], "&") == 0){
    start_index = start_index+1;
  }
  int dest_index = start_index;     // set final index equal to start index.

  // stop at final index or '&'
  while(dest_index < size && (strcmp(argv[dest_index],"&") != 0)){
    if(strcmp(argv[dest_index],"-p") == 0){
      start_index = start_index + 1;
      dest_index = start_index;
    }
    else
        dest_index++;
  }
  arr[0] = start_index;
  arr[1] = dest_index -1;
  return arr;
}

int main(int argc, char** argv){
  int i,j;
  int s_index;//start index
  if (argc <= 1) {
    printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
    exit(1);
  }
  int num_of_exec = number_of_executions(argc, argv);
  // int array for the process indexes.
  int* possi_indexes;
  s_index = 1;

  if(is_parallel(argc,argv) == 0){// sequential
    for(i=0; i < num_of_exec + 1; i++){
        possi_indexes = makeindex(s_index,argc,argv);
        s_index = possi_indexes[1] + 1;
        char* args[(possi_indexes[1] - possi_indexes[0]) + 1]; // make constant char array to take the neccesary command.
        j =0;
        pid_t pid = fork();
        if (pid == -1)
          perror("Fork failed\n");
        if(pid == 0){//child
          for(i = possi_indexes[0]; i <= possi_indexes[1]; i++){    // Put the correspond command into our array
            args[j] = argv[i];
            j++;  // keep track of the end of the array to put the NULL
          }
          args[j] = (char*) NULL;
          s_index = possi_indexes[1]; // renew the start_index to the final index that we've used
          execv(args[0],args);
        }
        else// parent
          wait(NULL);
    }
  }

  else { // parallel
    int pids[num_of_exec + 1];
    for(i = 0; i < num_of_exec + 2; i++){
      pids[i] = fork();
      if(pids[i] > 0) {
        if (i != num_of_exec + 1) {
          possi_indexes = makeindex(s_index,argc,argv);
          s_index = possi_indexes[1] + 1;
        }
      }
      else if(pids[i] == 0 ){
        char* args[(possi_indexes[1] - possi_indexes[0]) + 1];
        j = 0;
        for(i = possi_indexes[0]; i <= possi_indexes[1]; i++){    // Put the correspond command into our array
          args[j] = argv[i];
          j++;  // keep track of the end of the array to put the NULL
        }
        args[j] = (char*) NULL;
        s_index = possi_indexes[1]; // renew the start_index to the final index that we've used
        execv(args[0],args);
      }
    }
    for(i = 0; i < num_of_exec + 2; i++)
      wait(&pids[i]);
  }
}
