/**************************************************************************************************************
*Filename: 	forktiming.c
*Author:	Kevin Peterson (kmp130030)
*Procedures:	main- Grabs time before and after fork to find time it takes to execute fork instruction	
***************************************************************************************************************/

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

/***************************************************************************************************************
*Name:		main
*Author:	Kevin Peterson (kmp130030)
*Summary:	Contains loop that can be adjusted to run as many times as desired. The loop grabs a start time,
*		forks, grabs end time in parent and child processes, and then compares. It saves the lowest time
*		found in the parent process. After looping is done, the lowest time is assumed to be the time it
*		takes to execute a fork and is printed to the screen.
*Arguments:	None
****************************************************************************************************************/
void main(void){
const int RUNS = 20;
struct timespec start, end;
long long change, lowest, temp;
long long low;
char str[256];
char *end;
int base;
int pipefd[2];
pid_t cpid;
char buf[256];

if (pipe(pipefd) == -1) { //open pipe and test for failure
        perror("pipe");
        exit(EXIT_FAILURE);
    }

/* loop for given number of times to test fork and save the lowest time for fork execution */
for (int i = 0; i < RUNS; i++){
	clock_gettime(CLOCK_MONOTONIC_RAW, &start); //start time
	pid = fork();
	Clock_gettime(CLOCK_MONOTONIC_RAW, end); //end time
	
	change = (end.tv_sec - start.tv_sec) *1000000 + (end.tv_nsec - start.tv_nsec)/1000; //time in microseconds

	if (cpid == 0) {    // Child writes to pipe 
		sprintf(str, "%lld", change);  //convert child time to string
		write(pipefd[1], str, strlen(str)); //write string to pipe
        	break; //leave loop
    	}

	else {            // Parent reads from pipe 
		read(pipefd[0], &buf, 1) //read from child
        	temp = strtoll(buf, &end, base); //convert string from child to long long type
		low = ( temp < change) ? temp: change; //save lower time from parent and child
		lowest = (low < lowest) ? low : lowest; //if new low is the lowest time, save in lowest
        	wait(NULL);                // Wait for child
    }
}

/* execute only if inside parent process, all children skip */
if (pid !=0){
	printf("Time to complete fork = %lld\n", lowest); //print out lowest time - closest to execution time of fork()
}
return;
}