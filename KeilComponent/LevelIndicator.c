#include<LPC21xx.h>

void LCD_init(void);
void LCD_cmd(unsigned char);
void LCD_Data_Shift(unsigned char);
void LCD_Write(unsigned char);
void LCD_Write_Text(unsigned char[]);
void Enable_Pulse(void);
void delay(unsigned int);

/* Different commands used to interface with LCD are : */
#define LCD_CLEAR 0x01
#define CURSOR_OFF 0x0C
#define FIRST_ROW 0x80
#define SECOND_ROW 0xC0 

int main()
{

	unsigned int var , temp;
	IO0DIR = 0x00ff003C;
	LCD_init();
	delay(10);
	while(1)
	{
		/*get input value and store it in ‘var’ , after logical
		left shifting by 12 the input value will be stored in 0th
		to 3rd bit position of ‘var’ */
		var = (0x0000f000 & IO0PIN);
		var >>=12;
		if(var != temp) // temp holds previous ‘var’ value
		{
			IO0CLR = 0x20;
			LCD_cmd(LCD_CLEAR);
			temp = var;
			if(var >= 8) //var = 8 to 15
			{
				unsigned char msg[] = "Tank is Full";
				LCD_Write_Text(msg);
				IO0SET = 0x20;
			}
			else if(var >= 4) //var = 4 to 7
			{
				unsigned char msg[] = "Tank at 75%";
				LCD_Write_Text(msg);
			}
			else if(var >= 2) //var = 2 or 3
			{ 
				unsigned char msg[] = "Tank at 50%";
				LCD_Write_Text(msg);
			}
			else if(var == 1) //var = 1
			{
				unsigned char msg[] = "Tank at 25%";
				LCD_Write_Text(msg);
				}
			else //var = 0
			{
				unsigned char msg[] = "Tank is Empty!";
				LCD_Write_Text(msg);
			}
		}

	}

}

void LCD_init(void)
{
		/* Initializes LCD with various commands so that it can
		interface with microcontroller */
		LCD_cmd(0x38); //Send 8-bit initialization command to lcd
		delay(10);
		LCD_cmd(CURSOR_OFF); //Cursor OFF 0x0C
		delay(10);
		LCD_cmd(LCD_CLEAR); // LCD_Clear 0x01
		delay(2);
		LCD_cmd(FIRST_ROW); //select LCD first row 0x80
}

void LCD_cmd(unsigned char x)
{
 /*Executes commands in LCD */ 

 IO0CLR = 0x0000001C;
 IO0SET = 0x00000010; //RS= 0 COMMAND MODE
 LCD_Data_Shift(x);

}
void LCD_Data_Shift(unsigned char x)
{
 /*Sets the output data line values */
 unsigned int value = x;
 value <<=16;
 IO0CLR = 0x00ff0000;
 IO0SET = value;
 Enable_Pulse();
}
void LCD_Write(unsigned char value)
{
 /*Configure LCD for receiving Display Data*/
 IO0CLR = 0x0000000C;
 IO0SET = 0x00000014; //RS=1 DATA MODE
 LCD_Data_Shift(value);
}
void LCD_Write_Text(unsigned char msg[])
{
 /*Prints the given string ‘msg’ */
 while(*msg)
 {
 LCD_Write(*msg);
 msg++;
 }
} 

void Enable_Pulse(void)
{
/* Generates a high to low pulse so that the command gets
executed */
 IO0SET = 0x00000010;
 delay(1);
 IO0CLR = 0x00000010;
 delay(1);
}
void delay(unsigned int x)
{
 /* Delay of ‘x’ milliseconds */
 unsigned long i, j;
 for( j=0; j<x; j++)
 for( i=0; i<7000; i++);
}