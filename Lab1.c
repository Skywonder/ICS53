//BY: Kuan-Ping Chang 71018021 

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//1. accepts the files and read them

struct Offers{
  char class[20]; //holds class name
  int quarter; //the quarter of which it host
};

struct Preqr{
  char class[20]; //the class name (first string /0 in the line
  //char prerequisites[200];//holds the prerequisits 
};

struct Requirement{
  char class[20];
};

struct ClassTake{
  char class[20];
};

struct ClassTook{
  char class[20];
};

struct ptrPrint{
  char class[20];
};

struct Prerequisites{
  char class[20];
};

struct OldData{
  char class[20];
};

//global value
struct Offers odata[100];//store all the offer data...offer up to 100 classes
struct Preqr pdata[100][100];//store all the pdata...preq for up to 100 classes
struct Requirement rdata[100];
struct ClassTake cdata[100];
struct ClassTook tdata[100];
struct ptrPrint prtdata[100];
struct Prerequisites ppdata[100];
struct OldData otdata[100];

////////////

int check_prereq(char*, struct Preqr(*)[], int);
int counter = 0;

int main(int argc, char* argv[]){
  printf("You have entered %d arguments\n", argc);
  int i;
  FILE *fp;
  int otcounter = 0;
  const char *Offer = argv[1];
  const char *Preq = argv[2];
  const char *Req = argv[3];
  
  FILE *file_Offer = fopen(Offer, "r");
  FILE *file_Preq = fopen(Preq, "r");
  FILE *file_Req = fopen(Req, "r");

  size_t len;
  char line[256];
  char pline[256];
  char oline[256];
  char buffer[1024];
  char prerequisite[100];//up to 100 chara
  char end[5]= "\n";
  if(argc > 1){
    int i;
    for(i=0;i<argc;i++){
      printf("Arg: %s\n", argv[i]);
    }
  }
  else{
    printf("Input File does not Exist or Missing");
  }
  //open file whose path is passed as an argument
  // fp = fopen(argv[3],"r");//open requirement
 
  ////////////OFFER ARRAY ASSIGNEMNT/////////////////DONE -DO NOT MODIFY///////////////////////////////////////////
  
  int j = 0;
  int opos = 0;

  if(file_Offer == NULL){
    printf("\n Could not open offer file");

  }
  else{
    // printf("\nFile(%s) opened\n", Offer);
    while(fgets(line, sizeof(line), file_Offer)!=NULL){
      char *otokenPtr;
      otokenPtr = strtok(line, " ");
      while(otokenPtr){
	otokenPtr[strcspn(otokenPtr, "\r\n")] = '\0';
	if(j == 0){
	  strcpy(odata[opos].class, otokenPtr);
	  //printf("Adding %s ", otokenPtr);
	}
	if(j > 0){//this should be a int value
	  
	  odata[opos].quarter = atoi(otokenPtr);
	  //printf("Adding Quarter %d\n ", atoi(otokenPtr));
	}
	otokenPtr =strtok(NULL, " ");
	j = 1;
      }
      j = 0;
      opos++;
    }
  }
  //opos++;
  /*
  int k = 0;
  for(k; k < opos && odata[k].class != end; k++){
    printf("Class Offer: %s ", odata[k].class);
    printf("Quarter: %d\n ", odata[k].quarter);
  }
  */
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


  ////////////PREREQUISITE ARRAY ASSIGNMENT////////////////DONE - DO NOT MODIFY//////////////////////////////////////////////////
  //storing data for prerequisite into array
 
  //int p = 0; //p counter
  int ppos = 0;
  //int inc = 0;
  char temp[200];
  int pinc = 0;
  if(file_Preq == NULL){
    printf("\n Could not open preq file");
  }
  else{
    //printf("\nFile(%s)opened\n", Preq);
    while(fgets(line, sizeof(line), file_Preq)!=NULL){ //this is going to read all the lines?
      char *tokenPtr;
      tokenPtr = strtok(line, " ");//grabbing the character
      while(tokenPtr){//while there is something
	tokenPtr[strcspn(tokenPtr, "\r\n")]='\0';
	if (pinc == 0){//if currect p = 0;
	  strcpy(pdata[ppos][pinc].class, tokenPtr);//store the tokenPtr in class
	  //printf("Adding %s", tokenPtr);
	}
	if (pinc > 0){ //move to prerequisites 
	 
	  // if (inc == 0){
	    strcpy(pdata[ppos][pinc].class, tokenPtr);
	   
	  }
		
	tokenPtr = strtok(NULL, " ");//go to next word
	pinc++;
      } 
      pinc = 0;
      ppos++;
    }
  }
  /*
int g = 0;
int o = 1;
for(g; g < ppos ;g++){
      printf("Class: %s ", pdata[g][0].class);
      for(o; o < ppos; o++){

        if(strlen(pdata[g][o].class) == 0)
        {
          break;
        }
        printf("Prerequisite: %s\n", pdata[g][o].class);
      }
      o = 1;
 }
  */

 
////////////////////////////////////////DONE- DO NOT MODIFY//////////////////////////////////////////


  int rpos = 0;
  //storing data to array for requirement
 if(file_Req ==NULL){
    printf("\n Could not open file");
  }
  else{
    //printf("\nFile(%s)opened\n", Req);
    //get lines and split by space
    while(fgets(line, sizeof(line), file_Req)!= NULL){
      char* val = strtok(line, " ");//grabbing the character

      //so we have done reading for this char already...move on...
      while(val){ //if this stop the result in the dictionary should be finished
	//printf("%s\n", val); //instead of print lets add the value in 
	val[strcspn(val, "\r\n")] = '\0';
	strcpy(rdata[rpos].class, val);
	rpos++;
	val = strtok(NULL, " ");
      }      
    }
  }
 // int r = 0;
 //for(r; r < rpos; r++){
 //   printf("Class: %s\n ", rdata[r].class);
 //}


 
 /////////////////////////////COURSE PLAN LOOP//////////////////////////
 //int year = 1;//use for increment year timer
 //int quarter = 1; //use to increment quarter...increase yr if quarter is == 3
 int size_requirement = rpos;//get the size requirement
 int r = 0; //for requirement
 // int g = 0; //for preq
 //int k = 0; //for class offer
 int c = 0; //for cdata 
 // int o = 1; 


 for(r; r < rpos;r++){ //for from the first rpos data
   rdata[r].class[strcspn(rdata[r].class, "\r\n")] = '\0';
   //printf("Start from %s\n", rdata[r].class);
   //now lets look for the prereqs
   check_prereq(rdata[r].class, pdata, ppos);//this should be getting all the preq for that requirement and store them to a global 
   
   strcpy(cdata[counter].class, rdata[r].class);
   counter++;
 }

 //remove duplicates

int e = 0;
int h = 0;
int b = 0;
int size = counter;

//printf("Total Courses Found: %d\n", counter);
for(e; e < size; e++){
  for(h = e + 1; h < size; h++){
    //printf("Courses check: %s\n", cdata[e].class);
     if(strcmp(cdata[e].class, cdata[h].class) ==0){
       for(b=h;b<size;b++){
	 strcpy(cdata[b].class, cdata[b+1].class);
	 //printf("found duplicate: %s\n", cdata[b].class);
       }
       size--;
       h--;
     }
  }
}
//print required courses (absolutely needed ones) 
/*
int l = 0;
printf("Total Courses Found: %d\n", size);
for(l; l< size; l++){
  printf("Classes Needed: %s\n", cdata[l].class);
   
}
*/

//////////GO THROUGH THE YRS NOW////////////



int tcounter = 0;
int scounter = size;
int year = 1;
int quarter = 1;
while(scounter > 0){
  int k = 0; //for cdata max: opos
  int u = 0; //for odata max: size
  int v = 0; //also for checking pdata
  int p = 0; //for pdata max: ppos
  int q = 1; //for pdata 2d array
  int boolc = 0; //for true or false 
  int hasp = 0;//for checking if there is preq
  int casp = 0;//for checking if casp exist already 
  int t = 0;
  int res = 0;
  int pcounter = 0;
  int ppcounter = 0;
  int same = 0;
  // while(scounter> 0){
  for(k; k < size; k++){ //for each requirement
    // printf("Start at %d\n", k);
    //printf("start at %s\n", cdata[k].class);
    if(tcounter != 0){
      t = 0;
      same = 0;
      for(t; t < tcounter; t++){
	if(strcmp(cdata[k].class,tdata[t].class)==0){
	  same = 1;
	  break;
	}
      }
      if(same == 1){
	continue;//skip below move up in k ...this will repeat until its ok to move on
      }
    }
    // printf("This one is ok %s\n", cdata[k].class);
    
    for(u; u< opos; u++){ //for each offer
      //printf("get here at least?\n");
      //printf("compare class %s with class %s and quarter %d with offer quarter %d\n", odata[u].class, cdata[k].class, quarter, odata[u].quarter);
      odata[u].class[strcspn(odata[u].class, "\r\n")]='\0';
      cdata[k].class[strcspn(cdata[k].class, "\r\n")]='\0';
      
      //check offer quarter
      if((strcmp(odata[u].class,cdata[k].class)==0) && (quarter == odata[u].quarter) ){
	//printf("examin %s\n", cdata[k].class); 
	//printf("odata match cdata and quarter\n");
	//check to see if requirement has prereq if it does it will show up in the pdata class name
	
	v = 0;
	hasp = 0;
	for(v; v < ppos; v++){ 
	  // printf("check pre-requirement\n");
	  pdata[v][0].class[strcspn(pdata[v][0].class, "\r\n")]='\0';
	  cdata[k].class[strcspn(cdata[k].class, "\r\n")]='\0';

	  //printf("compare %s and %s\n", pdata[v][0].class, cdata[k].class);
	  if(strcmp(pdata[v][0].class, cdata[k].class)== 0){// && strlen(pdata[v][1].class)!= 0){ //if class name exist
	    //printf("found name with prerequisite %s\n", pdata[v][0].class);
	    // printf("Find Prereq for this: %s\n", cdata[k].class);
	    //printf("pass here\n");
	    //ppcounter = 0;
	     q = 1;
	    for(q; q < ppos; q++){ //lets grab all the prereqs
	      if(strlen(pdata[v][q].class)!= 0){ //as long as this length isnt 0?
		//printf("Add to be queue %s\n", pdata[v][q].class);
		hasp = 1;
		strcpy(ppdata[ppcounter].class, pdata[v][q].class);
		//printf("class %s\n", pdata[v][q].class);
		//printf("counter %d\n", ppcounter);
		ppcounter++;
	      }
	      else{
		//printf("break here inner\n");
		break;
	      }
	      //printf("%d\n", ppcounter);
	    }
	    //printf("break here outter\n");
	   
	    break;
	  }
	  
	}

	if(strlen(ppdata[0].class)==0){
	  hasp = 0;
	}


	//mod here
	//v = 0;
	//printf("hasp %d\n", hasp);
	//printf("ppcounter %d\n ", ppcounter);
	if(hasp == 1){//if has requirement(there must be requirement)
	  //printf("goes here");
	  int checkerc = 0;
	  casp = 0; 
	  //going through while checker -- if everything passed then it should be casp = 0 otherwise casp = 1
	    if(strlen(otdata[0].class) != 0){
	      // printf("enter here");
	      for(p; p < ppcounter;p++){
		t = 0;
		for(t; t < otcounter; t++){//change to use otcounter
		  // printf("tdata value %s comapre ppdata value %s\n",tdata[t].class, ppdata[p].class);
		  
		    
		  if(strcmp(otdata[t].class, ppdata[p].class)==0){//change to use otdata
		      //if the number of match == ppcounter then make it pass
		    // printf("match found in tdata %s and pdata %s\n", tdata[t].class, ppdata[p].class);
		      checkerc++;
		      // printf("checker value: %d\n", checkerc);
		    
		    }
		  
		}
	      }
	      p = 0;
	      //printf("checker value %d and ppcounter value %d\n", checkerc, ppcounter);
	      
	      if (checkerc == ppcounter){
		//printf("casp = 0\n");
		casp = 0;
		ppcounter = 0;
	      }
	      else{
		//printf("casp = 1\n");
		casp = 1;
		ppcounter = 0;
	      }
	    }
	    else{
	      break;
	    }//try
	    //	}//try 

	    // printf("casp = %d exist while loop\n", casp);
	    if(strlen(tdata[0].class)!= 0){
	      if(casp == 0){//since all prerequisites are found we can add it to tdata
		//printf("value found 1\n");
		strcpy(tdata[tcounter].class, cdata[k].class);
		  //add to print array
		strcpy(prtdata[pcounter].class, cdata[k].class);
		//printf("Add to tdata in 1: %s\n", tdata[tcounter].class);
		pcounter++;
		tcounter++;
		scounter--;
		break;
	      }
	    }
	  }
	//printf("hasp %d\n",hasp);
	if(hasp == 0) //if no requirement
	{
	  // printf("enter hasp 0");
	  // printf("No requirement\n");
	  //just check to see if its already in the class taken
	  t = 0;
	  for(t; t < tcounter; t++){
	    if(strcmp(tdata[t].class, cdata[k].class)==0){//check to see if cdata is already in tdata
	      casp = 1;
	      // printf("value exist 2\n");
	    }
	    else{
	      casp = 0;//mean cdata is not in tdata
	    }
	  }
	  if(casp == 0){
	    // printf("value found 2\n");
	    //printf("Added %s in 0\n", cdata[k].class);
	    strcpy(tdata[tcounter].class, cdata[k].class);
	    strcpy(prtdata[pcounter].class, cdata[k].class);
	    pcounter++;
	    tcounter++;
	    scounter--;
	    //ppcounter = 0;
	    break;
	  //added
	  }
	}
      }
      //printf("arrive here? 4");
    }
    //set u = 0
    //v = 0;
    u = 0;
  }
  // v = 0;  
  if(pcounter != 0){
      // printf("reporting with %d value\n", pcounter);
      int ppp = pcounter;
      printf("%d %d ", year, quarter);//print year and quarter first
      for(res; res < pcounter; res++){
	//printf("get here/n");
	strcpy(otdata[otcounter].class, prtdata[res].class);//keep track of old data...added 1/21/2018
	printf("%s ", prtdata[res].class);//print the class
	ppp++;
	otcounter++;
	//empty the counter...
	//strcpy(prtdata[res].class, '\0');//empty it
       
	//pcounter--;//empty the counter
      }
      printf("\n");
      //printf("arrive here\n");
      memset(&prtdata[0],0,sizeof(prtdata)); //delete the array 
      pcounter = pcounter - ppp;
      quarter++;
      if(quarter == 4){
	quarter = 1;
	year++;
      }
  }
  else if (pcounter == 0){
    quarter++;
    if(quarter == 4){
      quarter = 1;
      year++;
    }
  }


 }//end of while   

 fclose(file_Offer);
 fclose(file_Preq);
 fclose(file_Req);
 
 ///////////////////////////////////////////////////////////////////////
 return 0; //end
}

int check_prereq(char* class, struct Preqr pdata[100][100], int ppos){
  int g = 0;
  int o = 1;
 
  // printf("PN: %d\n", ppos);
  //printf("PC: %s\n", pdata[0][1].class);
  for(g; g < ppos; g++){
    //printf("PClass: %s\n",pdata[g][0].class);
  
    if(strcmp(pdata[g][0].class,class)==0){
 
      for(o; o < ppos; o++){
	if(strlen(pdata[g][o].class)==0){
	  break;
	}
	//printf("PPreq: %s\n",pdata[g][o].class);
	//printf("Store Data to Array\n");
	strcpy(cdata[counter].class, pdata[g][o].class);
	counter++;
	//take this and recursively move backward
	check_prereq(pdata[g][o].class, pdata, ppos);
      }
    }
    o = 1;
  }
}

