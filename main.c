
#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

enum States {Press_Start,Display_Sequence,Display_Timer,Display_Off,User_Input,Checker_Input,Looser,Winner, Button_Pressed,Button_Released,Display_Score,idle,idle_Off, Button_Released_Next_Input}state;
unsigned int r = 0;
unsigned char score = 0;
char *game_checker;
unsigned int counter = 0;
unsigned int Max = 0;
unsigned int tracker = 0;
unsigned char cnt = 0;
unsigned int Game_arr[9] = {0x01, 0x02, 0x01, 0x04, 0x08, 0x01,0x04,0x02,0x01};
unsigned char inputz = 0;
unsigned long outputNumber_elapsedTime = 1000;
const unsigned long timerPeriod = 200;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char button1 = 0;
unsigned char button2 = 0;
unsigned char button3 = 0;
unsigned char button4 = 0;
unsigned char cntzz = 0;
unsigned char keeper = 0;
unsigned char cntzz2 = 0;
unsigned char maximum = 0;

unsigned char DisplayIndex = 0;
unsigned char CheckIndex = 0;
unsigned char buttons;
void Tick()
{
	button1 = ~PINA & 0x01;
	button2 = ~PINA & 0x02;
	button3 = ~PINA & 0x04;
	button4 = ~PINA & 0x08;
	//game_checker = Game_arr;														//CHECKS USER INPUT WITH GENERATED VALUES POINTER
	switch(state)
	{
		case Press_Start:
		if(button1 || button2 ||button3 || button4)													//User Pressed a Button to Start
		{
			state = Display_Sequence;
			cntzz++;
		}
		else if(!button1 && !button2 && !button3 && !button4)											//Waiting for user_Input
		{
			state = Press_Start;															//Insert Welcome Screen
		}
		break;
		
		case Display_Sequence:																		//bksbakjbfkjbafbjka
		state = idle;
		PORTB = Game_arr[DisplayIndex];
		DisplayIndex++;
		break;
		
		case idle:
		if(cntzz < 10)
		{
			state = idle;
			cntzz++;
		}
		else if(cntzz >= 10)
		{
			state = idle_Off;
			cntzz = 0;
		}
		break;
		
		case idle_Off:
		if(maximum >= DisplayIndex)
		{
			state = Display_Sequence;
		}
		else
		{
			state = User_Input;
			PORTB = 0;																//Resets all variables
			LCD_DisplayString(1, "Waiting for Input: ");
		}
		break;
		
		case User_Input:
		if(!button1 && !button2 && !button3 && !button4)
		{
			state = User_Input;
		}
		else
		{
			state = Checker_Input;
		}
		break;
		
		case Button_Released:
		if(!button1 && !button2 && !button3 && !button4)
		{
			state = Display_Sequence;
			LCD_DisplayString(1, "Current Score  ");
			LCD_WriteData(score + '0');
		}
		break;
		
		case Button_Released_Next_Input:
		if(!button1 && !button2 && !button3 && !button4)
		{
			state = User_Input;
			LCD_DisplayString(1, "Next input please  ");
		}
		break;
		
		case Checker_Input:
		buttons = button1 | button2 | button3 | button4;
		if(Game_arr[CheckIndex] == buttons)
		{
			if(CheckIndex < maximum && score < 10)
			{
				// need to check next input.
				CheckIndex++;
				state = Button_Released_Next_Input;
				LCD_DisplayString(1, "Release Button!");
			}
			else if(score < 10)
			{
				// Won round.
				maximum++;
				DisplayIndex = 0;
				CheckIndex = 0;
				score++;
				state = Button_Released;
			}
			else
			{
				state = Winner;
				LCD_DisplayString(1, "GG, You Win");
			}
		}
		else
		{
			state = Looser;
			LCD_DisplayString(1, "GG, You Lose");
			LCD_WriteData(score + '0');
		}
		break;
		
		
		
		case Looser:
		if(!button1 && !button2 && !button3 && !button4)
		{
			state = Winner;
		}
		else
		{
			state = Press_Start;
			LCD_DisplayString(1, "Press Start");
		}
		break;
		
		case Winner:
		if(!button1 && !button2 && !button3 && !button4)
		{
			state = Winner;
			//LCD_DisplayString(1, "GG, You Win");
		}
		else
		{
			state = Press_Start;
			LCD_DisplayString(1, "Press Start");
		}
		break;
		
		case Display_Score:
		state = Display_Sequence;
		//counter = 0;
		
		//Resets
		default: break;																				//Create Random sequence
	}
	
	switch(state)
	{
		case Press_Start:																		//Resets all variables
		counter = 0;
		score = 0;
		tracker = 0;
		cnt = 0;
		keeper = 0;
		maximum = 0;
		
		break;
		
		case Display_Sequence:
		break;
		
		case idle:
		break;
		
		case idle_Off:
		break;
		
		case User_Input:
		break;
		
		case Button_Pressed:
		break;
		
		case Button_Released:
		break;
		
		case Checker_Input:
		break;
		
		case Looser:
		break;
		
		case Winner:
		
		break;
		
		case Display_Score:
		break;
		
	}

}
void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff()
{
	TCCR1B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}



int main(void)
{
	state = Press_Start;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(50);
	TimerOn();
	LCD_init();
	LCD_DisplayString(1, "Press Start");
	while(1)
	{
		Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

