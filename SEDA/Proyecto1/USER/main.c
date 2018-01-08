#include <LPC17xx.H>
#include "init.h"
#include <i2c_lpc17xx.h>

/* ---------------------------------------------------- Variables ----------------------------------------------------*/
uint32_t temp_LM35 = 0,humedad = 0;
volatile uint32_t frec_anemometro;

/* ---------------------------------------------------- Funciones de atenci�n a la interrupci�n ----------------------------------------------------*/

void TIMER0_IRQHandler (void)				// Interrumpe cada segundo
{
	
	LPC_ADC->ADCR|=(1<<16); // BURST=1 --> Cada 65TclkADC se toma una muestra de cada canal comenzando desde el m�s bajo (bit LSB de CR[0..7])
	LPC_TIM0->IR|=(1<<1);		// Borrar flag interrupci�n		
	
	frec_anemometro = LPC_TIM3->TC; //Quedar�a obtener la velocidad del viento
	
	/* Mandar valores por UART
		 Mandar valores a LCD
		 Leer frecuencia anem�metro
		 Iniciar conversi�n ADC (Hay que pensar si vamos a usar modo burst o lo vamos a hacer manual)
		 RESET contador CAP
		 Quitamos RESET contador CAP
		 Quitamos flag de interrupci�n de este timer
	*/
}

void TIMER1_IRQHandler(void)
{	


	LPC_TIM1->IR|=(1<<1);		// Borrar flag interrupci�n								
}


void TIMER2_IRQHandler(void)
{	


	LPC_TIM2->IR|=(1<<1);		// Borrar flag interrupci�n								
}


void TIMER3_IRQHandler(void)
{	


	LPC_TIM3->IR|=(1<<1);		// Borrar flag interrupci�n								
}

void ADC_IRQHandler(void)
{
	
	LPC_ADC->ADCR&=~(1<<16); // BURST=0     // Deshabilitamos el modo R�faga (ojo continua la conversi�n del siguiente canal) 
  
	//Almacenamos las muestras
	temp_LM35 = (((LPC_ADC->ADDR0 >>4)&0xFFF)*3.3/4095)*100;	//Temperatura LM35 en �C
	humedad = ((((LPC_ADC->ADDR2 >>4)&0xFFF)*3.3/4095)-0.772)/0.03;	//%Humedad relativa
}

void config_DS1621(void)
{
	I2CSendAddr(0x48,0);
	I2CSendByte(0xAC);
	I2CSendByte(0x02);
	I2CSendStop();
	I2Cdelay();
	I2CSendAddr(0x48,0);
	I2CSendByte(0xEE);
	I2CSendStop();
}

unsigned char leer_DS1621(unsigned char ACK)
{
	I2CSendAddr(0x48,0);
	I2CSendByte(0xAA);
	I2CSendAddr(0x48,1);
	return I2CGetByte(ACK);
}


/* --------------------------------------------------------------- Programa Principal ---------------------------------------------------------------*/
int main(void)
{
	init_GPIO();
	init_ADC_sensores();
	/*
		1. Inicializar pines
		2. Inicializar ADC
		3. Inicializar FSM(?) - Vamos a usar una m�quina de estados para crear un men� en el LCD??
		4. Inicializar LCD con LCD_Initialization()
	*/
	
	//while (1);
	
}
