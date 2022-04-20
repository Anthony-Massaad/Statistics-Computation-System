#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/time.h>
#define MICRO_SEC 1000000
#define MAX_SIZE 100 
// declare struct for 3 different message queues. Command, arguement and result
// different struct due to easier coding of knowing what is going on in what
struct my_msg_st {
	long int my_msg_type;
	char command_input[BUFSIZ];
};

struct my_msg_args{
	long int my_msg_type;
	char args_input[BUFSIZ];
};

struct my_msg_result{
	long int my_msg_type;
	char result_text[BUFSIZ];
};
// method to sort the array passed numerically ascending order 
// return the array
int * getSortedArray(int array[], int size){
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size-i-1; j++){
			if (array[j] > array[j + 1]){
				int t = array[j];
				array[j] = array[j+1];
				array[j+1] = t;
			}
		}
	}
	return array;
}
// method to get the total time taken given the start and end time
float getTimeTaken(struct timeval end, struct timeval start){
	return ((end.tv_sec * MICRO_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC + start.tv_usec));
}

int main(){
	// declare and initalize any variables needed :')
	int running = 1;
	int msgid, msgid2;
	struct my_msg_st command_message;
	struct my_msg_args args_message;
	struct my_msg_result result_message; 
	long int msg_to_receive = 0;
	int values[MAX_SIZE];
	char value[35];
	char message[200];
	int index = 0; 
	char *ptr; 
	int args;
	int min; 
	struct timeval start, end;
	float totalTime[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
	int totalTimeIndex[6] = {0, 0, 0, 0, 0, 0}; 
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
	// infinite loop to keep checking operations until user ends the program
	while(running) {
		// recieve the message command from user, exit program if failure
		if (msgrcv(msgid, (void *)&command_message, BUFSIZ, msg_to_receive, 0) == -1) {
			fprintf(stderr, "msgrcv 1 failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		// recieve the argument message from user, exit program if failure 
		if (msgrcv(msgid, (void *)&args_message, BUFSIZ, msg_to_receive, 0) == -1) {
			fprintf(stderr, "msgrcv 2 failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		// convert the command to an integer (always true never errors)
		int command = strtol(command_message.command_input, &ptr, 10);
		// run program accordingly to the command
		switch(command){
			// if command is insert
			case 1: 
				// begin the timer of the operation
				gettimeofday(&start, NULL);
				// convert the arguement into an integer (always true never errors)
				args = strtol(args_message.args_input, &ptr, 10);
				// add args into the set of values and increment the set size
				values[index] = args; 
				index++;
				// parse result of insert into the message queue back to the user 
				// announce success and show set of numbers
				strcpy(message, "Command Insert success\nlist: [");
				for (int i = 0; i < index; i++){
					snprintf(value, 10, "%d", values[i]);
					strcat(message, value);
					if (i != index-1)
						strcat(message, ", ");
				}
				strcat(message, "]");
				// collect time taken to perform operation, and store the time taken into the set.
				// increment the set size of time
				gettimeofday(&end, NULL);
				totalTime[0] += getTimeTaken(end, start);
				totalTimeIndex[0]++;
				break;
			// command for delete
			case 2: 
				// ensure that there are indeed values in the set. 
				// if so, do operation, otherwise skip
				if (index > 0){
					// begin the timer of the operation
					gettimeofday(&start, NULL);
					// convert the arguement into an integer (always true never errors)
					args = strtol(args_message.args_input, &ptr, 10);
					int currIndex = 0;
					// loop through entire set of values starting from index 0 (currentIndex) until it is above the size of set
					while (currIndex < index){
						// if the current index value is not the same as the argument, move onto next index
						// otherwise perform deletion accordingly, and subtract the size
						if (values[currIndex] == args){
							for (int i = currIndex; i<=index; i++){
								values[i] = values[i+1];
							}
							index--; 
						}else{
							currIndex++; 
						}
					}
					// parse result of operation into the message queue back to the user 
					// announce success and show set of numbers
					strcpy(message, "Command Delete success\nlist: [");
					for (int i = 0; i < index; i++){
						snprintf(value, 10, "%d", values[i]);
						strcat(message, value);
						if (i != index-1)
							strcat(message, ", ");
					}
					strcat(message, "]");
					// collect time taken to perform operation, and store the time taken into the set.
					// increment the set size of time
					gettimeofday(&end, NULL);
					totalTime[1] = getTimeTaken(end, start);
					totalTimeIndex[1]++;
				}else{
					// announce failure to perform command due to lack of values 
					strcpy(message, "Command Delete ignored, no values to delete");
				}
				break;
			// command for sum
			case 3: 
				// begin the timer of the operation
				gettimeofday(&start, NULL);
				// initalize the message that sum is success
				strcpy(message, "Command Sum success.\nTotal = ");
				// get the sum of the set of values
				int total = 0;
				for (int i = 0; i <= index; i++){
					total += values[i];
				}
				// parse result of operation into the message queue back to the user 
				snprintf(value, 10, "%d", total);
				strcat(message, value);
				// collect time taken to perform operation, and store the time taken into the set.
				// increment the set size of time
				gettimeofday(&end, NULL);
				totalTime[2] = getTimeTaken(end, start);
				totalTimeIndex[2]++;
				break;
			// command for median
			case 4: 
				// ensure that there are indeed values in the set. 
				// if so, do operation, otherwise skip
				if (index > 0){
					// begin the timer of the operation
					gettimeofday(&start, NULL);
					// initalize the message that median is success
					strcpy(message, "Command Median success.\nSorted Array = ");
					// sort the array and initialize it as copyValues
					int copyValues[MAX_SIZE];
					memcpy(copyValues, getSortedArray(values, index), MAX_SIZE);
					// add to the message queue the display of the sorted array
					for (int i = 0; i < index; i++){
						snprintf(value, 10, "%d", copyValues[i]);
						strcat(message, value);
						if (i != index-1)
							strcat(message, ", ");
					}
					strcat(message, "]\n");
					// if the size of the set of values is even, get median for even
					// otherwise, get median for odd
					if (index % 2 == 0){
						// declare and initialize the two center values of the sorted array
						int val1, val2; 
						val1 = copyValues[(index-1)/ 2];
						val2 = copyValues[((index-1)/2) + 1];
						// add to the message queue the result of the median operation
						strcat(message, "size of set is even\n");
						snprintf(value, 10, "%d", val1);
						strcat(message, "Middle value 1 is ");
						strcat(message, value);
						strcat(message, "\nMiddle value 2 is ");
						snprintf(value, 10, "%d", val2);
						strcat(message, value);

					}else{
						// declare and initialize the center values of the sorted array
						int val3 = copyValues[((index-1)/2)];
						// add to the message queue the result of the median operation
						strcat(message, "size of set is odd\n");
						snprintf(value, 10, "%d", val3);
						strcat(message, "Middle value is "); 
						strcat(message, value);						
					}
					// collect time taken to perform operation, and store the time taken into the set.
					// increment the set size of time
					gettimeofday(&end, NULL);
					totalTime[3] = getTimeTaken(end, start);
					totalTimeIndex[3]++;
				}else{
					// announce failure to perform command due to lack of values 
					strcpy(message, "Command Median ignored, no values in set");
				}
				break;
			// command for Min 
			case 5: 
				// ensure that there are indeed values in the set. 
				// if so, do operation, otherwise skip				
				if (index > 0){
					// begin the timer of the operation
					gettimeofday(&start, NULL);
					// initalize the message that Min is success
					strcpy(message, "Command Min success.\n");
					// get the minimum value from the set of values
					min = values[0];
					for (int i = 1; i < index; i++){
						if (values[i] < min){
							min = values[i];
						}
					}
					// parse result of operation into the message queue back to the user 
					snprintf(value, 10, "%d", min);
					strcat(message, "Minimum value is "); 
					strcat(message, value);
					// collect time taken to perform operation, and store the time taken into the set.
					// increment the set size of time
					gettimeofday(&end, NULL);
					totalTime[4] = getTimeTaken(end, start);
					totalTimeIndex[4]++;					
				}else{
					// announce failure to perform command due to lack of values 
					strcpy(message, "Command Min ignored, no values in set");
				}
				break;
			// command for Average
			case 6: 
				// ensure that there are indeed values in the set. 
				// if so, do operation, otherwise skip
				if (index > 0){
					// begin the timer of the operation
					gettimeofday(&start, NULL);
					// initalize the message that operation is success
					strcpy(message, "Command Average success.\n");
					// get the average value from the set of values
					float total = 0;
					for (int i = 0; i < index; i++){
						total += values[i];
					}
					float average = total/index;
					// parse result of operation into the message queue back to the user 
					snprintf(value, 10, "%.2f", average);
					strcat(message, "Average value is "); 
					strcat(message, value);
					// collect time taken to perform operation, and store the time taken into the set.
					// increment the set size of time
					gettimeofday(&end, NULL);
					totalTime[5] = getTimeTaken(end, start);
					totalTimeIndex[5]++;		
				}else{
					// announce failure to perform command due to lack of values 
					strcpy(message, "Command Average ignored, no values in set");
				}
				break;
			// command for Exit 
			default:
				// initalize the message of operation
				strcpy(message, "Ending Program Now\n");
				// get the total average
				float averageTimes[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
				float overallAverage = 0.0;
				float totalCalls = 0;
				for (int i = 0; i < 6; i++){
					float calls = totalTimeIndex[i];
					if (calls == 0) calls++; 
					averageTimes[i] += totalTime[i]/calls;
					overallAverage += averageTimes[i];
					totalCalls += totalTimeIndex[i];
				}
				if (totalCalls == 0) totalCalls = 1; 
				overallAverage = overallAverage/totalCalls;
				// parse the average time into the message queue
				strcat(message, "Average time taken to run each operation are:\n");
				strcat(message, "Insert: ");
			 	snprintf(value, 10, "%.2f", averageTimes[0]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "Delete: ");
			 	snprintf(value, 10, "%.2f", averageTimes[1]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "Sum: ");
			 	snprintf(value, 10, "%.2f", averageTimes[2]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "Median: ");
			 	snprintf(value, 10, "%.2f", averageTimes[3]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "Min: ");
			 	snprintf(value, 10, "%.2f", averageTimes[4]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "Average: ");
			 	snprintf(value, 10, "%.2f", averageTimes[5]);
				strcat(message, value);
				strcat(message, " ms\n");
				strcat(message, "---------------------\nOverall average: ");
				snprintf(value, 10, "%.2f", overallAverage);
				strcat(message, value);
				strcat(message, " ms");
				break; 
		}
		// set the message into the message queue of the result_text to the user
		strcpy(result_message.result_text, message);
		result_message.my_msg_type = 1;
		// send the message to the user. Ends program if failure
		if (msgsnd(msgid2, (void *)&result_message, BUFSIZ, 1) == -1) {
			fprintf(stderr, "msgsnd id2 failed\n");
			exit(EXIT_FAILURE);
		}
		// if the command is exit, exit the while loop by setting running to 0
		if (command == 7) {
			running = 0;
		}
	}
	// clear the message ids
	if (msgctl(msgid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	if (msgctl(msgid2, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) 2 failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
