#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

char* cd (char* direction) {
  DIR* dir = opendir(trimwhitespace(direction));
  static char s[MAX_CMD_LEN];
  if (dir > 0) {
    chdir(trimwhitespace(direction));
    getcwd(s, MAX_CMD_LEN);
    return s;
  }
  else {
    printf("Command error: No such file or directory\n");
    getcwd(s, MAX_CMD_LEN);
    return s;
  }
}

void redirect(int filedes, char * name_file){// for >
  close(filedes);//close the original output
  int new = open(name_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR | S_IROTH | S_IWOTH); // create a new fd with writeonly permission
  if (open < 0)// if it fails
  {
      fprintf(stderr, "error: %d \n %s\n", errno, strerror(errno));
      exit(1);
  }
  dup2(new,filedes);//replace the original
}

void redirect_append(int filedes, char *name_file){//for >>
  close(filedes);
  int new = open(name_file, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );//create a new fd with append and wronly permission
  if (open < 0) {
        fprintf(stderr, "error: %d \n %s\n", errno, strerror(errno));
        exit(1);
  }
    dup2(new,filedes);
}

int is_redirection (char *str) {
  if(strcmp(str,">") == 0)
    return 0;
  else if (strcmp(str,">>") == 0)
    return 1;
  else
    return 2;
}
int first_index_redirection(int p1,char **str){
  int j = 0;
  for(int i =0; i< p1; i++){
    if(strcmp(str[i],">") == 0 || strcmp(str[i], ">>") == 0){
      return j;
    }
    j++;
  }
  return 0;
}

char * make_command(char *str){
  int x = get_command_type(str);
    switch(x){
      case 0:
        return "./cd";
        break;
      case 1:
        return "./du";
        break;
      case 2:
        return "./grep";
        break;
      case 3:
        return "./exit";
        break;
      case 4:
        return str;
        break;
    }
}

int main () {

  // Type your code here
  int i =0;
  int p1,p2,x,l,r,y;
  char front[] = "[4061-shell]";
  char mycwd[MAX_CMD_LEN];
  if(getcwd(mycwd, MAX_CMD_LEN) == NULL){
   perror("Failed to get current working directory\n");
  }
  // get formmating part.
  char buffer[MAX_CMD_LEN];
    while(1) {
      printf("%s %s %c ",front,mycwd,'$');
      char **adv_buffer = (char **) malloc(sizeof(char *) * MAX_TOKENS);
      char **str_ptr;
      char **str_ptr2;
      fgets(buffer,MAX_CMD_LEN,stdin);
      int i = parse_line(buffer,adv_buffer, "|");
    /* ==================================================
    *  ====================NO=PIPE=======================
    * =================================================== */
      if (i == 1) {
        str_ptr = (char **) malloc(sizeof(char*) * MAX_TOKENS);
        p1 = parse_line(adv_buffer[0],str_ptr," ");
        char *cmd = make_command( trimwhitespace(str_ptr[0]));

        if (get_command_type( trimwhitespace (str_ptr[0])) == 0) { // CD
          strncpy(mycwd, cd(str_ptr[1]), MAX_CMD_LEN);
        }
        else if (get_command_type(str_ptr[0]) == 3){
          printf("Exit Shell\n");
          free(adv_buffer);
          free(str_ptr);
          exit(1);
        }
        else {
          l = 3;
          for(int m = 0; m < p1; m++){
            str_ptr[m] = trimwhitespace(str_ptr[m]);
            if(l != 0 && l != 1){
              l = is_redirection(str_ptr[m]);
            }
          }
          int first_index = first_index_redirection(p1,str_ptr);

          pid_t pid = fork();
          if(pid == 0){
            char* args[p1];
            args[0] = cmd;
            int j = 1;
            if(x == 4){
              if (l != 2){
                for(int i = 1 ; i < first_index; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char* )NULL;
              }
              else{
                for(int i = 1; i < p1; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char *) NULL;
              }
            }     // if ERROR not cd, du, so on..
            else{
              if (l != 2){
                for(int i = 1 ; i < first_index; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char* )NULL;
              }
              else{
                for(int i = 1; i < p1; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char *) NULL;
              }
            }
            if(l != 2){      // Ridirection
              if(l == 0) redirect(1,str_ptr[first_index + 1]);
              else if( l == 1) redirect_append(1,str_ptr[first_index + 1]);
            }

            if (strlen(args[0]) != 0) {
              execvp(args[0],args);
              perror("Command error : Cannot Execute in Simple function");
            }

          }
          else if (pid >0){
            wait(NULL);
            free(str_ptr);
            free(adv_buffer);
          }
        }
      }
    /* ==================================================
    *  =====================PIPE=========================
    * =================================================== */
      else {
        str_ptr = (char **) malloc(sizeof(char*) * MAX_TOKENS);
        str_ptr2 = (char **) malloc(sizeof(char*) * MAX_TOKENS);
        int fds[2];
        pipe(fds);          // pipe
        pid_t pid, pid2;

        p1 = parse_line(adv_buffer[0],str_ptr," ");
        p2 = parse_line(adv_buffer[1],str_ptr2," ");

        char *cmd = make_command(str_ptr[0]);
        x = get_command_type(str_ptr[0]);
        char *cmd2 = make_command(str_ptr2[0]);
        y = get_command_type(str_ptr2[0]);
          l = 3;      // to track the redirection
          for (int m = 0; m < p1; m++){
            str_ptr[m] = trimwhitespace(str_ptr[m]);
            if(l != 0 && l != 1) l = is_redirection(str_ptr[m]);
          }
          int first_index = first_index_redirection(p1,str_ptr);
          int second_index = first_index_redirection(p2,str_ptr2);
          pid = fork();

          if (pid < 0) {
            perror("Error: in fork\n");
            exit(1);
          }
          else if (pid == 0) {  // CHILD
            char* args[p1];
            args[0] = cmd;
            int j = 1;
            if (x == 4) {                                         // ERROR
              if (l != 2){
                for(int i = 1 ; i < first_index; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char* )NULL;
              } else {
                for(int i = 1; i < p1; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char *) NULL;
              }
            }
            else{
              if (l != 2){
                for(int i = 1 ; i < first_index; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char* )NULL;
              } else {
                for(int i = 1; i < p1; i++){
                  args[j] = str_ptr[i];
                  j++;
                }
                args[j] = (char *) NULL;
              }
            }
            if(get_command_type(str_ptr[0]) == 0){
              break;
            }
            else if (get_command_type(str_ptr[0]) == 3){
              break;
            }
            else{
              if(l != 2 ){                    // Redirection
                // > first then |
                if(l == 0){
                  redirect(1,str_ptr[first_index + 1]);
                  close(fds[0]);
                  close(fds[1]);
                  execvp(args[0], args);
                  perror("Command error : Cannot Execute in Child");
                }
                // >> first then |
                else if( l == 1){
                  redirect_append(1,str_ptr[first_index + 1]);
                  close(fds[0]);
                  close(fds[1]);
                  execvp(args[0], args);
                  perror("Command error : Cannot Execute in Child");
                }
              } else {
                close(fds[0]);
                dup2(fds[1], 1);
                close(fds[1]);
                if (strlen(args[0]) != 0) {
                  execvp(args[0], args);
                  perror("Command error : Cannot Execute in Child");
                }
              }
            }  // Child Process
          }
          else if (pid >0){            // PARENT
            pid2 = fork();
            if (pid2 < 0) {
              perror("Error: in fork");
              exit(1);
            }
            // Child
            else if (pid2 == 0) {
              char* args2[p2];
                r = 3;
                for(int m = 0; m < p2; m++){
                  str_ptr2[m] = trimwhitespace(str_ptr2[m]);
                  if(r != 0 && r != 1){
                    r = is_redirection(str_ptr2[m]);
                  }
                }
                args2[0] = cmd2;
                int j =1;
                if (y == 4) {       // after pipe ERROR
                  if (r != 2){
                    for(int i = 1 ; i < second_index; i++){
                      args2[j] = str_ptr2[i];
                      j++;
                    }
                    args2[j] = (char* )NULL;
                  }
                  else{
                    for(int i = 1; i < p2; i++){
                      args2[j] = str_ptr2[i];
                      j++;
                    }
                    args2[j] = (char *) NULL;
                  }
                }

                else{
                  if (r != 2){
                    for(int i = 1 ; i < second_index; i++){
                      args2[j] = str_ptr2[i];
                      j++;
                    }
                    args2[j] = (char* )NULL;
                  } else {
                    for(int i = 1; i < p2; i++){
                      args2[j] = str_ptr2[i];
                      j++;
                    }
                    args2[j] = (char *)NULL;
                  }
                }
                if(get_command_type(str_ptr2[0]) == 0){
                  break;
                }
                else if (get_command_type(str_ptr2[0]) == 3){
                  printf("Exit Shell\n");
                  free(adv_buffer);
                  free(str_ptr);
                  free(str_ptr2);
                  break;
                }
                else {
                  if(r != 2 ){     // redirection
                    if(r == 0) redirect(1,str_ptr2[second_index + 1]);
                    else if( r == 1) redirect_append(1,str_ptr2[second_index + 1]);
                  }
                  close(fds[1]);
                  dup2(fds[0], 0);
                  close(fds[0]);
                  if (strlen(args2[0]) != 0) {
                    execvp(args2[0], args2);
                    printf("Command Error: Cannot Execute \n");
                    break;
                  }
                }
            }
            else {   // parent
              close(fds[0]);
              close(fds[1]);
              wait(NULL);
              wait(NULL);
            }
          } // PARENT
        free(adv_buffer);
        free(str_ptr);
        free(str_ptr2);
      } // PIPE
      l = 3;
      r = 3;
    }  // WHILE
}   // MAIN
