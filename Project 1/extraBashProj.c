#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int number_of_executions (int size, char** argv){//Counts number of '&' or'&&' 
    int count = 0;
    for(int i =1; i < size; i++){
        if(strcmp(argv[i], "&") == 0 || strcmp(argv[i], "&&") == 0)
            count++;
    }
    return count;
}

int* symbolChecker (int size, char** argv) {// Checks number of '&' and '&&' 
  int new_size = number_of_executions(size, argv);
  int * arr = malloc (sizeof(int)*(new_size+1));
  int count = 0;
  for (int i = 1; i < size; i ++) {
    if(strcmp(argv[i], "&") == 0 || strcmp(argv[i], "&&") == 0) {
      arr[count] = i;
      count ++;
    }
  }
  return arr;
}


int* makeindex(int start_index,int size,char** argv){//makes an array of the start and end indexes of the process
  static int arr[2];
  // index point to "&" start index automately skip the "&"
  if(strcmp(argv[start_index], "&") == 0 || strcmp(argv[start_index], "&&") == 0){
    start_index = start_index+1;
  }
  int dest_index = start_index;     // set final index equal to start index.
  // whenever it find the & sign it will stop or if final index greater than
  // argv size, it indicate end of command
  while(dest_index < size && (strcmp(argv[dest_index],"&") != 0) &&
                    strcmp(argv[dest_index], "&&")){
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
  int j = 0;
  int s_index; // change of starting index
  int wait_count = 0; // count how many time do I need to wait.
  int * temp_symbol = symbolChecker(argc,argv);
  int symbol_tracker = 0;

  if (argc <= 1) {
    printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
    exit(1);
  }
  int num_of_exec = number_of_executions(argc, argv);
  int* possi_indexes;
  s_index = 1;
  int pids[num_of_exec +1];
  for (int i = 0; i < num_of_exec + 1; i++) {
    possi_indexes = makeindex(s_index, argc, argv);
    s_index = possi_indexes[1] + 1;
    pids[i] = fork();
    if (pids[i] == -1) {
      perror("fork failed\n");
    }
    else if (pids[i] == 0) {
      char* args[(possi_indexes[1] - possi_indexes[0]) + 1]; // make constant char array to take the neccesary command.
      for(i = possi_indexes[0]; i <= possi_indexes[1]; i++){    // Put the correspond command into our array
        args[j] = argv[i];
        j++;  // keep track of the end of the array to put the NULL
      }
      args[j] = (char*) NULL;
      s_index = possi_indexes[1]; // renew the start_index to the final index that we've used
      execv(args[0], args);
    }
    // when is father's node.
    else {
      // when it is parallel
      if (strcmp(argv[temp_symbol[symbol_tracker]], "&") == 0) {
        wait_count += 1;
      }
      // when it is sequential
      else {
        wait(&pids[i]); //wait for correspond child
      }
      symbol_tracker++;
    }
  }
  // This will wait it for all process child finished.
  for (int k = 0; k < wait_count + 1; k ++) {
    wait(NULL);
  }
  free(temp_symbol);
}
