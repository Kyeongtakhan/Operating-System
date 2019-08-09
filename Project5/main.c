#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// count how many lines
int number_of_lines(char* fname) {
  FILE* f;
  if ((f = fopen(fname, "r")) == NULL) {
    printf("Error: File does not exists\n");
    exit(1);
  }
  char *line = (char*) malloc (sizeof(char) * LINE_SIZE);
  int i = 0;
  while (fgets(line, LINE_SIZE, f)) {
    i ++;
  }
  return i;
}


int main (int argc, char** argv){
  // Error checking for argument amount
  if (argc != 3) {
    printf("Error: Wrong Number of Input (needs three inputs)\n");
    printf("       file name // number of threads\n");
    exit(1);
  }

  char* file_name = argv[1];
  int num_thread = atoi(argv[2]);

  // Error checking for thread amount
  if (num_thread > 16) {
    printf("Error: Too many threads (thread should be less than 16)\n");
    exit(1);
  }
  if (num_thread < 1) {
    printf("Error: No threads (thread should be bigger than 0)\n");
    exit(1);
  }


  pthread_t t[num_thread];
  int i = 0;  // track thread
  int line_no_count = 0;
  int num_of_lines = number_of_lines(file_name);

  // if there are more threads than number of lines
  if (num_thread > num_of_lines) {
    num_thread = num_of_lines;
  }

  // thread create
  while (line_no_count != num_of_lines){
    if (i == num_thread) {
      i = 0;
    }
    fl* file_liner = (fl*) malloc (sizeof(fl));
    file_liner->fname = file_name;
    file_liner->real_line_no = line_no_count;
    file_liner->max_line = num_of_lines;

    int t_error = pthread_create(&t[i],NULL,tasks_collection,(void*) file_liner);
    if (t_error < 0 ){
      perror ("Faile to create thread");
    }
    line_no_count++;
    i++;
  }

  // thread join
  for(int i =0; i < num_thread; i++){
    int j_error = pthread_join(t[i],NULL);
    if(j_error < 0){
      perror("Failed to join thread");
    }
  }

  // traversal
  traversal(head);

  int d_error = pthread_mutex_destroy(&(head->ll_lock));
  if(d_error < 0){
    perror("Failed to destroy thread");
  }
  return 0;
}
