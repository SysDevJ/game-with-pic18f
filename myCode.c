#include <p18f452.h>

#pragma config WDT = OFF

#define		rs		PORTCbits.RC0
#define 	rw 		PORTCbits.RC1
#define 	en		PORTCbits.RC2
#define		size    3

unsigned char state = 0;
unsigned char k = 0;
unsigned char counter = 0;
unsigned char choose = 0;
unsigned char x = 0, y = 0;
unsigned char player = 0;
unsigned char counting = 0;
unsigned char cell = 0;

unsigned char map[size][size];
unsigned char string_1[] = "ROW :  ";
unsigned char string_2[] = "COLUMN :  ";
unsigned char string_3[] = {0, 1, 2, '\0'};
unsigned char string_4[] = "WRONG POSITION";
unsigned char string_5[] = "PLAYER 1";
unsigned char string_6[] = "PLAYER 2";
unsigned char string_7[] = "PLAYER 1 WINS";
unsigned char string_8[] = "PLAYER 2 WINS";


void initiaLcd(void);			
void delay250ms(void);		
void delay3us(void);			
void commandInst(void);
void busyFlag(void);				
void dataInst(void);				
void timer0Generation(void);
void creatTable(void);
void shiftCursor(unsigned char limit);
void jumpLine(unsigned char i);
void firstLine(void);
void secondLine(void);
void thirdLine(void);
void forthLine(void);
void enterRowNumber(void);
void enterColumnNumber(void);
void deletForthLine(void);
void displayResult(void);
void checkPosition(void);
void fillArray(void);
void fillPlayer(unsigned char* pointer);
void checkWinner(void);
void justDisplay(unsigned char* pointer);
void clearEntire(void);


#pragma interrupt myInterruptFunction
void myInterruptFunction(void)
{	
	if(INTCON3bits.INT1IF == 1)				// INT1 INTERRUPT
	{
		INTCON3bits.INT1IF = 0;
		if(player == 0)
			fillPlayer(string_5);
		else
			fillPlayer(string_6);
		if(!state)							// PRESS ONCE
		{
			state = 1;
			enterRowNumber();		
		}	
		else		
		{			
			state = 0;				
			enterColumnNumber();
		}
	}
	else if(INTCONbits.INT0IF == 1)			// INT0 INTERRUPT
	{
		INTCONbits.INT0IF = 0;
		counter = string_3[k++];
		displayResult();
		if(k == 3)
			k = 0;
	}
	else if(INTCON3bits.INT2IF == 1)
	{
		INTCON3bits.INT2IF = 0;	
		if(choose == 0)
		{
			choose = 1;
			x = counter;
			counter = 0;
			k = 0;
		}
		else
		{
			y = counter;
			counter = 0;
			k = 0;
			checkPosition();
		}
	}
}

#pragma code myInterruptVector = 0x00008
void myInterruptVector(void)
{
	_asm
		GOTO myInterruptFunction
	_endasm
}

void main(void)
{		
		unsigned char i = 0;
		TRISD = 0x00;
		TRISC = 0xF8;
		initiaLcd();
		while(i<3)								// CREAT A TABLE OF 3 LINE + 3 COLUMNS
		{	
			creatTable();
			jumpLine(i);						// JUMP TO NEXT LINE BASED ON I
			++i;
		}
		firstLine();
		fillArray();
		INTCONbits.GIE = 1;						// GLOBAL INTERRUPT ENABLE BIT
		INTCONbits.INT0IE = 1;					// ACTIVATING INT0 INTERRUPT
		INTCON3bits.INT1IE = 1;					// ACTIVATING INT1 INTERRUPT
		INTCON3bits.INT2IE = 1;				
		INTCONbits.INT0IF = 0;					
		INTCON3bits.INT1IF = 0;				
		INTCON3bits.INT2IF = 0;	
		INTCON2bits.INTEDG0 = 0;				// INTERRUPT ON FALLING EDGE
		INTCON2bits.INTEDG1 = 0;				// INTERRUPT ON FALLING EDGE
		INTCON2bits.INTEDG2 = 0;		
		while(1);			
}
void initiaLcd(void)
{
		LATD = 0x38;
		commandInst();
		delay250ms();
		LATD = 0x01;
		commandInst();
		delay250ms();
		LATD = 0x0C;
		commandInst();
		delay250ms();
}
void delay250ms(void)
{
		T0CON = 0x01;
		TMR0H = 0x0B;
		TMR0L = 0xBC;
		timer0Generation();		
}
void delay3us(void)
{
		T0CON = 0x48;
		TMR0L = 253;
		timer0Generation();
}
void commandInst(void)
{
		rs = 0;
		rw = 0;
		en = 1;
		delay3us();
		en = 0;
}
void dataInst(void)
{
		rs = 1;
		rw = 0;
		en = 1;
		delay3us();
		en = 0;
}
void busyFlag(void)
{
		rs = 0;
		rw = 1;	
		TRISDbits.TRISD7 = 1;
		do
		{
			en = 0;
			delay3us();
			en = 1;
		}while(PORTDbits.RD7 == 1);
		en = 0;
		TRISDbits.TRISD7 = 0;
}
void timer0Generation(void)
{
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 1;
	while(INTCONbits.TMR0IF == 0);
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 0;	
}
void creatTable(void)
{	
	shiftCursor(1);	
	LATD = '|';					
	dataInst();
	busyFlag();
	shiftCursor(1);	
	LATD = '|';
	dataInst();
	busyFlag();	
}
void shiftCursor(unsigned char limit)
{
	unsigned char i = 0;
	while(i<limit)
	{
		LATD = 0x14;
		commandInst();
		busyFlag();
		++i;
	}
}
void jumpLine(unsigned char i)				
{
	switch(i)
	{
		case 0 :
			secondLine();
			break;
		case 1 :
			thirdLine();
			break;
	}
}
void firstLine(void)				// JUMP TO FIRST LINE
{
	LATD = 0x80;
	commandInst();
	busyFlag();
}
void secondLine(void)				// JUMP TO SECOND LINE
{		
	LATD = 0xC0;
	commandInst();
	busyFlag();
}
void thirdLine(void)				// JUMP TO THIRD LINE
{
	LATD = 0x90;
	commandInst();
	busyFlag();
}
void forthLine(void)				// JUMP TO FORTH LINE
{
	LATD = 0xD0;
	commandInst();
	busyFlag();
}
void enterRowNumber(void)			// DISPLAY IN FORTH LINE THE STRING_1
{
	deletForthLine();
	justDisplay(string_1);
}
void enterColumnNumber(void)			// DISPLAY IN FORTH LINE THE STRING_2
{
	deletForthLine();
	justDisplay(string_2);
}
void deletForthLine(void)			// DELET FORTH LINE TO DISPLAY NEW STRING
{
	unsigned char i = 0;
	forthLine();
	while(i<16)
	{
		LATD = 0x20;
		dataInst();
		busyFlag();
		++i;
	}
	forthLine();
}
void displayResult(void)			// DISPLAY THE NUMBER OF ROW OR COLUMN ENTERD USING (PORTB RB0)
{
	LATD = 0x10;
	commandInst();
	busyFlag();
	LATD = counter + 0x30;
	dataInst();
	busyFlag();
}
void fillArray(void)
{
	unsigned char i = 0, j = 0;
	while(i<size)
	{
		j = 0;		
		while(j<size)
		{
			map[i][j] = 'E';
			++j;
		}
		++i;
	}
}
void checkPosition(void)
{
	++counting;
	if(map[x][y] == 'E')
	{
		static unsigned char decision = 0;
		switch(x)
		{
			case 0 :
				firstLine();				// JUMP FIRST LINE		
				break;
			case 1 :		
				secondLine();				// JUMP TO SECOND LINE			
				break;
			case 2 :
				thirdLine();				// JUMP TO THIRD LINE
				break;
		}	
		switch(y)
		{
			case 0 :
				shiftCursor(0);					// FISRT COLUMN	
				break;
			case 1 :
				shiftCursor(2);					// THIRD COLUMN
				break;
			case 2 :
				shiftCursor(4);					// FIFTH COLUMN
				break;
		}
		if(decision == 0)
		{
			LATD = 'X';
			dataInst();
			busyFlag();
			map[x][y] = 'X';
			decision = 1;
		}
		else
		{
			LATD = 'O';
			dataInst();
			busyFlag();
			map[x][y] = 'O';
			decision = 0;
		}
		if(player == 0)						// DISPLAY PLAYER 0
			player = 1;
		else								// DISPLAY PLAYER 1
			player = 0;
		if(counting >= 4)
			checkWinner();
	}
	else
	{	
		deletForthLine();
		justDisplay(string_4);
		delay250ms();
		delay250ms();
		delay250ms();
		deletForthLine();
	}
	counter = 0;
	state = 0;
	choose = 0;
	x = 0;
	y = 0;
	k = 0;
	firstLine();
}

void fillPlayer(unsigned char* pointer)					// DISPLAY PLAYER STRING	
{
	firstLine();
	shiftCursor(6);
	justDisplay(pointer);
	firstLine();
}
void justDisplay(unsigned char* pointer)				// FUNCTION TO DISPLAY STRINGS 
{
	unsigned char i = 0;
	while(pointer[i] != '\0')
	{
		LATD = pointer[i];
		dataInst();
		busyFlag();
		++i;
	}
}
void checkWinner(void)										// CHECK PLAYER 1 WINS
{
	
	if(map[0][0] == 'X' && map[0][1] == 'X' && map[0][2] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[1][0] == 'X' && map[1][1] == 'X' && map[1][2] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);	
		clearEntire();	
	}
	else if(map[2][0] == 'X' && map[2][1] == 'X' && map[2][2] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[0][0] == 'X' && map[1][0] == 'X' && map[2][0] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[0][1] == 'X' && map[1][1] == 'X' && map[2][1] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[0][2] == 'X' && map[1][2] == 'X' && map[2][2] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[0][0] == 'X' && map[1][1] == 'X' && map[2][2] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);
		clearEntire();		
	}
	else if(map[0][2] == 'X' && map[1][1] == 'X' && map[2][0] == 'X')
	{
		deletForthLine();
		justDisplay(string_7);	
	}
	else if(map[0][0] == 'O' && map[0][1] == 'O' && map[0][2] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);	
		clearEntire();	
	}
	else if(map[1][0] == 'O' && map[1][1] == 'O' && map[1][2] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);	
		clearEntire();	
	}
	else if(map[2][0] == 'O' && map[2][1] == 'O' && map[2][2] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);	
		clearEntire();	
	}
	else if(map[0][0] == 'O' && map[1][0] == 'O' && map[2][0] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);	
		clearEntire();	
	}
	else if(map[0][1] == 'O' && map[1][1] == 'O' && map[2][1] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);
		clearEntire();		
	}
	else if(map[0][2] == 'O' && map[1][2] == 'O' && map[2][2] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);	
		clearEntire();	
	}
	else if(map[0][0] == 'O' && map[1][1] == 'O' && map[2][2] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);
		clearEntire();		
	}
	else if(map[0][2] =='O' && map[1][1] == 'O' && map[2][0] == 'O')
	{
		deletForthLine();
		justDisplay(string_8);
		clearEntire();	
	}
	else 	
	{
		++cell;
		if(cell == 6)
		{
			unsigned char str[] = "DRAW";
			deletForthLine();
			justDisplay(str);
			clearEntire();	
		}
	}
}
void clearEntire(void)
{
	unsigned char i = 0;
	counter = 0;
	state = 0;
	choose = 0;
	x = 0;
	y = 0;
	k = 0;
	cell = 0;
	counting = 0;
	player = 0;
	delay250ms();
	delay250ms();
	delay250ms();
	delay250ms();
	LATD = 0x01;
	commandInst();
	delay250ms();
	while(i<3)								// CREAT A TABLE OF 3 LINE + 3 COLUMNS
	{	
		creatTable();
		jumpLine(i);						// JUMP TO NEXT LINE BASED ON I
		++i;
	}
	fillArray();
	firstLine();
}