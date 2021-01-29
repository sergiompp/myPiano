#include "stm32l1xx.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "Mi_Biblioteca.h"
#include "math.h"
int num =0;

void binary2Ascii(short mynum, unsigned char *mystring)
{
	if(mynum > 0){		
		mystring[0] = mynum/100000 + 0x30;
		mynum = mynum%100000; 
		mystring[1] = mynum/10000 + 0x30;
		mynum = mynum%10000; 
		mystring[2] = mynum/1000 + 0x30;
		mynum = mynum%1000; 
		mystring[3] = mynum/100 + 0x30;
		mynum = mynum%100; 
		mystring[4] = mynum/10 + 0x30;
		mynum = mynum%10; 
		mystring[5] = mynum + 0x30;

	} else if(mynum<0){
		
		mynum = mynum*(-1);
		mystring[0] = mynum/100000 + 0x30;
		mynum = mynum%100000; 
		mystring[1] = mynum/10000 + 0x30;
		mynum = mynum%10000; 
		mystring[2] = mynum/1000 + 0x30;
		mynum = mynum%1000; 
		mystring[3] = mynum/100 + 0x30;
		mynum = mynum%100; 
		mystring[4] = mynum/10 + 0x30;
		mynum = mynum%10; 
		mystring[5] = mynum + 0x30;
		mystring[0] = '-';
		
		
	
	}else{
		for(int i = 0;i<5;i++){
				mystring[i] = 0;
		}
	}
}

void espera_segundo(void)
{
	for(int i=0; i<50;i++)
	{
		for(int j=0; j<0xffff;j++)
		{}
	}
}

void espera(double nTime)
{
	for(double i=0; i<nTime; i++)
	{
		espera_segundo();
	}
}

void espera_decima_segundo(double i)
{
	for(i =0;i<32000;i++){}
}

void espera_milesima_segundo(double i){
	for(i=0; i<32;i++){}
}

unsigned int convierteDecimal(unsigned char* cadena)
{
	
	return num = (cadena[0]-48)*10+(cadena[1]-48);
}

