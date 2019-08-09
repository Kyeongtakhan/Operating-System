# Project4 : Message Queues and Shared Memory
# Group Member: Kyeongtak Han, Dongha Kang, Vedaant Sukhtankar
# x500: Han00127, kangx637, sukht002
# File list : server.c client1.c client2.c makefile README.txt

#information:
This program creates the message queues to communicate between client1, client2 with the server.
Since server and clients need the private key to communicate with others, we used Shared memory
to send out receive the private key between the server and clients (in this particular project
we set them to 777 and 888. However it is possible we can just randomly generate the key).

1 will send message and get message from client1. 2 will communicate with client2 and 3 will
send the message to both of client1 and client2.


# Contribution:
Kyeongtak Han, Veedaant Sukhtankar, Dongha Kang distributed equally and contributed in this project equally.
We did not divide each others work by Shared Memory, Message Queues, and Signals.


# Brief overview (server.c)
1. print_message : simply prints out the description of what to do
3. firstClient : used to communicate with the Client1 using message queue.
4. secondClient : used to communicate with the Client2 using message queue.
5. broadcasting : used to send a message to both client1 and client2.
6. p_exit : frees the allocated memory and exiting the server after closing the connection.


# Brief overview (client1.c and client2)
main - There is just one big main function that receives and sends the message from the Server
       This main function only deals with the struct of message. Therefore, this file
       does not need extra specific functions.



# Using makefile
make all : to compile all files (server.c client1.c client2.c)
make clean : rm all three files.


# Test cases used: (in terminal)
1. 1 (to choose client 1)
2. 2 (to choose client 2)
3. 3 (to broadcast)

#Examples:
1)
$ ./server

Enter 1 to choose client 1
      2 to choose client 2
      3 to broadcast a message
      0 to exit

Enter Choice: 1
Write message : hello Susan !

Message Received : hello Brian !

Enter 1 to choose client 1
      2 to choose client 2
      3 to broadcast a message
      0 to exit



$ ./client1
Message Received : hello Susan !
Write Response : hello Brian !


2) similar to the first Example

3)
$ ./server

Enter 1 to choose client 1
      2 to choose client 2
      3 to broadcast a message
      0 to exit

Enter Choice: 3
Write message to broadcast: HELLO EVERYONE !!
HELLO EVERYONE !!


$ ./client1
Message Received : HELLO EVERYONE !!
