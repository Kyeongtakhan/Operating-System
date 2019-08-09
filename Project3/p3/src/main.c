//main.c program to translate the virtual addresses from the virtual.txt
//into the physical frame address. Project 3 - CSci 4061

#include <stdio.h>
#include <stdlib.h>
#include "vmemory.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INPUT_FILE "../bin/virtual.txt"

int main(int argc, char* argv[])
{
	if (argc > 2) {
		printf("Too many arguments, enter up to one argument\n");
		exit(-1);
	}
	int count = 0;
	int p_frame, v_frame;
	int policy = (argc == 2) ? 1:0;
	initialize_vmanager(policy);

	//TODO: Fill the algorithm by reading from INPUT_FILE
	struct stat fstat;
	if (stat(INPUT_FILE,&fstat) == -1){
		perror("Failed to get file status\n");
		return 1;
	}
	int fsize = fstat.st_size;
	char buffer[fsize];
	int fd = open(INPUT_FILE,O_RDONLY);
	if(fd == -1){
		perror("Failed to open file. It does not exist\n");
		return 1;
	}
	while(read(fd,buffer,sizeof(buffer)));
	if(close(fd) == -1){
		perror("Failed to close the file\n");
		return 1;
	}
	FILE *f = fopen(INPUT_FILE, "r");
	while(fgets(buffer,sizeof(buffer),f)){
		char *ptr;
		unsigned long v_addrs = strtoul(buffer,&ptr,16);
		//printf("%d vaddress is %1lx   and ",count,v_addrs);
		p_frame = fetch_physical_frame(v_addrs);
		v_frame = v_addrs >> 12;
		int off = get_offset(v_addrs);
		// printf("count %d is physical address is %1x\n",count, p_frame);
		print_physical_address(p_frame,off);
		populate_tlb(v_frame, p_frame);

		if (p_frame == -1) {
			count ++;
		}
	}
  printf("Page Fault : %d times!\n", count);

	// start part2
	get_hit_ratio();
	print_tlb();
	//Free the page table
	free_resources();
	return 0;
}
