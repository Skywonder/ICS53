//ICS53 LAB3 BY Kuan-Ping Chang    71018021   Date: 2/18/2018

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


//Virtual Memory - 16 addresses (0 - 15)


//Main Memory - 8 addresses (0 - 7)


//Virtual Page 8 ...each page contains 2 address
struct VirtualPage{
	int addr1;
	int addr2;
	int priority;//determine if this is the recent or not
	int empty;
};

//Main Memory Page 4 ... each page contains 2 address
struct MainPage{
	int addr1;
	int addr2;
	int priority;//determine if this is the recent or not
	int empty;
};

//PTable - Stores the page info
struct PTable{
	int valid;
	int dirty;
	int pageNum;
};

struct VirtualPage vpage[8]; //access by vpage[idx].addr1 or vpage[idx].addr2
struct MainPage mpage[4]; //access by mpage[idx].addr1 or mpage[idx].addr2
struct PTable ptable[8]; //contains 8 virtual page info



int main(int argc, char* argv[]){
	//recent and old are both 0 at beginning
	int recent_accessed_main = 0;//store the most recent accessed address location [idx]
	int recent_accessed_virtual = 0;
	int old_main_location = 0; //store the oldest main address location [idx]
	int old_vr_location = 0;
	int i;
	int looping = 1;//for infinite while loop before quit is entered
	
	//initialize virtual page and main pages to -1
	for(i = 0; i < 8; i++){
		vpage[i].addr1 = -1;
		vpage[i].addr2 = -1;
		vpage[i].priority = 0;
		vpage[i].empty = 0;

	}
	
	for(i = 0; i < 4; i++){
		mpage[i].addr1 = -1;
		mpage[i].addr2 = -1;
		mpage[i].priority = 0;
		mpage[i].empty = 0;//should be zero at start to indicate that its empty 
	}
	
	//initialize ptable
	int z = 0;
	for(z; z < 8; z++){
		ptable[z].valid = 0;
		ptable[z].dirty = 0;
		ptable[z].pageNum = z; //because valid = 0
				
	}
		
	
	
	//command loop 
	while(looping == 1){
		char* store[3];//for holding the number of inputs...at most 3  ...at idx{0,1,2}
		int str;
		for(str = 0; str < 3; ++str){ //used to reset store
			store[str] = NULL;
		}
		 //prompt and input
		char input[20];//this is used to store input value
		printf("$");
		fgets(input,sizeof(input),stdin);
		if(feof(stdin)){
			exit(0);
		}
		if(strlen(input) < 2)
			continue;
		char* pos;
		if((pos = strchr(input, '\n'))!= NULL){
			*pos = '\0'; //end of line
		}
		char *tok = strtok(input, " ");
		int j = 0;
		while(tok != NULL){
			store[j] = tok;
			tok = strtok(NULL, " ");
			++j;
		}
		store[j] = NULL;
		/*
		Check for command
		*/
		//quit
		if(strcmp(store[0], "quit") == 0){
			looping = 0;
		}
		//read<address> -- read the value stored in the main
		//First check to see the read address in ptable 
		
		else if(strcmp(store[0], "read") == 0 && store[1] != NULL){
			if(atoi(store[1]) >= 0 && atoi(store[1]) < 16){
				//read address at location ... say 16....16/2 = 8 address 2
				int page_location = atoi(store[1])/2; //page_location to check with given address 
				int address_inpage = atoi(store[1])%2; //address in page to look for
				int actual_location = ptable[page_location].pageNum;
				int isValid = ptable[page_location].valid;//checking if its in main(1) or desk(0);
				//printf("page location: %d\n", page_location);
				//vpage[page_location].priority = 0; //this would prevent the page we are reading now be replaced if we are moving main
				
				int temp1 = vpage[page_location].addr1;//holding the value for vpage
				int temp2 = vpage[page_location].addr2;//holding the value for vpage
				int no_space = 0;
				

				if(isValid == 1){//if already in main just read it....
					int y = 0;
					for(y; y < 4; y++){
						if(y == actual_location){
							//printf("Already in Main\n");
							mpage[y].empty = 1;
							recent_accessed_main = y;
							if(address_inpage == 0){
								printf("Address [%d] has value: %d\n", atoi(store[1]), mpage[y].addr1);
							}
							else{
								printf("Address [%d] has value: %d\n", atoi(store[1]), mpage[y].addr2);
							}
						}
					}
				}
				else 
				{ 
					int v = 0; //for this loop
					int temp3 = 0;
					int temp4 = 0;
					for(v; v < 4; v++){ 
						if(mpage[v].addr1 == -1 && mpage[v].addr2 == -1 && mpage[v].empty == 0){//meaning its empty at addr1 to start with...
							//printf("Found space \n");
							mpage[v].empty = 1;

							ptable[page_location].pageNum = v;//set page number
							ptable[page_location].dirty = 0;
							ptable[page_location].valid = 1; //we are in read zone now...

							no_space = 0;//space exist
							
							temp3 = mpage[v].addr1; 
							temp4 = mpage[v].addr2;

							mpage[v].addr1 = vpage[page_location].addr1; //take in the address
							mpage[v].addr2 = vpage[page_location].addr2;

							vpage[page_location].addr1 = temp3;
							vpage[page_location].addr2 = temp4;
							
							recent_accessed_main = v;
							recent_accessed_virtual = page_location;

							if(address_inpage == 0){
								printf("Address [%d] has value: %d\n", atoi(store[1]), mpage[v].addr1);
							}
							else{
								printf("Address [%d] has value:	%d\n", atoi(store[1]), mpage[v].addr2);
							}
							break;//after filling break it
						}
						else{//??? not sure about this
							no_space = 1;
						}
					}

					///if all main pages are filled ... move it to oldest disk memory or over empty disk first before fillingin
					if(no_space == 1)//all main page spaces are full (page has no space)
					{
						//printf("NO SPACE FOR READ\n");
						//move physical mem with least access to virtual mem (desk) 
						int o = 0;
						int high = 0;
						int qhigh = 0;
						int location_to_move;
						int qlocation_to_move;
						int vr_no_space = 0;
						
						
						//find the main page to move to desk page
						for(o; o < 4; o++){
							//printf("%d priority %d\n", o, mpage[o].priority);
							if(mpage[o].priority > high){

								high = mpage[o].priority; //this is the new high
								location_to_move = o; // o is the page that is moved
								
							}
						}
						//printf("location of page in main %d\n", location_to_move);

						int tt = 0;
						int true_location;
						for(tt; tt < 8; tt++){
							if (ptable[tt].dirty == 1 && ptable[tt].valid == 1 && ptable[tt].pageNum == location_to_move){
								true_location = tt;
								//break;
							}
						}
						//printf("location of original main %d\n", true_location);

						int qq = 0;
						for(qq; qq < 8; qq++){
							if(qq == true_location){ //if this spot is empty...store the main memory into it
								//printf("Found empty in vpage %d\n", qq);

								//first the page location we are writing is set to valid + dirty ..this is the 
								ptable[page_location].valid = 1;
								ptable[page_location].dirty = 1;
								ptable[page_location].pageNum = location_to_move;//should be in main

								//then we will update the ptable for both the main and disk location...
								ptable[qq].valid = 0;
								ptable[qq].dirty = 0; 
								ptable[qq].pageNum = qq; //this is for the DISK 


								//Save the value originally held at main 
								vpage[qq].addr1 = mpage[location_to_move].addr1;
								vpage[qq].addr2 = mpage[location_to_move].addr2;
							
								recent_accessed_virtual = qq;

								//update the value from the old disk 
								mpage[location_to_move].addr1 = temp1;
								mpage[location_to_move].addr2 = temp2;
							
								recent_accessed_main = location_to_move;
								
								break;
							}
						}

						if(address_inpage == 0){
				
							printf("Address [%d] has value: %d\n", atoi(store[1]), mpage[location_to_move].addr1);

						}
						else{
	
							printf("Address [%d] has value: %d\n", atoi(store[1]), mpage[location_to_move].addr2);

						}
						
					}

				}

				int r = 0;
				for(r; r < 4; r++){
					if(r == recent_accessed_main){
						mpage[r].priority = 0;
					}
					else{
						mpage[r].priority++; //increase by 1 means lower priority
					}
				}
				int qrq = 0;
				for(qrq; qrq < 8; qrq++){
					if(qrq == recent_accessed_virtual){
						vpage[qrq].priority = 0;
					}
					else{
						vpage[qrq].priority++;
					}
				}

			}
			else{
				printf("Input address or value must be positive number less than 15 (inclusive) \n");
			}
		}
		
		//write<address><num> -- writes data to a memory location (main)
		else if(strcmp(store[0],"write") == 0 && store[1] != NULL && store[2] != NULL){
			if(atoi(store[1]) >= 0 && atoi(store[1]) < 16 && atoi(store[2]) >= 0){
				

				int page_location = atoi(store[1])/2; // 0/2 = 0, 1/2 = 0, 2/2 = 1, 3/2 = 1...
				int address_inpage = atoi(store[1])%2; // either 0 or 1 ...
				//find address to fill	
				int v_page_entry = ptable[page_location].valid; //check for value
				int no_space = 0;

				//temp hold the virtual value
				int temp1 = vpage[page_location].addr1; //9
				int temp2 = vpage[page_location].addr2; //99


				int page_num = ptable[page_location].pageNum; //location of the page in ptable
				//printf("page location: %d\n", page_location);
			

				if(v_page_entry == 0){ //if address we want to write is not in main...then lets move it to main first 
					//check for available spot that can store the page
					//printf("Write target Not in main\n");
					int v = 0; //for this loop
					for(v; v < 4; v++){ //because we only have 4 page ... and we have no pn location on main
						//printf("No_Space %d\n", no_space);
						if(mpage[v].addr1 == -1 && mpage[v].addr2 == -1 && mpage[v].empty == 0){ //if address value are -1 or empty...
							mpage[v].empty = 1;
							//printf("Found Empty\n");
							no_space = 0;
				
							mpage[v].addr1 = temp1;
							mpage[v].addr2 = temp2;
							
							ptable[page_location].dirty = 1;
							ptable[page_location].valid = 1;
							ptable[page_location].pageNum = v;
							//now overwrite this
							recent_accessed_main = v;
							recent_accessed_virtual = page_location;

							if(address_inpage == 0){
								if(mpage[v].addr1 != -1){
									ptable[page_location].dirty = 1;
								}
								mpage[v].addr1 = atoi(store[2]); 
							}
							else{
								if(mpage[v].addr2 != -1){
									ptable[page_location].dirty = 1;
								}
								mpage[v].addr2 = atoi(store[2]);
							}
							//
							break;
						}
						else //every page is used
						{
							no_space = 1;
						}
					}
					
					//no_space = 1;
					if(no_space == 1)//all main page spaces are full (page has no space).....
					{
						//printf("WRITE NO SPACE\n");
						//move physical mem with least access to virtual mem (desk) 
						int o = 0;
						int high = 0;
						int qhigh = 0;
						int location_to_move;
						int qlocation_to_move;
						int vr_no_space = 0;
						
						ptable[page_location].valid = 1; //since we are moving it to Main - here page location is the virtual one

						if(temp1 != -1 && temp2 != -1){
							ptable[page_location].dirty = 0;
						}
						else{
							ptable[page_location].dirty = 1;
						}


						//find the main page to move to desk page
						for(o; o < 4; o++){ 
							//printf("priority %d \n", mpage[o].priority);
							if(mpage[o].priority > high){
								high = mpage[o].priority; //this is the new high
								location_to_move = o; // o is the page that is moved
							}	
						}
						//printf("location of page in main %d\n", location_to_move); //the location of the data to save 
						
						int tt = 0;
						int true_location;
						for(tt; tt < 8; tt++){
							//printf("dirty %d valid %d pagenum %d \n", ptable[tt].dirty, ptable[tt].valid, ptable[tt].pageNum );
							if (ptable[tt].valid == 1 && ptable[tt].pageNum == location_to_move && ptable[tt].pageNum == location_to_move){ //find a place that is not the same as the main...
								//printf("FOUND TRUE LOCATION");
								true_location = tt; //the location of the mpage in ptable
							}
						}
						//printf("location of main in ptable %d\n", true_location);

						int qq = 0;
						for(qq; qq < 8; qq++){
							if(qq == true_location){ //if this spot is empty...store the main memory into it
								//printf("page location %d\n", page_location);
								//printf("Found empty in vpage %d\n", qq);
								
								//the page to move to main disk 
								ptable[page_location].valid = 1;
								ptable[page_location].dirty = 1;
								ptable[page_location].pageNum = location_to_move;//should be in main
								
								//then we will update the ptable for both the main and disk location...
								
								ptable[qq].valid = 0;
								ptable[qq].dirty = 0; 
								ptable[qq].pageNum = qq; //this is for the DISK 

								//save the value at main? 
								vpage[qq].addr1 = mpage[location_to_move].addr1;
							    vpage[qq].addr2 = mpage[location_to_move].addr2;
								
								recent_accessed_virtual = qq;

								//update the value from the old disk 
								mpage[location_to_move].addr1 = temp1;
								mpage[location_to_move].addr2 = temp2;

								recent_accessed_main = location_to_move;
								
								break;
							}
						}
						
						if(address_inpage == 0){
							if(mpage[location_to_move].addr1 != -1){
								ptable[location_to_move].dirty = 1;
							}
							mpage[location_to_move].addr1 = atoi(store[2]); //store to address in location pn
							//set new page number
						}
						else{
							if(mpage[location_to_move].addr1 != -1){
								ptable[location_to_move].dirty = 1;
							}
							//access the main location directly
							mpage[location_to_move].addr2 = atoi(store[2]);
							
							//set new page number
						}
						
					}
				
					
					
				}
				else{ 	//if already in main and the address written is not occupied
					//check for the ptable for pn 					
					//printf("Already in Main\n");

					ptable[page_location].dirty = 1; //since we are overwriting directly in main
					//printf("goes here because already in main\n");
					ptable[page_location].valid = 1;
					int pn = ptable[page_location].pageNum;
					mpage[pn].priority = 0;
					mpage[pn].empty = 1;
					recent_accessed_main = pn;
					if(address_inpage == 0){
						//access the main location directly 
						mpage[pn].addr1 = atoi(store[2]); //store to address in location pn
					}
					else{
						//access the main location directly
						mpage[pn].addr2 = atoi(store[2]);
					}
					
				}

				int r = 0;
				for(r; r < 4; r++){
					if(r == recent_accessed_main){
						mpage[r].priority = 0;
					}
					else{
						mpage[r].priority++; //increase by 1 means lower priority
					}
				}

				int qrq = 0;
				for(qrq; qrq < 8; qrq++){
					if(qrq == recent_accessed_virtual){
						vpage[qrq].priority = 0;
					}
					else{
						vpage[qrq].priority++;
					}
				}

			}
			else{
				printf("Input address or value must be positive number less than 15 (inclusive) \n");
			}
		}
		
		//showmain<ppn> -- this should be fine (done) -- holds 8 address slot
		else if(strcmp(store[0], "showmain") == 0 && store[1] != NULL){
			if(atoi(store[1]) >= 0 && atoi(store[1]) < 4){
				//show main address(s) at page location 
				int address0 = atoi(store[1]) * 2 + 0; //if page #7 ... 7 * 2 + 0 = 14 //first address 
				int address1 = atoi(store[1]) * 2 + 1; //if page #7 ... 7 * 2 + 1 = 15 //second address
				printf("Address   Contents\n");
				printf("%d        %d\n", address0, mpage[atoi(store[1])].addr1);
				printf("%d        %d\n", address1, mpage[atoi(store[1])].addr2);
				
			}
			else{
				printf("Input page address must be in range [0, 4]\n");
			}
		}
		//showdisk<dpn> -- this should be fine (done)
		else if(strcmp(store[0], "showdisk") == 0 && store[1] != NULL){
			if(atoi(store[1]) >= 0 && atoi(store[1]) < 8){
				//show disk page address(s) at page location 
				int address0 = atoi(store[1]) * 2 + 0; //if page #7 ... 7 * 2 + 0 = 14 //first address 
				int address1 = atoi(store[1]) * 2 + 1; //if page #7 ... 7 * 2 + 1 = 15 //second address
				printf("Address   Contents\n");
				printf("%d        %d\n", address0, vpage[atoi(store[1])].addr1);
				printf("%d        %d\n", address1, vpage[atoi(store[1])].addr2);
				
			}
			else{
				printf("Input page address must be in range [0, 7]\n");
			}
			
		}
		
		//showptable 
		else if(strcmp(store[0], "showptable") == 0){
			//show page table in virtual 
			printf("VPageNum	Valid		Dirty		PN\n");
			int g = 0;
			for(g;g < 8; g++){ //number of pages
				//VPageNum would be g idx
				printf("%d		", g);
				printf("%d		", ptable[g].valid);
				printf("%d		", ptable[g].dirty);
				printf("%d\n", ptable[g].pageNum);
			}
		}

		else{
			printf("Error: invalid block number or Invalid command\n");
			printf("Available commands:\n ");
			printf("read<address>\nwrite<address><num>\nshowdisk<dpn>\nshowmain<ppn>\nshowptable\nquit\n");
		}


	}
	
	return 0;
}


