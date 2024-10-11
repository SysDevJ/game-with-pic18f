#ifndef _LCD
#define _LCD


#define		RS_PIN		PORTCbits.RC0
#define 	RW_PIN 		PORTCbits.RC1
#define 	EN_PIN		PORTCbits.RC2
#define		size  		 3

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

#endif