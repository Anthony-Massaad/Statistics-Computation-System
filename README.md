# Statistics Computation System (Using Message Queues)

## Description
This repository simulates a client-server calculator program that allows the user to store and perform operations on numbers. It puts the use of Linux message queues to allow the Client and the Server to communicate with each other and perform the necessary tasks. 

This program consist of two processes: User and Calculator which both will run on the same Central Processing Unit (CPU). The User is the client while Calculator is the server of the program. The Calculator is responsible for storing and performing operations on numbers, while the user is responsible for sending the following commands to the Calculator to perform the operations: 

1) **Insert**: This command takes in a number as an argument and, in response, stores that number in a set of numbers that is stored in the system. 

2) **Delete**: This command takes in a number as an argument and, in response, will delete every occurent of the given number from the stored numbers in the system. 

3) **Sum**: This command signals the Calculator to return the sum of all the existing numbers stored in the set of numbers.

4) **Median**: This command signals the Calculator to return the median value(s) for the stored set of numbers. When the number of values in the set is even, Calculator returns two values. 

5) **Min**: This command signals the Calculator to return the smallest existing number in the stored set of numbers.

6) **Average**: This command singals the Calculator to return the average of all the existing numbers in the stored set of numbers.

7) **Exit**: This signals the calculator to return the sum, average, min, and median of all the existing numbers i nthe set of numbers stored numbers, which then transitions to the program to deleting the existing queues and ending.

The Calculator stores the set of numbers in an array. The size of the array is set to 100 but can be changed by modifying the MAX_SIZE constant at the top of the ```calculator.c``` file.

Lastly, at the end of every command, the calculator will also return the time to perform the operation. The Exit command will return the average time of all the previous commands used.

## Running the Program
- To run the program, it must be set in a Linux environment
- Requires gcc version of 9.3.1 or higher 

1. To compile the program, simply use the makefile by typing:
```
$ make
```
2. After, an executables named ```calcualtor``` and ```user``` will be created. Open two different terminals and run the following concurrently on separate terminals:

Terminal 1:
``` 
$ ./calculator
```
Terminal 2:
```
$ ./user
```
## Instructions
Once running, the terminal that has the calculator running will show nothing, but will perform all tasks provided by the user through the backend of the server

The User terminal will output the list of commands and ask the user to enter a command (numeric choice). Only commands "Delete" and "Insert" will prompt the user to enter another numeric argument to go with the command. Otherwise, all other commands is a one time input. At the same time, any invalid input will notify the user of the user of the invalid input.

### Example without invalid inputs
![image](https://user-images.githubusercontent.com/62800170/164132000-03647e35-b71c-4550-b0a7-cfcfc948db8a.png)

### Example with invalid inputs
![image](https://user-images.githubusercontent.com/62800170/164132114-feef3b50-0dd6-4fe7-a6d2-f949c9406e15.png)

## Credits
Author: Anthony Massaad

Copyright © 2021 Anthony Massaad. All rights reserved
