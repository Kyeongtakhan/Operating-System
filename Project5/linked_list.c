#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// global variables
int seq_no_count = 0;
pthread_mutex_t mutex;

// read_line
char* read_line(char* fname, int num) {
    FILE* file = fopen(fname, "r");
    if(file == NULL){
      perror("Failed to open file");
    }
    // since we know that there is a file, continue.
    char *line = (char*) malloc (sizeof(char) * LINE_SIZE);
    int i = 0;
    while (fgets(line, LINE_SIZE, file)) {
      if (i == num) {
        fclose(file);
        return line;
      }
      i ++;
    }
    int fc = fclose(file);
    if(fc < 0){
      perror("failed to close file");
    }
    return line;
}


//Traverse the linked list with the head as the argument
void traversal(node * n) {
  FILE *fptr = fopen("copy.txt", "w");
  if(fptr == NULL){
    perror ("Failed to open file");
  }
  while (n != NULL) {
    fprintf(fptr, "%d, %d, %s", n->seq_no, n->line_no, n->content);
    n = n->next;
  }
  int fc = fclose(fptr);
  if (fc < 0){
    perror("Failed to close file");
  }
}

//Insert a node created using create_node into the linked list
//param 1: head address, param 2: node
void insert(node ** head_address, node * n) {
  node* current;
  pthread_mutex_lock(&mutex);
  if ((*head_address) == NULL) {
    n->seq_no = seq_no_count;
    seq_no_count ++;
    *head_address = n;
    pthread_mutex_unlock(&mutex);
  }
  else {
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&(head->ll_lock));
    n->seq_no = seq_no_count;
    seq_no_count ++;
    if ((*head_address)->line_no > n->line_no) {
      n->next = *head_address;
      *head_address = n;
    } else {
    current = *head_address;
    while (current->next != NULL &&
           current->next->line_no < n->line_no) {
        current = current->next;
    }
    n->next = current->next;
    current->next = n;
    }
  pthread_mutex_unlock(&(head->ll_lock));
  }
}


struct linked_node* create_node (int num, char * line_content){
	struct linked_node* new = (struct linked_node*) malloc (sizeof (struct linked_node));
  pthread_mutex_init(&(new->ll_lock),NULL);
  new->next = NULL;
	new->line_no = num;
	new->content = line_content;
  return new;
}


void* tasks_collection (void* arg){
  pthread_mutex_init(&mutex, NULL);
  struct fname_line_no* file_line = arg;
  char* line = read_line(file_line->fname,file_line->real_line_no);
  struct linked_node* new = create_node(file_line->real_line_no, line);
  insert(&head, new);

}
