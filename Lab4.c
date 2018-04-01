//By Kuan-Ping Chang 71018021

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


char convertToHexa(int hexNum);
int allocate(char * ptr, char * endPtr, int aSize, int bNum);
int freeBlock(char * ptr, char * endPtr, int blockNum);
void printingheader(char * ptr, char * endPtr, int blockNum);
void printHeap(char * ptr, char * endPtr, int blockNum, int numBytes);
void writeToBlock(char * ptr, char * endPtr, char eChar, int blockNum, int numWrites);
int showBlockList(char * ptr, char * endPtr, int starts, int blockCount);

int main() {
  
  char input[500], oInput[500];
  int Running = 0, heapSize = 127, blockCount = 0; 
  int args;
  int parameters;
  int iVal_1, iVal_2;
  int cSize, starts = 0; 
  int checkAllocate = 0;
  
  char command[500];
  char charEnter, heapVal;
  
  //initialize heap
  char *heap = (char*)malloc(heapSize * sizeof(char));
  
  heap[0] = 0;
  heap[1] = -2;
  int i = 2;
  while (i < 127) {
    heap[i] = 0;
    i++;
  }
  
  char *start = heap;
  char *end = heap + 127;

  while(Running == 0) {  
      printf("> ");
	  fgets(input, 100, stdin);

      if (strcmp(input, "quit\n") == 0) {
		//printf("access here\n");
        Running = 1;
      } 
      //display the block list
      else if (strcmp(input, "blocklist\n") == 0) {
        starts = showBlockList(start, end, starts, blockCount);
      } 
      else { //if input argument(s) is more than 1 
          strcpy(oInput, input);
          
          int tokCount = 0;
          char* token;
          const char s[] = "\n";
      
          token = strtok(oInput, s);
          token = strtok(token, " ");
      
          while (token != NULL) {
          tokCount += 1;
          token = strtok(NULL, " ");
          }
          args = tokCount;

          if (args == 2) {
            parameters = sscanf(input, "%s %d", command, &iVal_1);
            if (parameters != 2) {
              printf("Invalid command. Try again.\n");
            } 
            else {
              ///for allocating block
              if (strcmp(command, "allocate") == 0) {
                  if (iVal_1 > 125) {
                      printf("Allocate size too large!\n");
                  } 
                  else if (iVal_1 <= 0) {
                      printf("No Allocate Size less than or equal to 0\n");
                  } 
                  else {
                      checkAllocate = allocate(start, end, iVal_1, blockCount);
                      if (checkAllocate == 1) {
                          blockCount += 1;
                          printf("%d\n", blockCount);
                      } 
                      else {
                          printf("Cannot allocate. Not enough memory.\n");
                      }
                      start = heap;
                  }
              } 
              ///for free up block
              else if (strcmp(command, "free") == 0) {
                  if ((iVal_1 > blockCount) || (iVal_1 <= 0)) {
                      printf("Cannot free. Invalid block to free.\n");
                  } 
                  else {
                      checkAllocate = freeBlock(start, end, iVal_1);
                      if (checkAllocate > 0) {
                          printf("%d\n", checkAllocate);
                      } 
                      else {
                          printf("Cannot free. Block is not allocated in heap.\n");
                      }
                      start = heap;
                  }
              } 
              else if (strcmp(command, "printheader") == 0) {
                  if ((iVal_1 > blockCount) || (iVal_1 <= 0)) {
                      printf("Invalid block. Cannot print header.\n");
                  } 
                  else {
                      printingheader(start, end, iVal_1);
                  }
                  start = heap;
              } 
              else {
                  printf("Invalid command. Try again.\n");
              }
            }
          } 
          ////if 3 argument then check if printheap
          else if (args == 3) {
              parameters = sscanf(input, "%s %d %d", command, &iVal_1, &iVal_2);
              if (parameters == 3) {
                  //if print heap
                  if (strcmp(command, "printheap") == 0) {
                      if (iVal_1 <= 0) {
                          printf("Invalid block\n");
                      } 
                      else if ((iVal_2 > 125) || (iVal_2 < 0)) {
                          printf("Invalid number of blocks\n.");
                      } 
                      else {
                          printHeap(start, end, iVal_1, iVal_2);
                          start = heap;
                      }
                  } 
                  else {
                      printf("Invalid command. Please try again.\n");
                  }
              } 
              else {
                  printf("Invalid Parameters. Please try again.\n");
              } 
          } 
          else if (args == 4) {
              parameters = sscanf(input, "%s %d %c %d", command, &iVal_1, &charEnter, &iVal_2);
              if (parameters == 4) {
                  if (strcmp(command, "writeheap") == 0) {
                      if (iVal_1 <= 0) {
                          printf("Invalid block number to write to.\n");
                      } 
                      else if (iVal_2 < 0) {
                          printf("Invalid number of characters to write in\n");
                      } 
                      else if ((charEnter > 125) || (charEnter < 0)) {
                          printf("Character not valid.\n");
                      } 
                      else {
                          writeToBlock(start, end, charEnter, iVal_1, iVal_2);
                          start = heap;
                      }
                  } 
                  else {
                      printf("Invalid command.\n");
                  }
              } 
              else {
                  printf("Invalid command. Write Heap Command Takes 3 Input.\n");
              }
          } 
          else {
              printf("Invalid command. Please refer to the below input format\n");
              printf("allocate<blocknumber>\n");
              printf("free<blocknumber>\n");
              printf("blocklist\n");
              printf("writeheap<BlockNumber><Char><NumberOfChar>\n");
              printf("printheap<BlockNumber><NumberOfBytesToPrint>\n");
              printf("printheader<BlockNumber>\n");
              printf("quit\n\n");

          }
      } 
  }
  free(heap);
  return 0;
}

int allocate(char * ptr, char * endPtr, int aSize, int bNum) {
  
  int cSize, allocatedSize = 0;
  char allocated, cellSize;
  while((ptr < endPtr) && (allocatedSize == 0)) {
    cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);//Bitwise Right Shift
    allocated = (*(ptr + 1) & 1);
    if ((allocated == 0) && (cSize >= (aSize + 2))) {
	      *ptr = (bNum + 1);
	      if (((cSize - (aSize + 2)) > 2) || ((cSize - (aSize + 2)) == 0)) {
            cellSize = ((aSize + 1) << 1);//Bitwise Left Shift
            cellSize ^= 1;
            *(ptr + 1) = cellSize;
            cSize -= (aSize + 2);
            if (cSize != 0) {
                ptr += (aSize + 2);
		            *ptr = 0;
		            cellSize = (cSize - 1) << 1;
                *(ptr + 1) = cellSize;
	          }
	      } 
        else {
		        cellSize = (cSize - 1) << 1;
		        cellSize ^= 1;
		        *(ptr + 1) = cellSize;
		        ptr += cSize;
	      }
        allocatedSize = 1;
    } 
    else {
       ptr += cSize;
    }
  }
  return allocatedSize;
}

int freeBlock(char * ptr, char * endPtr, int blockNum) {
  int cSize, allocated, cByteNum = 0, blockFreed = 0, founded = 0;
  char cellSize;
  while ((ptr < endPtr) && (founded == 0)) {
    cByteNum = *ptr;
    cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);//Bitwise Right Shift
    allocated = (*(ptr + 1) & 1);
    if ((cByteNum == blockNum) && (allocated == 1)){
      *ptr = 0;
      cellSize = *(ptr + 1);
      cellSize ^= 1;
      *(ptr + 1) = cellSize;
      blockFreed = blockNum;
      founded = 1;
    } 
	else {
      ptr += cSize;
    }
  }
  return blockFreed;
}

//Helper Used to reduce length 
char convertToHexa(int hexNum) {
  if (hexNum == 10) {
    return 'a';
  } else if (hexNum == 11) {
    return 'b';
  } else if (hexNum == 12) {
    return 'c';
  } else if (hexNum == 13) {
    return 'd';
  } else if (hexNum == 14) {
    return 'e';
  } else if (hexNum == 15) {
    return 'f';
  }
}

//Printing the header
void printingheader(char * ptr, char * endPtr, int blockNum) {
  printf("\n");
  int cSize, cByteNum, founded = 0;
  while ((ptr < endPtr) && (founded == 0)) {
    cByteNum = *ptr; //set cByteNum 
    if (cByteNum == blockNum) { //if block num matches
      cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
      
      ////Prints the 1st byte in hexa
      int h_dig = cByteNum / 16;
      int l_dig = cByteNum % 16;
      char sizeHex [3];
      char h_val;
      char l_val;
      ////Prints the 2nd byte in hexa

      if (h_dig > 9) {
        h_val = convertToHexa(h_dig);
        if (l_dig > 9) {
          l_val = convertToHexa(l_dig);
          sprintf(sizeHex, "%c%c", h_val, l_val);
        } 
        else {
          sprintf(sizeHex, "%c%d", h_val, l_dig);
        }
      } 
      else {
        if (l_dig > 9) {
          l_val = convertToHexa(l_dig);
          sprintf(sizeHex, "%d%c", h_dig, l_val);
        } 
        else {
          sprintf(sizeHex, "%d%d", h_dig, l_dig);
        }
      }
	  printf("%s", sizeHex);

	  int h_dig2 = cSize/ 16; //for 2nd
      int l_dig2 = cSize % 16; //for 2nd
      char sizeHex2 [3]; //for 2nd 
      char h_val2; //for 2nd
      char l_val2; //for 2nd
      //For 2nd Hexa
      if (h_dig2 > 9) {
        h_val2 = convertToHexa(h_dig2);
        if (l_dig2 > 9) {
          l_val2 = convertToHexa(l_dig2);
          sprintf(sizeHex2, "%c%c", h_val2, l_val2);
        } 
        else {
          sprintf(sizeHex2, "%c%d", h_val2, l_dig2);
        }
      }
      else {
        if (l_dig2 > 9) {
          l_val2 = convertToHexa(l_dig2);
          sprintf(sizeHex2, "%d%c", h_dig2, l_val2);
        } 
        else {
          sprintf(sizeHex2, "%d%d", h_dig2, l_dig2);
        }
      }
	  printf("%s", sizeHex2);
      printf("\n");
      founded = 1;			
    } 
    else {
      cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
      ptr += cSize;
    }
  }
  if (founded == 0) {
    printf("Block not located. Can't find header\n");
  }
}

void printHeap(char * ptr, char * endPtr, int blockNum, int numBytes) {
  
  int cSize, cByteNum, founded = 0;
  if (numBytes == 0) {
    printf("\n");
  } 
  else {
    int counter = numBytes;
    int i = 0, complete = 0;
    while ((ptr < endPtr) && (founded == 0)) {
      cByteNum = *ptr;
      if (cByteNum == blockNum) {
        while ((complete == 0) && (ptr < endPtr)) {
          cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
          ptr += 2;
          if (cSize > counter) {
              while (i < counter) {
                  printf("%c", *(ptr + i));
                  i += 1;
	            }
          } 
          else {
              while (i < (cSize - 2)) {
	                printf("%c", *(ptr + i));
	                i += 1;
	            }
          }
          i = 0;
	        counter -= (cSize - 2);
          if (counter <= 0) {
            complete = 1;
          } 
          else {
            ptr += (cSize - 2);
          }
        }
        printf("\n");
        founded = 1;
      } 
      else {
        cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);//Bitwise Right Shift
        ptr += cSize;
      }
    }
    if (founded == 0) {
      printf("Could not print. Block entered was not founded\n");
    }
  }
  printf("\n");
}

void writeToBlock(char * ptr, char * endPtr, char eChar, int blockNum, int numWrites) {
  int cSize, cByteNum, founded = 0;
  int i = 0;
  while ((ptr < endPtr) && (founded == 0)) {
    cByteNum = *ptr;
    cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    if (cByteNum == blockNum) {
      if ((cSize - 2) < numWrites) {
        printf("\nWrite too big\n");
      } else {
        ptr += 2;
	while (i < numWrites) {
	  *(ptr + i) = eChar;
	  i += 1;
	}
      }
      founded = 1;
    } else {
      ptr += cSize;
    }
  }
  if (founded == 0) {
    printf("Could not write to that block. Block not allocated\n");
  }
}

int showBlockList(char * ptr, char * endPtr, int starts, int blockCount) {
  printf("\n");
  int cSize, currAddress = 0, allocated = 0;
  int prev = 0;
  int counter = 1;
  int count = 1;
  printf("Size		Allocated	Start		End\n");

  while (ptr < endPtr - 1) {
	
    cSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    allocated = (*(ptr + 1) & 1);
	
	//size
	if(cSize == 128 || cSize == 127){
		printf("%-4d ", cSize - 1);
    }
	else if (count > blockCount){
		printf("%-4d ", cSize - 1);
	}
	else
	{
		printf("%-4d ", cSize);
	}
	
	//allocated
	if (allocated == 0) {
      printf("		no        ");
    } else {
      printf("		yes       ");
    }

	//start
	if(counter == 1){
		printf("	%d", 0);
	}
	else{
		printf("	%d", prev + 1);
	}

    //end
	ptr += (cSize - 1);
	if (starts == 0 && allocated == 0){
		printf("		%d", 126);
		starts++;
	}
	else if(counter == 1){
		printf("		%d", cSize - 1);
		prev = cSize - 1;
	}
	else{
		int size = cSize + prev;
		if (size > 126){
			size = 126;
		}
		printf("		%d", size);
		prev = cSize + prev;
		
	} 
    counter--;
	count++;
	printf("\n");
    ptr += 1;
  }

  return starts;
}
