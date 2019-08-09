//Implement the API, modeling the translation of virtual page address to a
//physical frame address. We assume a 32 bit virtual memory and physical memory.
//Access to the page table is only via the CR3 register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vmemory.h"
#include <stdint.h>

#define OUT_TLB "../bin/tlb_out.txt"

bool FIFO_policy = true;
int **cr3;
int sizeOfTLB = 8;
int tlb[8][2];        // this is the initial TLB
int tlb_lru[10][2];
int hist[10];
int h=0;

//
// More static functions can be implemented here
//
unsigned long split_offset(unsigned long* buff){
  static unsigned long pages[5];
  unsigned long mask_v_page = 0x0000f;
  unsigned long temp = 0;
  int i;
  for (i = 3; i < 8; ++i){
    temp = temp | (buff[i] << 4 * i);
  }
  for(i = 5; i< 8; ++i){
    temp = temp >> 4;
  }
  return temp;
}

int get_offset(unsigned long v_addr){
  int i;
  unsigned long result = 0;
  unsigned long mask1 = 0x0000000f;
  unsigned long bytes[8];
  unsigned long off_set[3];
  for(i =0; i < 8; ++i){
    bytes[i] = v_addr & mask1;
    v_addr = v_addr >> 4;
  }
  for(i = 0; i< 3; ++i){
    off_set[i] = bytes[i];
    //printf("off_set[%d] = %lu\n",i, off_set[i]);
  }
  for(i = 0; i< 3; ++i){
    result = result | (off_set[i] << 4 * i);
  }
  //printf("off_set = %1lx\n", result);
  //printf("off_set (int) = %d\n", (int)result);
  //printf("\n");
  return (int) result;
}

unsigned long make_1pages(unsigned long pages){
  unsigned long page_1st[5];
  unsigned long mask_v_1st = 0x00003;
  unsigned long result = 0;
  int i;
  for(i = 0; i < 5; ++i){
      pages = pages >>2;
  }
  for(i =0; i < 5; ++i){
    page_1st[i] = pages & mask_v_1st;
    pages = pages >> 2;
  }
  for(i =0; i < 5; ++i){
    //printf("page_1st[%d] = %lu\n", i,page_1st[i]);
    result = result | (page_1st[i] << 2 * i);
  }
  //printf("\n");
  return result;
}

unsigned long make_2pages(unsigned long pages){
  unsigned long page_2nd[5];
  unsigned long mask_v_2nd = 0x00003;
  unsigned long result = 0;
  int i;
  for(i =0; i < 5; ++i){
    page_2nd[i] = pages & mask_v_2nd;
    pages = pages >> 2;
  }
  for(i =0; i < 5; ++i){
    //printf("page_2nd[%d] = %lu\n", i,page_2nd[i]);
    result = result | (page_2nd[i] << 2 * i);
  }
  return result;

}

// The implementation of get_vpage_cr3 is provided in
// an object file, so no need to re-implement it
void initialize_vmanager(int policy)
{
	// Set LRU policy when passsed as a parameter
	if (policy)
		FIFO_policy = false;
	cr3 = get_vpage_cr3();
  // printf("\n");
	// printf("cr3: %p\n", cr3);
  if(FIFO_policy){
    for(int i = 0; i < sizeOfTLB; i ++) {
      for (int j = 0; j < 2; j ++) {
        tlb[i][j] = -1;
      }
    }
  }
  else{
    for(int i = 0; i < 10; i ++) {
      for (int j = 0; j < 2; j ++) {
        tlb_lru[i][j] = -1;
      }
    }
  }
}

//
// The implementation of following functions is required
//
int fetch_physical_frame(unsigned int v_addr)    // This is 1.2
{
	//TODO
	unsigned long mask1 = 0x0000000f;
	unsigned long mask_v_page = 0x0000f;
	unsigned long pages;
  unsigned long bytes[8];
  int i =0;
  for(i =0; i < 8; ++i){
    bytes[i] = v_addr & mask1;
    v_addr = v_addr >> 4;
  }

  pages = split_offset(bytes);
  unsigned long page1 = make_1pages(pages);
  unsigned long page2 = make_2pages(pages);
  if(*(cr3 + page1) == NULL){
      printf("Failed to get first page\n");
	  	return -1;
	}                                           // &(cr3[(int)page1][(int)page2]) == NULL
  else if(cr3[(int)page1][(int)page2] == -1){   //cr3[(int)page1][(int)page2] == 0  (*((cr3 + page1) + page2) == NULL)
    // printf("cr3 + page1 + page2 : %p\n", *(cr3 + page1 + page2));
    // printf("Failed to get physical frame\n");
    printf("Failed to get second page\n");
    return -1;
  }
   else{
	 	return (int)cr3[(int)page1][(int)page2];
	 }
}

void print_physical_address(int frame, int offset) // This is 1.2
{
	//TODO
  char front[] = {"0x"};
	if(frame == -1){
		printf("-1\n");
		return;
	}
  else{
    for(int i =0; i < 3; ++i){
      frame = frame << 4;
    }
    int result = frame + offset;
    printf("==============================\n");
    printf("Physical address is %s%1x\n",front, result);
    printf("==============================\n");
  }
	return;
}


/***********************************************
*************** TLB ****************************
************************************************/

int count_FIFO = 0;
int count_LRU = 0;
int hit_count = 0;
int total_run = 0;



int track_FIFO() {
  return count_FIFO;
}


void setLRU(){
  for(int i=0;i<10;i++){
    bool flag=true;
    for(int j=i+1;j<10;j++)
      if(hist[j]==hist[i])
        flag=false;
    if(flag==true){
      count_LRU=i;
      return;
    }
  }
  return;
}

int track_LRU() {
  if(h!=9)
    return h;
  else{
    setLRU();
    return count_LRU;
}
}

// int n --> containing the virtual address 20 MSB
// return physical frame base address mapped to that particular virtual address
int get_tlb_entry(int n)
{
  for (int i = 0; i < 8; i ++) {
    if (tlb[i][0] == n) {
      return tlb[i][1];
    }
  }
  // printf("TLB miss\n");
  return -1;
}


void updateHist(int k){
  if(h==9){
    for(int i=1;i<=9;i++)
      hist[i-1]=hist[i];
    hist[9]=k;
  }
  else{
    hist[h]=k;
    h++;
  }
  return;
}


int get_tlb_entry_up_lru(int n)
{
  for (int i = 0; i < 10; i ++) {
    if (tlb_lru[i][0] == n) {
      updateHist(i);
      return tlb_lru[i][1];
    }
  }
  // printf("TLB miss\n");
  return -1;
}



void populate_tlb(int v_addr, int p_addr)
{
  total_run++;
  if (v_addr == -1 || p_addr == -1) {
    return;
  }
  if (FIFO_policy) {
    if (get_tlb_entry(v_addr) != -1) {
      hit_count ++;
      return;
    } else {
      int x = track_FIFO();
      tlb[x][0] = v_addr;
      tlb[x][1] = p_addr;
      count_FIFO ++;
      if (count_FIFO == sizeOfTLB) {
        count_FIFO = 0;
      }
    }
    return;
  }
  else  {
    if (get_tlb_entry_up_lru(v_addr) != -1) {
      hit_count ++;
      return;
    } else {
      int x = track_LRU();
      tlb_lru[x][0] = v_addr;
      tlb_lru[x][1] = p_addr;
      updateHist(x);
      setLRU();
    }
    return;
  }
}


float get_hit_ratio()
{
  if (FIFO_policy) {
    printf("hit_count: %d\n", hit_count);
    printf("total_run: %d\n", total_run);
    float ratio = (float) hit_count / total_run;
    return ratio;
  }
  // LRU
  else {
    printf("hit_count: %d\n", hit_count);
    printf("total_run: %d\n", total_run);
    float ratio = (float) hit_count / total_run;
    return ratio;
  }
}

//Write to the file in OUT_TLB
//
void print_tlb()
{
  FILE *outfile = fopen(OUT_TLB, "w");
  fprintf(outfile,"  virtual  |  physical  \n");
  fprintf(outfile, "----------------------\n");

  if(FIFO_policy){
  for (int i = 0; i < sizeOfTLB; i ++) {
    if (tlb[i][0] == -1 || tlb[i][1] == -1) {
      fprintf(outfile, "%d:  -1     |     -1  \n", i);
    } else {
      fprintf(outfile, "%d: %05x   |    %05x\n", i, tlb[i][0], tlb[i][1]);
    }
  }
    fprintf(outfile, "track_FIFO: %d", track_FIFO());
  } else {
    for (int i = 0; i < 10; i ++) {
      if (tlb_lru[i][0] == -1 || tlb_lru[i][1] == -1) {
        fprintf(outfile, "%d:  -1     |     -1  \n", i);
      } else {
        fprintf(outfile, "%d: %05x   |    %05x\n", i, tlb_lru[i][0], tlb_lru[i][1]);
      }
    }
    fprintf(outfile, "track_LRU: %d", track_LRU());
  }
  fclose (outfile);
}
