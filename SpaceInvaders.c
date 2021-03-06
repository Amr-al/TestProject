
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "time.h"
#include "stdlib.h"

void PortF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;


// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10

const unsigned char car[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xF0,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char enemy[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};


// *************************** Capture image dimensions out of BMP**********

#define carW        ((unsigned char)car[18])// h=10,w=14
#define carH        ((unsigned char)car[22])
#define enemyW        ((unsigned char)enemy[18])
#define enemyH        ((unsigned char)enemy[22])


// ***************************** Variables ************************ //

int  y,speed=1 ;			  
int eMove,Y_position,Y_position2;		//enemies position and movement
int tracks[3] = {10, 28, 46};										//
int score;

int main(void){
	
	// ******************** initializations ***************************** //
	PortF_Init();
	
	y = 28;		//mid screean y = 28
	eMove = 84-enemyW;


	score = 0;
	
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
 Random_Init(1);
	
	
	Y_position = rand()%3;
	Y_position2= rand()%3;
	if(Y_position==Y_position2)Y_position2 =(Y_position2+1)%3 ;
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	
	// ***************************** Welcome screen *************************** //
	Nokia5110_Clear();

  Nokia5110_SetCursor(0, 2);
  Nokia5110_OutString("starting the");
  Nokia5110_SetCursor(1, 3);
	Nokia5110_OutString("racing game");


  Delay100ms(50);

	// ***************************** Some delay before begining ************************ //
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, y, car, 0);
	Nokia5110_DisplayBuffer();
	Delay100ms(30);
	
	// ***************************** Game loop ************************ //
	while(1){
		
		Nokia5110_ClearBuffer();		
		
									//***************** enemy movement ********************
		Nokia5110_PrintBMP(0,y, car, 0);
		if(eMove > 0){
			Nokia5110_PrintBMP(eMove, tracks[Y_position], enemy, 0);
			Nokia5110_PrintBMP(eMove, tracks[Y_position2], enemy, 0);
			eMove-=speed;
		}
		
		if(eMove <= 0){
			
				score++;					//number of passed enemys
				Y_position = rand()%3;
				Y_position2 = rand()%3;
				if(Y_position==Y_position2)Y_position2 = rand()%3;
				eMove = 84-enemyW;
				if(score%5==0&&speed<10)speed++;
			Delay100ms(10);
			Nokia5110_PrintBMP(eMove, tracks[Y_position], enemy, 0);
			Nokia5110_PrintBMP(eMove, tracks[Y_position2], enemy, 0);
			
		}
		
		if(eMove <= carW-1 && (tracks[Y_position] == y||tracks[Y_position2] == y)){
			break;
		}
				
								//***************** Screen displaying ********************
		Nokia5110_DisplayBuffer();
		Delay100ms(150);
		
		
	}

	Delay100ms(50);				//Delay before gameover screen

						 //***************** Gameover screen ********************//
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("your score is");
	Nokia5110_SetCursor(3, 3);
  Nokia5110_OutUDec(score);
	NVIC_EN0_R=0x00000000;//disable interrupt
  while(1){
  }

}
//end of main

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 1<<5;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R =1<<1 | 1<<2 | 1<<3;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R =0x00;       // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0 

	GPIO_PORTF_IS_R=0;	//level or edge 
	GPIO_PORTF_IBE_R=0;	//both edge or not
	GPIO_PORTF_IEV_R=0;	//falling or rising
	GPIO_PORTF_ICR_R=0x11;	//clear interrup 
	GPIO_PORTF_IM_R|=0x11;	//enable interrup on pins (interrupt mask)
	NVIC_PRI7_R=(NVIC_PRI7_R&0xFF00FFFF)|0x00A00000;	//priority   				portf ->IRQ(30)   30/4=7  30%4=2  1010
	NVIC_EN0_R=0x40000000;	//Enable interrupts on INVIC 			30 /32
}

void GPIOPortF_Handler(void){
	if((GPIO_PORTF_RIS_R&0x01)!=0&&y<46){//sw2
			y += 18;
		GPIO_PORTF_ICR_R=0x01;	
	}
	else if((GPIO_PORTF_RIS_R&0x10)!=0&&y>10){//sw1
			y -= 18;
		GPIO_PORTF_ICR_R=0x10;
	}
	else GPIO_PORTF_ICR_R=0x11;
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240/80;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
