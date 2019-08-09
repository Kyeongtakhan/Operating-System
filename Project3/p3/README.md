# Project3 : VManager
# Group Member: Kyeongtak Han, Dongha Kang, Vedaant Sukhtankar
# x500: han00127, kangx637, sukht002
# File list : vmemory.c vmemory.h main.c

#information:
This program is to demonstrate the TLB in FIFO and LRU to use memory more efficient. When this program is running. It reads virtual.txt file that contains
all the virtual addresses. After reading virtual addresses, it calculates the
physical addresses and put virtual address and physical address in the TLB table.
There also are two algorithm put the addresses in the table, which are FIFO (first in first out) and LRU (least recently used).


# Contribution:
Kyeongtak Han worked on transferring virtual memory address to physical address
Dongha Kang worked on TLB format with FIFO algorithm, and Vedaant implemented LRU algorithm.


# Brief overview (main.c)
- There is only a main function in "main.c" file.
- Main function, takes a input to figure out whether we are using FIFO
  or LRU for TLB. If there is no argument, it runs in FIFO policy and if there is a argument ('-lru'), it runs in LRU policy.
- it opens the file that contains virtual memory addresses and in the while
  loop, calucates the physical address and put in the table by using functions from 'vmemory.c'.


# Brief overview (vmemory.c)
- Calculation of the virtual memory to physical memory
1. split_offset - split the virtual address into frame and offset. (e.g.   
                  0x4f65d40c to 4f65d and 40c)
2. get_offset - takes a virtual address as an argument and print out the offset
                value. (e.g. 0x4f65d40c --> 40c)
3. make_1pages - takes a page number as an argument and print out the first
                 page number (0 ~ 1023)
4. make_2pages - takes a page number as an argument and print out the second
                 page number that first page is pointing to (0 ~ 1023)
5. initialize_vmanager - checking whether they are using FIFO or LRU and Also
                         initialize tlb array
6. fetch_physical_frame - takes virtual address as an argument and change
                          virtual frame to the physical frame by using all the
                          functions above. Use virtual address to track the page1 to page2 and if second page has a value inside,
                          fetch that physical frame and if not return -1.
7. print_physical_address - just help to print physical address in a certain
                            format (e.g. 0x77c6f032).

- TLB
1. get_tlb_entry - takes virtual frame as an argument. If tlb table contains
                   a virtual frame that is same as the argument, print the
                   physical frame that virtual frame holds.
2. is_tlb_slot_empty - return true if tlb is empty
3. track_FIFO and track_LRU - used to track where the current placement is in
                              certain policy.
4. populate_tlb - takes v_addr, p_addr and put inside the slot in TLB
                  table. First, it finds whether or not the v_addr is in the table. If so, don't do anything. Second, if the table has an empty slot, put the values in there. If not, with the policy and track_FIFO/track_LRU, put inside the slot where it is right.
5. get_hit_ratio - print out the number of times that it get hit divided by all
                   the virtual addresses that we are testing.
6. print_tlb - creates/overwrites tlb_out.txt in bin folder and print the TLB
               table in the file.


# testing
We added extra cases in the virtual.txt file which occurs page fault. We assume that all the virtual memory consists of 32 bytes long.
Extra case: 0x00000001
                   0x00000002
                   0x00001123
                   0x20480123
                   0xcb000143
                   0x32d75196
                   0xcb175175
                   0xcb010789

# Test case used: ./vmanager                   
==============================
Physical address is 0x7b6f8665
==============================
==============================
Physical address is 0x78ac5f2d
==============================
==============================
Physical address is 0x72a2c207
==============================
==============================
Physical address is 0x7aba827c
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x73b0f879
==============================
==============================
Physical address is 0x7aba8c7c
==============================
==============================
Physical address is 0x7f90f676
==============================
==============================
Physical address is 0x77c6f40c
==============================
==============================
Physical address is 0x7084c0fe
==============================
==============================
Physical address is 0x77c6f4dc
==============================
==============================
Physical address is 0x763a11b8
==============================
==============================
Physical address is 0x78ac522d
==============================
==============================
Physical address is 0x73bf9df7
==============================
==============================
Physical address is 0x770e02a5
==============================
==============================
Physical address is 0x7be7e7bf
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x763a11b8
==============================
==============================
Physical address is 0x7be7e7bf
==============================
==============================
Physical address is 0x75749009
==============================
==============================
Physical address is 0x7084c0ff
==============================
==============================
Physical address is 0x7aa42fc6
==============================
==============================
Physical address is 0x7da2dcab
==============================
==============================
Physical address is 0x7aba857c
==============================
==============================
Physical address is 0x733823eb
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x7f90f6b6
==============================
==============================
Physical address is 0x733822eb
==============================
==============================
Physical address is 0x733823ff
==============================
==============================
Physical address is 0x72edd27c
==============================
==============================
Physical address is 0x74aef252
==============================
==============================
Physical address is 0x770037d5
==============================
==============================
Physical address is 0x725e228e
==============================
==============================
Physical address is 0x72edd27a
==============================
==============================
Physical address is 0x7bd82d08
==============================
==============================
Physical address is 0x7bd82d08
==============================
==============================
Physical address is 0x725e208f
==============================
==============================
Physical address is 0x72edda7a
==============================
==============================
Physical address is 0x72edd27c
==============================
==============================
Physical address is 0x73b6a1a8
==============================
==============================
Physical address is 0x712a1d75
==============================
==============================
Physical address is 0x73b6a1a8
==============================
==============================
Physical address is 0x712a1d76
==============================
==============================
Physical address is 0x70b138cf
==============================
==============================
Physical address is 0x75eb447a
==============================
==============================
Physical address is 0x73f6ff7f
==============================
==============================
Physical address is 0x76bd05ee
==============================
==============================
Physical address is 0x71f462cb
==============================
==============================
Physical address is 0x70baa92b
==============================
==============================
Physical address is 0x7d9ea628
==============================
==============================
Physical address is 0x733f7996
==============================
==============================
Physical address is 0x78514a77
==============================
==============================
Physical address is 0x77110a84
==============================
==============================
Physical address is 0x78514a77
==============================
==============================
Physical address is 0x724d9a5d
==============================
==============================
Physical address is 0x79a24573
==============================
==============================
Physical address is 0x7b359a89
==============================
==============================
Physical address is 0x7df4460a
==============================
==============================
Physical address is 0x75c52105
==============================
==============================
Physical address is 0x71f40530
==============================
==============================
Physical address is 0x7ef2b0b5
==============================
==============================
Physical address is 0x733aba3b
==============================
==============================
Physical address is 0x7c8556e0
==============================
==============================
Physical address is 0x76611e46
==============================
==============================
Physical address is 0x78530b09
==============================
==============================
Physical address is 0x7b7946ef
==============================
==============================
Physical address is 0x7f0c09eb
==============================
==============================
Physical address is 0x756beab3
==============================
==============================
Physical address is 0x76825722
==============================
==============================
Physical address is 0x751003a4
==============================
==============================
Physical address is 0x752eb87a
==============================
==============================
Physical address is 0x7af437fd
==============================
==============================
Physical address is 0x7805aa9e
==============================
==============================
Physical address is 0x73c03c25
==============================
==============================
Physical address is 0x7e27647c
==============================
==============================
Physical address is 0x75981c9b
==============================
==============================
Physical address is 0x706c2d9b
==============================
==============================
Physical address is 0x7f178068
==============================
==============================
Physical address is 0x79f75a84
==============================
==============================
Physical address is 0x7b6dcb31
==============================
==============================
Physical address is 0x7c6fc6ce
==============================
==============================
Physical address is 0x7807ac15
==============================
==============================
Physical address is 0x74e8cf63
==============================
==============================
Physical address is 0x7065ef14
==============================
==============================
Physical address is 0x7ea78bb2
==============================
==============================
Physical address is 0x7287c85c
==============================
==============================
Physical address is 0x7abfdaea
==============================
==============================
Physical address is 0x78fcf6cd
==============================
==============================
Physical address is 0x7450919e
==============================
==============================
Physical address is 0x77af2596
==============================
==============================
Physical address is 0x7b456468
==============================
==============================
Physical address is 0x7f6b28da
==============================
==============================
Physical address is 0x743c297f
==============================
==============================
Physical address is 0x7b456467
==============================
==============================
Physical address is 0x7b7e9b99
==============================
==============================
Physical address is 0x743c2978
==============================
==============================
Physical address is 0x743c2201
==============================
==============================
Physical address is 0x79364472
==============================
==============================
Physical address is 0x7b7e9b88
==============================
==============================
Physical address is 0x743c2978
==============================
==============================
Physical address is 0x71728247
==============================
Failed to get first page
-1
Failed to get first page
-1
Failed to get first page
-1
Failed to get first page
-1
Failed to get second page
-1
Failed to get first page
-1
==============================
Physical address is 0x724d9175
==============================
Failed to get second page
-1
Page Fault : 7 times!
hit_count: 21
total_run: 109



=============tlb_out.txt==========================
  virtual  |  physical  
----------------------
0: cb175   |    724d9
1: 47e7f   |    77af2
2: 9efee   |    7b456
3: f4004   |    7f6b2
4: 559b5   |    743c2
5: a69cf   |    7b7e9
6: b732d   |    79364
7: 72ae2   |    71728
track_FIFO: 1






# Test case used: ./vmanager -lru
==============================
Physical address is 0x7b6f8665
==============================
==============================
Physical address is 0x78ac5f2d
==============================
==============================
Physical address is 0x72a2c207
==============================
==============================
Physical address is 0x7aba827c
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x73b0f879
==============================
==============================
Physical address is 0x7aba8c7c
==============================
==============================
Physical address is 0x7f90f676
==============================
==============================
Physical address is 0x77c6f40c
==============================
==============================
Physical address is 0x7084c0fe
==============================
==============================
Physical address is 0x77c6f4dc
==============================
==============================
Physical address is 0x763a11b8
==============================
==============================
Physical address is 0x78ac522d
==============================
==============================
Physical address is 0x73bf9df7
==============================
==============================
Physical address is 0x770e02a5
==============================
==============================
Physical address is 0x7be7e7bf
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x763a11b8
==============================
==============================
Physical address is 0x7be7e7bf
==============================
==============================
Physical address is 0x75749009
==============================
==============================
Physical address is 0x7084c0ff
==============================
==============================
Physical address is 0x7aa42fc6
==============================
==============================
Physical address is 0x7da2dcab
==============================
==============================
Physical address is 0x7aba857c
==============================
==============================
Physical address is 0x733823eb
==============================
==============================
Physical address is 0x7a60d616
==============================
==============================
Physical address is 0x7f90f6b6
==============================
==============================
Physical address is 0x733822eb
==============================
==============================
Physical address is 0x733823ff
==============================
==============================
Physical address is 0x72edd27c
==============================
==============================
Physical address is 0x74aef252
==============================
==============================
Physical address is 0x770037d5
==============================
==============================
Physical address is 0x725e228e
==============================
==============================
Physical address is 0x72edd27a
==============================
==============================
Physical address is 0x7bd82d08
==============================
==============================
Physical address is 0x7bd82d08
==============================
==============================
Physical address is 0x725e208f
==============================
==============================
Physical address is 0x72edda7a
==============================
==============================
Physical address is 0x72edd27c
==============================
==============================
Physical address is 0x73b6a1a8
==============================
==============================
Physical address is 0x712a1d75
==============================
==============================
Physical address is 0x73b6a1a8
==============================
==============================
Physical address is 0x712a1d76
==============================
==============================
Physical address is 0x70b138cf
==============================
==============================
Physical address is 0x75eb447a
==============================
==============================
Physical address is 0x73f6ff7f
==============================
==============================
Physical address is 0x76bd05ee
==============================
==============================
Physical address is 0x71f462cb
==============================
==============================
Physical address is 0x70baa92b
==============================
==============================
Physical address is 0x7d9ea628
==============================
==============================
Physical address is 0x733f7996
==============================
==============================
Physical address is 0x78514a77
==============================
==============================
Physical address is 0x77110a84
==============================
==============================
Physical address is 0x78514a77
==============================
==============================
Physical address is 0x724d9a5d
==============================
==============================
Physical address is 0x79a24573
==============================
==============================
Physical address is 0x7b359a89
==============================
==============================
Physical address is 0x7df4460a
==============================
==============================
Physical address is 0x75c52105
==============================
==============================
Physical address is 0x71f40530
==============================
==============================
Physical address is 0x7ef2b0b5
==============================
==============================
Physical address is 0x733aba3b
==============================
==============================
Physical address is 0x7c8556e0
==============================
==============================
Physical address is 0x76611e46
==============================
==============================
Physical address is 0x78530b09
==============================
==============================
Physical address is 0x7b7946ef
==============================
==============================
Physical address is 0x7f0c09eb
==============================
==============================
Physical address is 0x756beab3
==============================
==============================
Physical address is 0x76825722
==============================
==============================
Physical address is 0x751003a4
==============================
==============================
Physical address is 0x752eb87a
==============================
==============================
Physical address is 0x7af437fd
==============================
==============================
Physical address is 0x7805aa9e
==============================
==============================
Physical address is 0x73c03c25
==============================
==============================
Physical address is 0x7e27647c
==============================
==============================
Physical address is 0x75981c9b
==============================
==============================
Physical address is 0x706c2d9b
==============================
==============================
Physical address is 0x7f178068
==============================
==============================
Physical address is 0x79f75a84
==============================
==============================
Physical address is 0x7b6dcb31
==============================
==============================
Physical address is 0x7c6fc6ce
==============================
==============================
Physical address is 0x7807ac15
==============================
==============================
Physical address is 0x74e8cf63
==============================
==============================
Physical address is 0x7065ef14
==============================
==============================
Physical address is 0x7ea78bb2
==============================
==============================
Physical address is 0x7287c85c
==============================
==============================
Physical address is 0x7abfdaea
==============================
==============================
Physical address is 0x78fcf6cd
==============================
==============================
Physical address is 0x7450919e
==============================
==============================
Physical address is 0x77af2596
==============================
==============================
Physical address is 0x7b456468
==============================
==============================
Physical address is 0x7f6b28da
==============================
==============================
Physical address is 0x743c297f
==============================
==============================
Physical address is 0x7b456467
==============================
==============================
Physical address is 0x7b7e9b99
==============================
==============================
Physical address is 0x743c2978
==============================
==============================
Physical address is 0x743c2201
==============================
==============================
Physical address is 0x79364472
==============================
==============================
Physical address is 0x7b7e9b88
==============================
==============================
Physical address is 0x743c2978
==============================
==============================
Physical address is 0x71728247
==============================
Failed to get first page
-1
Failed to get first page
-1
Failed to get first page
-1
Failed to get first page
-1
Failed to get second page
-1
Failed to get first page
-1
==============================
Physical address is 0x724d9175
==============================
Failed to get second page
-1
Page Fault : 7 times!
hit_count: 16
total_run: 109


==============tlb_out.txt====================
  virtual  |  physical  
----------------------
0: 72ae2   |    71728
1: bdd7d   |    74509
2: cb175   |    724d9
3: a69cf   |    7b7e9
4: 559b5   |    743c2
5: 9efee   |    7b456
6: cb69c   |    733ab
7: ffffffff   |    ffffffff
track_LRU: 1
