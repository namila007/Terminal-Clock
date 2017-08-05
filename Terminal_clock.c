#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#define DATE_ROW	1
#define	DATE_COL	15
#define ROW_SIZE 	5				//declaring constants
#define COL_SIZE 	6
#define LEVEL_SIZE 11
#define RESET 		0
#define BRIGHT 		1
#define COLOURSIZES 8
#define ZERO		0
#define BACKGROUND  0
#define DOTSIGN 10

/* prototype functions 
*/
void timedisplay();
void printwindows();
void initializewindows();
WINDOW *create_newwin(int height, int width, int starty, int startx);
void timesetdigits(int hour,int min,int sec);
void displaydigit(int timedigits,WINDOW *win1,WINDOW *win2);
void digitprint(int numprint, WINDOW *win);
void refreshwins();
void printwindows(WINDOW *win,char* strngs);
void clearwins();
int colourchecker(char **argv,int argcount);
void errorprint(int argc, char  **argv);
void intcolours();
void displaydays(int year,int mont,int day);



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

WINDOW *HOUR1,*HOUR2,*MIN1,*MIN2,*SEC1,*SEC2,*DATE,*DOT1,*DOT2; //declaring ncurses windows




void intcolours(){
	/*
	colour pairs ncurses library
	*/
	
	init_pair(1,BACKGROUND,forecolour);
	init_pair(2,forecolour,BACKGROUND);
}

void displaydays(int year,int mont,int day){
	/*
	printing year day and month in the terminal
	*/
	wattron(DATE,COLOR_PAIR(2));
	wprintw(DATE,"%d-%02d-%02d\n",1900+year,mont+1,day);
	wattroff(DATE,COLOR_PAIR(2));
	
	
	
	
}


void timesetdigits(int hour,int min,int sec)
{
	/*
	setting the digits to deplay and ':' symbol between hour ,min and sec
	*/
 	
 	displaydigit(hour,HOUR1,HOUR2);
 	digitprint(DOTSIGN,DOT1);
 
	displaydigit(min,MIN1,MIN2);
 	digitprint(DOTSIGN,DOT2);
 	
 	displaydigit(sec,SEC1,SEC2);
 }


void displaydigit(int timedigits,WINDOW *win1,WINDOW *win2){
	/*
	getting two digits from timedigits and print them in terminal
	*/
	int digit1,digit2;

	digit1=timedigits/10;
	digit2=timedigits%10;

	digitprint(digit1,win1);
	
	digitprint(digit2,win2);
	
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




void digitprint(int numprint, WINDOW *win){
	/*
	function to print digits 5x6 matrix size
	*/

	int row,col;
	for (row = 0; row <ROW_SIZE; ++row)
	{
		for (col = 0; col <COL_SIZE; ++col)
		{
			if(Disp_array[numprint][row][col]==1){
				wattron(win,COLOR_PAIR(1));
				 wprintw(win," ");
				wattroff(win,COLOR_PAIR(1));
			}else{
				wprintw(win," ");
			}
		}
		
	}

}



void initializewindows(){
	/*
	innitializing ncruses windows for each digits
	*/
	HOUR1= create_newwin(ROW_SIZE,COL_SIZE,1,1);
	HOUR2= create_newwin(ROW_SIZE,COL_SIZE,1,8);
	DOT1= create_newwin(ROW_SIZE,COL_SIZE,1,14);
	MIN1= create_newwin(ROW_SIZE,COL_SIZE,1,20);
	MIN2= create_newwin(ROW_SIZE,COL_SIZE,1,27);
	DOT2= create_newwin(ROW_SIZE,COL_SIZE,1,33);
	SEC1= create_newwin(ROW_SIZE,COL_SIZE,1,39);
	SEC2= create_newwin(ROW_SIZE,COL_SIZE,1,46);
	DATE= create_newwin(DATE_ROW,DATE_COL,7,22);
}

int colourchecker(char **argv,int argcount){
	/*
	function to check whether input colour is available or not,if available return colour value.
	 if there is no colour inputs it print error message how to use colours
	*/
	int count;
	char *colourset=argv[argcount];  //asigning the -C or -C<colour> string
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
			
			int countar;
			int stringlenght=(int)strlen(colourset);
			//stringcolour[stringlenght-2]='\0';
			
			for(countar=2;countar<stringlenght;countar++)
				{
					stringcolour[countar-2]=colourset[countar];

					//printf("%c\n",stringcolour1[countar] );
				}
				stringcolour[stringlenght-2]='\0';

			if(stringlenght!=2){
				
				
					printf("%s\n",stringcolour);
				
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
				int argrun;
				for(argrun=1;argrun<strlen(argv[argcount]);argrun++){
				fprintf(stderr, "%s: invalid option -- '%c'\n",argv[0],argv[argcount][argrun]);
			}
				fprintf(stdout, "usage : clock -C <black|red|green|yellow|blue|magenta|cyan|white]\n");
				exit(0);
				
			}
		}

	}
}




void refreshwins(){
	/*
	refreshing ncurses windows
	*/
	wrefresh(HOUR1);
	wrefresh(HOUR2);
	wrefresh(MIN1);
	wrefresh(MIN2);
	wrefresh(SEC1);
	wrefresh(SEC2);
	wrefresh(DOT1);
	wrefresh(DOT2);
	wrefresh(DATE);
}

void clearwins(){
	/*
	clearing ncurses windows
	*/
	wclear(HOUR1);
	wclear(HOUR2);
	wclear(MIN1);
	wclear(MIN2);
	wclear(SEC1);
	wclear(SEC2);
	wclear(DOT1);
	wclear(DOT2);
	wclear(DATE);
}


WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

int main(int argc, char  **argv){
	/*
	main function here making a clock loop to print
	*/
	errorprint(argc,argv);
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();
	initializewindows();

	intcolours();
	

	while(1){
		
		clearwins();
		timedisplay();
		refreshwins();
		//usleep(timer);  //sleeping the clock for timer microsecond
	}


	endwin();
	curs_set(TRUE);
	return 0;
}