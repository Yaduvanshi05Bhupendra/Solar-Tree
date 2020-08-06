#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#define Ref 1500                      //Servo
//*********************************************SERVO START***********************************************//
int x = Ref, y = Ref;
int p = Ref, r = Ref;

int Rotation_Plus(int a)
{
	
	int i=1, n;
	while(bit_is_clear(PINA, 0))
	{
		OCR1A = ICR1 - (a+(9*i));
		_delay_ms(500);
		i++;
	}
	n = ICR1 - OCR1A;
	return n;
}

int Rotation_Minus(int a)
{
	
	int i=0 ,n;
	while(bit_is_clear(PINA, 1))
	{
		OCR1A = ICR1 - (a-(9*i));
		_delay_ms(500);
		i++;
	}
	n = ICR1 - OCR1A;
	return n;
}

int Retain_Position(int a)
{
	Lcd_cmd(0x8A);
	_delay_ms(100);
	Display("...");
	_delay_ms(100);
	
	int n;
	OCR1A = ICR1 - a;
	_delay_ms(500);
	n = ICR1 - OCR1A;
	return n;
}

int Reset_to_Zero(int a)
{
	int i, n, p, k;
	
	
	if(a>Ref)            
	{
		p = a - 1500;
		k = p/9;
		for(i=1; i <= k; i++)
		{
			OCR1A = ICR1 - (a-(9*i));
			_delay_ms(500);
		}
	}
	
	if(a<=Ref)
	{
		p = 1500 - a;
		k = p/9;
		for(i=1; i <= k ; i++)
		{
			OCR1A = ICR1 - (a+(9*i));
			_delay_ms(500);
		}
	}
	n = ICR1 - OCR1A;
	return n;
}
//***************************************SERVO END**************************************************//

//*********************************************SERVO 1 START***********************************************//

int Rotation_Plus_1(int a)
{
	PORTD |= 1<<PIND0;
	
	int i=1, n;
	while(bit_is_clear(PINA, 2))
	{
		OCR1A = ICR1 - (a+(12*i));
		_delay_ms(500);
		i++;
	}
	
	n = ICR1 - OCR1A;
	return n;
}

int Rotation_Minus_1(int a)
{
	
	PORTD |= 1<<PIND0;
	
	int i=0 ,n;
	while(bit_is_clear(PINA, 3))
	{
		OCR1A = ICR1 - (a-(12*i));
		_delay_ms(500);
		i++;
	}
	n = ICR1 - OCR1A;
	return n;
}


int Retain_Position_1(int a)
{
	PORTD |= 1<<PIND0;
	
	Lcd_cmd(0x8A);
	_delay_ms(100);
	Display("...");
	_delay_ms(100);
	
	int n;
	OCR1A = ICR1 - a;
	_delay_ms(500);
	n = ICR1 - OCR1A;
	return n;
}

int Reset_to_Zero_1(int a)
{
	PORTD |= 1<<PIND0;
	
	int i, n, p, k;
	
	
	if(a>Ref)
	{
		p = a - 1500;
		k = p/12;
		for(i=1; i <= k; i++)
		{
			OCR1A = ICR1 - (a-(12*i));
			_delay_ms(500);
		}
	}
	
	if(a<=Ref)
	{
		p = 1500 - a;
		k = p/12;
		for(i=1; i <= k ; i++)
		{
			OCR1A = ICR1 - (a+(12*i));
			_delay_ms(500);
		}
	}
	n = ICR1 - OCR1A;
	return n;
}
//***************************************SERVO 1 END**************************************************//

//****************************************LED ON****************************************************//

void Led_On()
{
	Lcd_cmd(0x01);
	_delay_ms(100);
	Display("LED ON...");
	_delay_ms(100);
	Lcd_cmd(0xC0);
	_delay_ms(100);
	Display("Night");
	_delay_ms(100);
	
	while(bit_is_clear(PINA, 4))
	{
		PORTD |= 1<<PIND3;
	}
}

//****************************************LED OFF***************************************************//


//***************************************LCD START*************************************************//

void Lcd_cmd(char x)
{
	PORTB = x;
	PORTC = 0x04; //|= (1<<PINC2)
	_delay_ms(10);
	PORTC = 0x00;     //&= ~(1<<PINC2); 
	_delay_ms(10);
}
void Lcd_data(char x)
{
	PORTB = x;
	PORTC =  0x05;  //|= (1<<PINC0) | (1<<PINC2); 
	_delay_ms(10);
	PORTC = 0x01;  //|= (1<<PINC0); PORTC &= ~(1<<PINC2);
	_delay_ms(10);
}

void Display(char x[])
{
	int i;
	for(i=0; x[i]!='\0'; i++)
	{
		Lcd_data(x[i]);
	}
}

void Lcd_int()
{
	Lcd_cmd(0x38);
	Lcd_cmd(0x0E);
	Lcd_cmd(0x01);
}

//****************************************LCD END**************************************************//
	

int main(void)
{
	DDRD |= 1<<PIND5 | 1<<PIND3 | 1<<PIND0;                                   // Define direction resistor OCR1A(PD5) as output, PD3 for LED
	DDRA &= ~(1<<PINA0) & ~(1<<PINA1) & ~(1<<PINA2) & ~(1<<PINA3) & ~(1<<PINA4);  //For sensor input
	PORTA |= 1<<PINA0 | 1<<PINA1 | 1<<PINA2 | 1<<PINA3 | 1<<PINA4;
	DDRB = 0xFF;                                                                 //LCD DATA PIN
	DDRC = 0xFF;                                                                 //LCD Signal, STEPPER OUTPUT
     
	
	
//******************** SERVO START TIMER**********************//
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	
	ICR1 = 19999;
/******************** SERVO END TIMER**************************/

OCR1A = ICR1 - 1500;

/***********************LCD ON*********************************/

Lcd_int();

Lcd_cmd(0x80);
Display("Project On");
_delay_ms(100);

Lcd_cmd(0x8B);
Display(":");
_delay_ms(100);

Lcd_cmd(0x8C);
Display("...");
_delay_ms(100);

Lcd_cmd(0xC0);
Display("SOLAR TREE");
_delay_ms(1000);

Lcd_cmd(0X01);
_delay_ms(100);

Lcd_cmd(0X80);
Display("Project by");
_delay_ms(100);

Lcd_cmd(0x8A);
Display("....");
_delay_ms(100);

Lcd_cmd(0xC0);
Display("Arun,");
_delay_ms(100);

Lcd_cmd(0xC5);
Display("Bhup,");
_delay_ms(100);

Lcd_cmd(0xCB);
Display("Mohit,");
_delay_ms(100);




	
	
	
/************************LCD END***********************************/	
	
	while(1)
	{
//*****************************SERVO START*********************************//
		if(bit_is_clear(PINA, 0))             
		{
				Lcd_cmd(0x01);
				_delay_ms(100);
				Display("Charging");
				_delay_ms(100);
				Lcd_cmd(0x8A);
				_delay_ms(100);
				Display("...");
				_delay_ms(100);
				
			x = Rotation_Plus(p);
			_delay_ms(500);
		}
		
		else if(bit_is_clear(PINA, 1))
		{	
			Lcd_cmd(0x01);
			_delay_ms(100);
			Display("Charging");
			_delay_ms(100);
			Lcd_cmd(0x8A);
			_delay_ms(100);
			Display("...");
			_delay_ms(100);
							
			x = Rotation_Minus(p);
			_delay_ms(500);
		}
		
		//*******************************SERVO END**********************************//
		
		//*****************************SERVO 1 START*********************************//
		if(bit_is_clear(PINA, 2))
		{
			Lcd_cmd(0x01);
			_delay_ms(100);
			Display("Charging");
			_delay_ms(100);
			Lcd_cmd(0x8A);
			_delay_ms(100);
			Display("...");
			_delay_ms(100);
			
			y = Rotation_Plus_1(r);
			_delay_ms(500);
			
			PORTD &= ~(1<<PIND0);
			
		}
		
		else if(bit_is_clear(PINA, 3))
		{
			Lcd_cmd(0x01);
			_delay_ms(100);
			Display("Charging");
			_delay_ms(100);
			Lcd_cmd(0x8A);
			_delay_ms(100);
			Display("...");
			_delay_ms(100);
			
			y = Rotation_Minus_1(r);
			_delay_ms(500);
			
			PORTD &= ~(1<<PIND0);
		}
		
		//*******************************SERVO 1 END**********************************//
		
		
		else if(bit_is_clear(PINA, 4))  
		{                               
		   x = Reset_to_Zero(p);
			_delay_ms(500);
			
			y = Reset_to_Zero_1(r);
			_delay_ms(500);
			
			PORTD &= ~(1<<PIND0);
			
			Led_On();
			
			_delay_ms(100);
			PORTD &= ~(1<<PIND3);
			Lcd_cmd(0x01);
			_delay_ms(100);
			Display("LED OFF");
			_delay_ms(100);
			Lcd_cmd(0xC0);
			_delay_ms(100);
			Display("Day");
			_delay_ms(100);
		}
		
	
		else
		{
			x = Retain_Position(p);     //SERVO RETAIN POSITION
			_delay_ms(500);
			
			y = Retain_Position_1(r);     //SERVO RETAIN POSITION
			_delay_ms(500);
			
			PORTD &= ~(1<<PIND0);
		
		}
		p = x;
		r = y;
	
	}
}