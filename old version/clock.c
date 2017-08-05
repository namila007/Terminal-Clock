/*
	CO222:Project 1 - Terminal clock
	Author Namila Bandara
 	E/14/084
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ROW_SIZE 	5				//declaring constants
#define COL_SIZE 	6
#define LEVEL_SIZE 11
#define RESET 		0
#define BRIGHT 		1
#define COLOURSIZES 8
#define ZERO		0
#define BACKGROUND  0

/* prototype functions 
*/
void timedisplay();			
void cursorjump();
void cursorjumpcoloum(int printcount);
void digitprint(int numprint,int printcount);
void timesetdigits(int hour,int min,int sec);
void displaydigit(int timedigits,int printcount);
void textcolor(int attr,int fg,int bg);
int colourchecker(char** argv,int argcount);
void displaydays(int year,int mont,int day); 
void errorprint(int argc, char  **argv);


/* Declaring global variables 
*/

const int Disp_array[LEVEL_SIZE][ROW_SIZE][COL_SIZE]={ //3d array for numbers 0-9 and symbol ':'
		{{1,1,1,1,1,1},{1,1,0,0,1,1},{1,1,0,0,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1}},  //Number 0
		{{0,0,0,0,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1}},	//Number 1
		{{1,1,1,1,1,1},{0,0,0,0,1,1},{1,1,1,1,1,1},{1,1,0,0,0,0},{1,1,1,1,1,1}},	//2
		{{1,1,1,1,1,1},{0,0,0,0,1,1},{1,1,1,1,1,1},{0,0,0,0,1,1},{1,1,1,1,1,1}},	//3
		{{1,1,0,0,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1}},	//4
		{{1,1,1,1,1,1},{1,1,0,0,0,0},{1,1,1,1,1,1},{0,0,0,0,1,1},{1,1,1,1,1,1}},	//5
		{{1,1,1,1,1,1},{1,1,0,0,0,0},{1,1,1,1,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1}},	//6
		{{1,1,1,1,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1},{0,0,0,0,1,1}},	//7
		{{1,1,1,1,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1}},	//8
		{{1,1,1,1,1,1},{1,1,0,0,1,1},{1,1,1,1,1,1},{0,0,0,0,1,1},{1,1,1,1,1,1}},	//9
		{{0,0,0,0,0,0},{0,0,1,1,0,0},{0,0,0,0,0,0},{0,0,1,1,0,0},{0,0,0,0,0,0}},	// symbol ':'
	};
char *coloursavailable[]={"black","red","green","yellow","blue","magenta","cyan","white"};  //colours available
char colour='C';
char help='h';
int forecolour=7;




void textcolor(int attr,int fg,int bg)
{	char command[13];

	/* Command is the control command to the terminal */

	/* textcolor(BRIGHT,BLACK,WHITE) will have characters printed in
	black in white background */
	if(attr==ZERO){
		//reseting the command line to original text colour
		sprintf(command,"\e[0;0;0m");
		printf("%s",command);
		printf("\n");

	}else {
	if(forecolour==ZERO){
		//setting text to black colour below function cant do that as black ANSI=016m
		sprintf(command,"\e[38;5;016m");
		printf("%s",command);
	}else{
		
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
	}
	}
}



void timedisplay(){
	/*
	function for get values from the system time day month and year
	*/
	int hour,min,sec,day,mont,year;
	time_t settime;
	struct tm* timevals;
	time(&settime);

	timevals=localtime(&settime);
	hour=timevals->tm_hour;
	min=timevals->tm_min;
	sec=timevals->tm_sec;
	day=timevals->tm_mday;
	mont=timevals->tm_mon;
	year=timevals->tm_year;

	timesetdigits(hour,min,sec);
	displaydays(year,mont,day);
}

void displaydays(int year,int mont,int day){
	/*
	printing year day and month in the terminal
	*/
	
	printf("\033[6B\033[33D" );
	
	textcolor(BRIGHT,forecolour,BACKGROUND);
	
	printf("%d-%02d-%02d\n",1900+year,mont+1,day);
	textcolor(RESET,forecolour,BACKGROUND);
	printf("\n");
}


void timesetdigits(int hour,int min,int sec)
{
	/*
	setting the digits to deplay and ':' symbol between hour ,min and sec
	*/
 	int printcount=0,dotsign=10;;
 	displaydigit(hour,printcount);
 	digitprint(dotsign,printcount+2);
 	cursorjump();
 	printcount=printcount+3;
	displaydigit(min,printcount);
 	digitprint(dotsign,printcount+2);
 	cursorjump();
 	printcount=printcount+3;
 	displaydigit(sec,printcount);
 }

void displaydigit(int timedigits,int printcount){
	/*
	getting two digits from timedigits and print them in terminal
	*/
	int digit1,digit2;

	digit1=timedigits/10;
	digit2=timedigits%10;

	digitprint(digit1,printcount);
	cursorjump();
	digitprint(digit2,printcount+1);
	cursorjump();
}
	



void digitprint(int numprint,int printcount){
	/*
	function to print digits 5x6 matrix size
	*/

	int row,col;
	for (row = 0; row <ROW_SIZE; ++row)
	{
		for (col = 0; col <COL_SIZE; ++col)
		{
			if(Disp_array[numprint][row][col]==1){
				textcolor(BRIGHT,BACKGROUND,forecolour);  //canging colours
				printf(" ");
				textcolor(BRIGHT,forecolour,BACKGROUND);
			}else{
				printf(" ");
			}
		}
		printf("\n");
		
			cursorjumpcoloum(printcount);
		
		
	}

}



void cursorjumpcoloum(int printcount){
	/*
	function to jump the coursor in 7*n+1 column length vertical
	*/
	if(printcount!=0 ){
		printf("\033[%dC",1+7*printcount);
	
	}else{
		printf("\033[1C" );
	}
	
}


void cursorjump(){
	/*
	function to jump the cursor 5 lines top and 7 column right 
	*/
	printf("\033[5A\033[7C");

}




int colourchecker(char **argv,int argcount){
	/*
	function to check whether input colour is available or not,if available return colour value.
	 if there is no colour inputs it print error message how to use colours
	*/
	int count;
	char *colourset=argv[argcount]; //asigning the -C or -C<colour> string
	char *stringcolour;
	for (count= 0; count<COLOURSIZES; ++count)
	{
		if(strstr(colourset,coloursavailable[count])!=NULL)
		{	/* here checking if the -C<colour> is available colour or otherwise print error
			message 
			*/
			stringcolour=strstr(colourset,coloursavailable[count]);

			if(strcmp(stringcolour,coloursavailable[count])==0){
			printf("%s\n",stringcolour );
			return count;
			}
			else{
				fprintf(stderr, " '%s' :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",stringcolour );
				exit(0);
			}
		}
		else if(count==COLOURSIZES-1){
			
			int stringlenght=(int)strlen(colourset);
			stringcolour[stringlenght-2]='\0';
			if(stringlenght!=2){
				memcpy(stringcolour,&colourset[2],stringlenght-2);
				fprintf(stderr, " '%s' :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",stringcolour );
					exit(0);
			}
			break;		
		}

	}

	if(argv[argcount+1]==NULL){
		/*-C <colour> ; when there is no colour entered print error
		*/
		fprintf(stderr, "%s: option requires an argument -- 'C'\n",argv[0]);
		fprintf(stdout, "usage : clock -C <black|red|green|yellow|blue|magenta|cyan|white]\n");
		exit(0);
	}


	for (count= 0; count<COLOURSIZES; ++count)
	{	//if there is -C <colour> available return the colour value
		if(strcmp(coloursavailable[count],argv[argcount+1])==0)
		{
			return count;
		}
	}
	//if there is -C <colour> available this error will print and exit the prog
	fprintf(stderr, " '%s' :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",argv[argcount+1] );
	exit(0);
}

void errorprint(int argc, char  **argv){
	/*
	this assign the colour from colourchecker func.if notprint errors when the args are wrong
	*/
	int argcount;

	
	
	for (argcount = 0; argcount < argc; ++argcount)
	{
		if( argv[argcount][0]=='-'){

			if(argv[argcount][1]==colour){
				forecolour=colourchecker(argv,argcount);
				break;
			}else if( argv[argcount][1]==help){
				fprintf(stdout, "usage : clock -C <black|red|green|yellow|blue|magenta|cyan|white]\n");
				exit(0);
			}else if( argv[argcount][0]=='-'){
				fprintf(stderr, "%s: invalid option -- '%s'\n",argv[0],argv[argcount]);
				fprintf(stdout, "usage : clock -C <black|red|green|yellow|blue|magenta|cyan|white]\n");
				exit(0);
				
			}
		}

	}
}



int main(int argc, char  **argv){
	/*
	main function here making a clock loop to print
	*/
	unsigned int timer=100000;
	errorprint(argc,argv);

	while(1){
		
		printf("\033[2J\033[1;1H"); //clear the screen and reset the cursor to 0,0 position
		printf("\033[1B\033[1C"); //move cursor to 1 line below
		timedisplay();
		
		usleep(timer);  //sleeping the clock for timer microsecond
	}
	return 0;
}