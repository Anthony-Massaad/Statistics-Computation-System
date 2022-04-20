#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#define MAX_TEXT 512
// declare struct for 3 different message queues. Command, arguement and result
// different struct due to easier coding of knowing what is going on in what
struct my_msg_st {
	long int my_msg_type;
	char command_input[MAX_TEXT];
};

struct my_msg_args{
	long int my_msg_type;
	char args_input[MAX_TEXT];
};

struct my_msg_result{
	long int my_msg_type;
	char result_text[BUFSIZ];
};
// print the start of the program
void printStart(){
	printf("Welcome to calculator program\n");
	printf("-------------------------------\n");
	printf("options <numeric>:\n");
	printf("(1) Insert\n");
	printf("(2) Delete\n");
	printf("(3) Sum\n");
	printf("(4) Median\n");
	printf("(5) Min\n");
	printf("(6) Average\n");
	printf("(7) Exit\n");
	printf("-------------------------------\n");
}

int main(){
	// declare and initalize any variables needed :')
	int running = 1;
	struct my_msg_st command_message;
	struct my_msg_args args_message; 
	struct my_msg_result result_message; 
	int msgid, msgid2;
	long int msg_to_receive = 0;
	char input[MAX_TEXT];
	char *ptr;
	int result;

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	msgid2 = msgget((key_t)1235, 0666 | IPC_CREAT);

	// check if the first message id queue fails
	if (msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	// check if the second message id queue fails
	if (msgid2 == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	printStart();
	// infinite loop to keep getting user input until user ends the program
	while(running) {
		// infinite loop until the user enters a valid choice 
		// if valid set the choice into the choice command messages to send to the calculator
		while(1){
			printf("Enter choice <numeric>: ");
			fgets(input, sizeof input, stdin);
			if (strlen(input) <= 35){
				result = strtol(input, &ptr, 10);
				if (result > 0 && result < 8){
					snprintf(command_message.command_input, 10, "%d", result);
					break;
				}else{
					printf("invalid choice\n");
				}
			}else{
				printf("Too many characters\n");
			}
			
		}
		// if the choice is delete or insert, get the arguments of the user 
		// set the arguuments into the argument messsage to send to the calculator
		if (result == 1 || result == 2){
			printf("Enter argument <numbers only>: ");
			fgets(input, sizeof input, stdin);
			int val = strtol(input, &ptr, 10);
			snprintf(args_message.args_input, 10, "%d", val);
		}
		// set the message types accordingly 
		command_message.my_msg_type = 1;
		args_message.my_msg_type = 1; 
		// send the command message to the calculator. Exits program if failed
		if (msgsnd(msgid, (void *)&command_message, MAX_TEXT, 1) == -1) {
			fprintf(stderr, "msgsnd 1 failed\n");
			exit(EXIT_FAILURE);
		}
		// send the argument commands to the calculator. exits program if failed
		if (msgsnd(msgid, (void *)&args_message, MAX_TEXT, 1) == -1) {
			fprintf(stderr, "msgsnd 2 failed\n");
			exit(EXIT_FAILURE);
		}
		// recieve the outcome message from the calculator
		if (msgrcv(msgid2, (void *)&result_message, BUFSIZ, msg_to_receive, 0) == -1) {
			fprintf(stderr, "msgrcv user failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		// print the outcome
		printf(result_message.result_text);
		printf("\n");
		// if the command is exit, break out of the while loop 
		if (result == 7) {
			running = 0;
		}
	}
	exit(EXIT_SUCCESS);
}
