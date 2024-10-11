#ifndef _PLAYER
#define _PLAYER

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

#endif