//Lab2.c By Kuan-Ping Chang 71018021

#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>

//function declaration
int mySearch(int fd[], int valArr[], int begin, int end, int find);


//set pid using fork() and have each search for either 1st half or 2nd half 
int mySearch(int fd[], int valArr[], int begin, int end, int find) {
  pid_t pid, pid2;
  int buf; 
  if (begin == end) { //if begin and end are the same
    printf("pid %d value: %d\n", getpid(), valArr[begin]);
    if (valArr[begin] == find) { //if found otherwise exit
      close(fd[0]); //close
      buf = begin;
      write(fd[1], &buf, sizeof(buf));
    }
    exit(0);
  } 
  else {
    pid = fork();
    if (pid == 0) { //child 1
      mySearch(fd, valArr, begin, ((begin + end)/2), find);
      waitpid(-1, NULL, 0);//waiting for pid to finish
      exit(0);
    } else {
      pid2 = fork();
      if (pid2 == 0) { //child 2  
        mySearch(fd, valArr, (((begin + end)/2)+1), end, find);
        waitpid(-1, NULL, 0); //waiting for pid to finish
        exit(0);
      }
    }
  }
}


//Main function to run the program
int main(int argc, char *argv[]) {
  FILE *fp;
  int count, readBytes, last, buf, searchV;
  int first = 0, fcount = 0, numToks = 0, nValue = 0;
  char* token;
  char* ntoken;
  int nArray[10];
  int fd[2];
  char readline[100];
  char cpyline[100];
  //for array building
  int idx = 0, valuesValid = 1, gVal = 0, temp;
  char junk;


  const char s[] = "\n";
  
  pid_t pid; //tag pid 
  
  if (argc != 3) { //check if argc is correct
    printf("Not a valid call of MySearch. Try again.\n");
    return 0;
  } 
  else {
    searchV = atoi(argv[2]); //convert to int

	//open the file from the correct position
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		perror("Error opening file");
		return -1;
	} 
	else {
		if (fgets(readline, 100, fp) != NULL) {
			strcpy(cpyline, readline);
			token = strtok(readline, s); //split string containing /n and space
			token = strtok(token, " ");
  
			while (token != NULL) { //if null end...
		
				numToks += 1;
				token = strtok(NULL, " ");
			}
	
			if (numToks > 10) { //spec says no more than 10 
				printf("Search Length must be less than 10\n");
				nValue = -1;
			} 
			else if (numToks == 0) {//no values
				nValue = 0;
			}	 
			else {
			
				ntoken = strtok(cpyline, s);
				ntoken = strtok(ntoken, " ");
				while((valuesValid != 0) && (ntoken != NULL)) { //making sure these are valid
					
					gVal = sscanf(ntoken, "%d%c", &temp, &junk);
					ntoken = strtok(NULL, " ");
					
					if (gVal == 1) {
						  nArray[idx] = temp;
						  idx += 1;
						  nValue += 1;
					} 
					else {
						  valuesValid = 0; //if 0 then strtok isnt valid...end...
						  nValue = -1;
					}
				}

				if (numToks != nValue){
					printf("No value in array.\n");
					nValue = -2;
				}
			} 
		} 
		else {
			nValue = 0;
		} 
	}
	//close file 
	fclose(fp);
	count = nValue;

    if (count > 0) {
      last = count - 1;
    } 
	else if (count == 0) {
      printf("-1\n");
      return 0;
    } 
	else if (count == -1) {
      return -1;
    } 
	else {
      return 0;
    }

	//start the pipe here...
	//search then wait for all child to finish
	//last close all pipe 
    pipe(fd); //start a pipe to store position
    mySearch(fd, nArray, first, last, searchV); //go through the search 
    waitpid(-1, NULL, 0); //wait for child
    close(fd[1]); //close 

	//check to see if any target search is/are found
    do{
      readBytes = read(fd[0], &buf, sizeof(buf));
      //printf("Read bytes: %d", readBytes);
      if (readBytes > 0){
        //printf("%d\n", buf);
        fcount++;
      } 
    } while(readBytes > 0);
    
    close(fd[0]); //close 
	
    if (fcount == 0) {//if none
      printf("Search Output: -1\n");	
    }
    else {//if found
      printf("Search Output: 0\n");
    }
  }
  return 0;
}
