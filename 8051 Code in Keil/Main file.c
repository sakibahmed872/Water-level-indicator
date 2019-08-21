/*
Author: Sakib Ahmed
*/
#include<reg52.h>
#include <stdio.h>
#include <LCD_8bit.h>
#include <math.h>

#define sound_velocity 34300  	/* sound velocity in cm per second */

#define period_in_us pow(10,-6)
#define Clock_period 1.085*period_in_us		/* period for clock cycle of 8051*/

sbit Trigger_pin=P3^1;        	/* Trigger pin */
sbit Echo_pin=P3^2;							/* Echo pin */

void Delay_us()
{
		TL0=0xF5;
		TH0=0xFF;
		TR0=1;
		while (TF0==0);
		TR0=0;
		TF0=0;
}

void init_timer(){
	TMOD=0x01;										/*initialize Timer*/
	TF0=0;
	TR0 = 0;
}

void send_trigger_pulse(){
	  Trigger_pin= 1;           	/* pull trigger pin HIGH */
    Delay_us();               	/* provide 10uS Delay*/
    Trigger_pin = 0;          	/* pull trigger pin LOW*/
}

void main()
{
	float distance_measurement, value;
	unsigned char water_level[10];
	LCD_Init();										/* Initialize 16x2 LCD */	
	LCD_String_xy(1,1,"Water Level : ");
	init_timer();									/* Initialize Timer*/
	
while(1)
{		
		//send_trigger_pulse();				/* send trigger pulse of 10us */  
		while(!Echo_pin){           /* Waiting for Echo */
    send_trigger_pulse();
		TR0 = 1;}                    /* Timer Starts */
    while(Echo_pin && !TF0);    /* Waiting for Echo goes LOW */
    TR0 = 0;                    /* Stop the timer */
	  
		/* calculate distance using timer */
		value = Clock_period * sound_velocity; 
		distance_measurement = (TL0|(TH0<<8));										/* read timer register for time count */
		distance_measurement = (distance_measurement*value)/2.0;  /* find distance(in cm) */
		//distance_measurement = ((139-distance_measurement)/139)*100;

		sprintf(water_level, "%d", (int)distance_measurement);
		LCD_String_xy(2,6,water_level);												/* show distance on 16x2 LCD */
		LCD_String("  %  ");		
					
		delay(100);
}
}
