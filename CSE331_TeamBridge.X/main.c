#define _XTAL_FREQ 4000000

//Define LCD pin
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//Define Sensor pin
#define Trigger RB1 //34 is Trigger
#define Echo RB2//35 is Echo

//Define LED and Buzzer pin
#define Buzzer RE0
#define LED RA0

#include <xc.h>
#include "lcd.h"
#include <stdio.h>


// BEGIN CONFIG
#pragma config FOSC = HS   
#pragma config WDTE = OFF  
#pragma config PWRTE = OFF 
#pragma config BOREN = ON 
#pragma config LVP = OFF   
#pragma config CPD = OFF   
#pragma config WRT = OFF  
#pragma config CP = OFF    
//END CONFIG

//initialize Variable
int time_taken;
int distance;
char d1,d2,d3;
void main()
{ 
  TRISD = 0x00;               // LCD Pins as Output
  TRISD = 0x00;              //PORTD declared as output for interfacing LCD
  TRISB1 = 0;               //Trigger pin of UltraSonic sensor is sent as output pin
  TRISB2 = 1;                //Echo pin of UltraSonic sensor is set as input pin
  TRISA0=0;                 //pin of LED set as Output
  TRISE0=0;                 // pin of Buzzer is set as output

  Lcd_Init();
  LED=1;
  Buzzer=1;
  Lcd_Set_Cursor(1,1);
  Lcd_Write_String("Water Level");
  Lcd_Set_Cursor(2,1);
  Lcd_Write_String("Indicator");
  __delay_ms(1000);
  Lcd_Clear();
  
  while(1)
    { 
        TMR1H =0; TMR1L =0; //clear the timer bits
        
        Trigger = 1; 
        __delay_us(10);           
        Trigger = 0;  
        
        while (Echo==0);
            TMR1ON = 1;
        while (Echo==1);
            TMR1ON = 0;
        
        time_taken = (TMR1L | (TMR1H<<8)); 
        
        distance= (0.0272*time_taken)/2;
        //time_taken = time_taken * 0.8;
        
        if(distance<10)
        {
            LED=1;
            Buzzer=1;
        }
        else
        {
            LED=0;
            Buzzer=0;
        }
        
        
        d1 = (distance/100)%10;
        d2 = (distance/10)%10;
        d3 = (distance/1)%10;
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Detecting....");
              
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("distance:");
        Lcd_Write_Char(d1+'0');
        Lcd_Write_Char(d2+'0');
        Lcd_Write_Char(d3+'0');
    
    }
}