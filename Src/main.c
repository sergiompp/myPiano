/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention	
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Mi_Biblioteca.h"
#include "stm32l1xx.h"
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

DAC_HandleTypeDef hdac;

LCD_HandleTypeDef hlcd;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

int valorTecla2DO=0;
int valorTecla1DO=0;
int valorTeclaDO=0;

int valorTecla2RE=0;
int valorTecla1RE=0;
int valorTeclaRE=0;

int valorTecla2MI=0;
int valorTecla1MI=0;
int valorTeclaMI=0;

int valorTecla2FA=0;
int valorTecla1FA=0;
int valorTeclaFA=0;

int valorTecla2SOL=0;
int valorTecla1SOL=0;
int valorTeclaSOL=0;

int valorTecla2LA=0;
int valorTecla1LA=0;
int valorTeclaLA=0;

int valorTecla2SI=0;
int valorTecla1SI=0;
int valorTeclaSI=0;

int pulsacion = 0;
int flag = 0;
int lenght = 0;
int sonido = 19;

int i = 0;
uint8_t seno[20] = {127,167,202,231,249,255,249,231,202,167,127,88,52,24,5,0,5,23,51,87};
int periodoDO = 0;
int periodoRE = 0;
int periodoMI = 0;
int periodoFA = 0;
int periodoSOL = 0;
int periodoLA = 0;
int periodoSI = 0;
	
// Valores ARR Notas Por Octavas	
static int periodoDO_Octava3 = 764;
static int periodoRE_Octava3 = 695;
static int periodoMI_Octava3 = 607;
static int periodoFA_Octava3= 573;
static int periodoSOL_Octava3 = 510;
static int periodoLA_Octava3 = 455;
static int periodoSI_Octava3 = 405;

static int periodoDO_Octava4	= 382;
static int periodoRE_Octava4 = 341;
static int periodoMI_Octava4 = 303;
static int periodoFA_Octava4= 286;
static int periodoSOL_Octava4 = 255;
static int periodoLA_Octava4 = 227;
static int periodoSI_Octava4 = 202;

// Variable para UART
// Simbolo = 4 / ASCII = 52 / Hex = 34
// Simbolo = 3 / ASCII = 51 / Hex = 33 	

unsigned char valorUART = '4'; 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_LCD_Init(void);
static void MX_TS_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	//----------- FUNCIONES -----------//
	void duracionMelodia(int duracion[], int n){
		TIM4->CNT= 0;
		TIM4->CCR1 = duracion[n]/20;
		TIM4->CR1 |= 0x0001;
	}
	void frecuenciaMelodia(int notas[], int valorCCR[], int n){
		TIM3->CNT= 0;
		TIM3->ARR = (notas[n])/20;
		TIM3->CCR1 = (valorCCR[n])/20;
		TIM3->CCER |= 0x0001;
		TIM3->CR1 |= 0x0001;
	}
	void funcionPIANO(void){
		BSP_LCD_GLASS_DisplayString((uint8_t *)" PIANO");
					
		valorTecla2DO = valorTecla1DO;
		valorTecla1DO = valorTeclaDO;
								
		valorTeclaDO = GPIOA->IDR & 0x0800;
											
		if(valorTecla2DO == 0x0800 && valorTecla1DO == 0 && valorTeclaDO == 0){
		
				TIM2->ARR = 381;	
				TIM2->CCR1 = 191; 		// Valor de ARR/2
				TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
										
			BSP_LCD_GLASS_Clear();						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" DO");
							
		} else if (valorTecla2DO == 0 && valorTecla1DO == 0x0800 && valorTeclaDO == 0x0800){
								
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
							
			TIM2->CNT = 0;
							
			BSP_LCD_GLASS_Clear();		
		}
							
		valorTecla2RE = valorTecla1RE;
		valorTecla1RE = valorTeclaRE;
					
		valorTeclaRE = GPIOA->IDR & 0x1000;
		if(valorTecla2RE == 0x1000 && valorTecla1RE == 0 && valorTeclaRE == 0){
					
			TIM2->ARR = 340;	
			TIM2->CCR1 = 170; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
					
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" RE");
							
		} else if (valorTecla2RE == 0 && valorTecla1RE == 0x1000 && valorTeclaRE == 0x1000){
						
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM2->CNT = 0;
							
			BSP_LCD_GLASS_Clear();
		}
							
		valorTecla2MI = valorTecla1MI;
		valorTecla1MI = valorTeclaMI;
		valorTeclaMI = GPIOB->IDR & 0x0004;
					
		if(valorTecla2MI == 0x0004 && valorTecla1MI == 0 && valorTeclaMI == 0){
					
			TIM2->ARR = 303;	
			TIM2->CCR1 = 152; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
						
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" MI");
						
		} else if (valorTecla2MI == 0 && valorTecla1MI == 0x0004 && valorTeclaMI == 0x0004){
						
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM2->CNT = 0;
						
			BSP_LCD_GLASS_Clear();
						
		}
						
		valorTecla2FA = valorTecla1FA;
		valorTecla1FA = valorTeclaFA;			
					
		valorTeclaFA  = GPIOC->IDR & 0x1000;
								
		if(valorTecla2FA == 0x1000 && valorTecla1FA == 0 && valorTeclaFA == 0){
				
			TIM2->ARR = 286;	
			TIM2->CCR1 = 143; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			
			BSP_LCD_GLASS_Clear();					
			BSP_LCD_GLASS_DisplayString((uint8_t *)" FA");
						
		} else if (valorTecla2FA == 0 && valorTecla1FA == 0x1000 && valorTeclaFA == 0x1000){
					
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM2->CNT = 0;
			
			BSP_LCD_GLASS_Clear();
		}
						
		valorTecla2SOL = valorTecla1SOL;
		valorTecla1SOL = valorTeclaSOL;
		valorTeclaSOL  = GPIOC->IDR & 0x2000;
										
		if(valorTecla2SOL == 0x2000 && valorTecla1SOL == 0 && valorTeclaSOL == 0){
					
			TIM2->ARR = 255;	
			TIM2->CCR1 = 128; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" SOL");
				
		} else if (valorTecla2SOL == 0 && valorTecla1SOL == 0x2000 && valorTeclaSOL == 0x2000){
						
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM2->CNT = 0;
										
				BSP_LCD_GLASS_Clear();
		}
						
		valorTecla2LA = valorTecla1LA;
		valorTecla1LA = valorTeclaLA;
		valorTeclaLA  = GPIOD->IDR & 0x0004;
								
		if(valorTecla2LA == 0x0004 && valorTecla1LA == 0 && valorTeclaLA == 0){
						
			TIM2->ARR = 227;	
			TIM2->CCR1 = 114; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" LA");
						
		} else if (valorTecla2LA == 0 && valorTecla1LA == 0x0004 && valorTeclaLA == 0x0004){
								
				TIM2->SR = 0; // Limpio los flags del contador
				TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
				TIM2->CNT = 0;
							
				BSP_LCD_GLASS_Clear();
		}
							
		valorTecla2SI = valorTecla1SI;
		valorTecla1SI = valorTeclaSI;
		valorTeclaSI  = GPIOH->IDR & 0x0002;
									
		if(valorTecla2SI == 0x0002 && valorTecla1SI == 0 && valorTeclaSI == 0){
							
			TIM2->ARR = 202;	
			TIM2->CCR1 = 101; 		// Valor de ARR/2
			TIM2->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			
			BSP_LCD_GLASS_Clear();					
			BSP_LCD_GLASS_DisplayString((uint8_t *)" SI");
						
		} else if (valorTecla2SI == 0 && valorTecla1SI == 0x0002 && valorTeclaSI == 0x0002){
										
			TIM2->SR = 0; // Limpio los flags del contador
			TIM2->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM2->CNT = 0;
						
			BSP_LCD_GLASS_Clear();
		}
		espera_milesima_segundo(85);
	}
	void funcionPIANO_OndaSenoidal(void){
		BSP_LCD_GLASS_DisplayString((uint8_t *)" PIANO");
					
		valorTecla2DO = valorTecla1DO;
		valorTecla1DO = valorTeclaDO;
								
		valorTeclaDO = GPIOA->IDR & 0x0800;
											
		if(valorTecla2DO == 0x0800 && valorTecla1DO == 0 && valorTeclaDO == 0){
		
			TIM3->ARR = periodoDO/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
								
			BSP_LCD_GLASS_Clear();						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" DO");
							
		} else if (valorTecla2DO == 0 && valorTecla1DO == 0x0800 && valorTeclaDO == 0x0800){
								
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
							
			TIM3->CNT = 0;
							
			BSP_LCD_GLASS_Clear();		
		}
		
		
		valorTecla2RE = valorTecla1RE;
		valorTecla1RE = valorTeclaRE;
					
		valorTeclaRE = GPIOA->IDR & 0x1000;
		if(valorTecla2RE == 0x1000 && valorTecla1RE == 0 && valorTeclaRE == 0){
					
			TIM3->ARR = periodoRE/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
										
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" RE");
							
		} else if (valorTecla2RE == 0 && valorTecla1RE == 0x1000 && valorTeclaRE == 0x1000){
						
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM3->CNT = 0;
							
			BSP_LCD_GLASS_Clear();
		}
							
		valorTecla2MI = valorTecla1MI;
		valorTecla1MI = valorTeclaMI;
		valorTeclaMI = GPIOB->IDR & 0x0004;
					
		if(valorTecla2MI == 0x0004 && valorTecla1MI == 0 && valorTeclaMI == 0){
					
			TIM3->ARR = periodoMI/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
							
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" MI");
						
		} else if (valorTecla2MI == 0 && valorTecla1MI == 0x0004 && valorTeclaMI == 0x0004){
						
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM3->CNT = 0;
						
			BSP_LCD_GLASS_Clear();
						
		}
						
		valorTecla2FA = valorTecla1FA;
		valorTecla1FA = valorTeclaFA;			
					
		valorTeclaFA  = GPIOC->IDR & 0x1000;
								
		if(valorTecla2FA == 0x1000 && valorTecla1FA == 0 && valorTeclaFA == 0){
				
			TIM3->ARR = periodoFA/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
			
			BSP_LCD_GLASS_Clear();					
			BSP_LCD_GLASS_DisplayString((uint8_t *)" FA");
						
		} else if (valorTecla2FA == 0 && valorTecla1FA == 0x1000 && valorTeclaFA == 0x1000){
					
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM3->CNT = 0;
			
			BSP_LCD_GLASS_Clear();
		}
						
		valorTecla2SOL = valorTecla1SOL;
		valorTecla1SOL = valorTeclaSOL;
		valorTeclaSOL  = GPIOC->IDR & 0x2000;
										
		if(valorTecla2SOL == 0x2000 && valorTecla1SOL == 0 && valorTeclaSOL == 0){
					
			TIM3->ARR = periodoSOL/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
			
			
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" SOL");
				
		} else if (valorTecla2SOL == 0 && valorTecla1SOL == 0x2000 && valorTeclaSOL == 0x2000){
						
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM3->CNT = 0;
										
				BSP_LCD_GLASS_Clear();
		}
						
		valorTecla2LA = valorTecla1LA;
		valorTecla1LA = valorTeclaLA;
		valorTeclaLA  = GPIOD->IDR & 0x0004;
								
		if(valorTecla2LA == 0x0004 && valorTecla1LA == 0 && valorTeclaLA == 0){
						
			TIM3->ARR = periodoLA/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
			
			
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" LA");
						
		} else if (valorTecla2LA == 0 && valorTecla1LA == 0x0004 && valorTeclaLA == 0x0004){
								
				TIM3->SR = 0; // Limpio los flags del contador
				TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
				TIM3->CNT = 0;
							
				BSP_LCD_GLASS_Clear();
		}
							
		valorTecla2SI = valorTecla1SI;
		valorTecla1SI = valorTeclaSI;
		valorTeclaSI  = GPIOH->IDR & 0x0002;
									
		if(valorTecla2SI == 0x0002 && valorTecla1SI == 0 && valorTeclaSI == 0){
							
			TIM3->ARR = periodoSI/20; 		
			TIM3->CR1 |= 0x0001; // CEN = 1 -> Arranco el contador
			TIM3->SR=1;
			
			
			BSP_LCD_GLASS_Clear();					
			BSP_LCD_GLASS_DisplayString((uint8_t *)" SI");
						
		} else if (valorTecla2SI == 0 && valorTecla1SI == 0x0002 && valorTeclaSI == 0x0002){
										
			TIM3->SR = 0; // Limpio los flags del contador
			TIM3->CR1 &=~ (0x0001);// CEN = 0 -> Desactivo el contador
			TIM3->CNT = 0;
						
			BSP_LCD_GLASS_Clear();
		}
	}
	
	void funcionMELODIAS(void){
		
		static int n=0;
		//Tecla DO
		if((GPIOA->IDR & 0x0800)== 0){
			// "ESCALA"
					
			// DO   RE  MI  FA SOL  LA  SI
			// 382 340 303 286 255 227 202
			// 191 170 152 143 128 114 101
						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" ESCALA");
			int notas[7]={382, 340, 303, 286, 255, 227, 202};
			int duracion[7]={400, 400, 400, 400, 400, 400, 400};
			int valorCCR[7] = {191, 170, 152, 143, 128, 114, 101};
			
			TIM4->SR = 0x0000;

			for(n =0; n<7; n++){
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
							
				espera_decima_segundo(85);
			}
			
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;					

		}
		//Tecla RE
		else if((GPIOA->IDR & 0x1000) == 0){
			// "TETRIS"
			
			//  LA  LA  MI  FA SOL  FA  MI  RE  RE  FA  LA SOL  FA  MI  MI  FA SOL  LA  FA  RE  RE
			// 227 227 303 286 255 286 303 340 340 286 227 255 286 303 303 286 255 227 286 340 340
			// 114 114 152 143 128 143 152 170 170 143 114 128 143 152 152 143 128 114 143 170 170
						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" TETRIS");
			int notas[21]={227, 227, 303, 286, 255, 286, 303, 340, 340, 286, 227, 255, 286, 303, 303, 286, 255, 227, 286, 340, 340};
			int duracion[21]={500, 500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500, 500};
			int valorCCR[21] = {114, 114, 152, 143, 128, 143, 152, 170, 170, 143, 114, 128, 143, 152, 152, 143, 128, 114, 143, 170, 170};
						
			TIM4->SR = 0x0000;
			
			for(n =0; n<21; n++){
							
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
				
				espera_decima_segundo(85);
			}	
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;	
		}
		//Tecla MI
		else if((GPIOB->IDR & 0x0004) == 0){
			// "HIMNO DE LA ALEGRIA"
						
			// MI   MI  FA SOL SOL FA   MI  RE  DO DO   RE  MI  MI RE
			// 303 303 286 255 255 286 303 340 382 382 340 303 303 340
			// 152 152 143 128 128 143 152 170 191 191 170 152 152 170
						
			BSP_LCD_GLASS_ScrollSentence((uint8_t *)" HIMNO DE LA ALEGRIA", 1, 40);
			int notas[14]={303, 303, 286, 255, 255, 286, 303, 340, 382, 382, 340, 303, 303, 340};
			int duracion[14]={700, 700, 700, 700, 700, 700, 700,700, 700, 700,700, 700, 700, 700};
			int valorCCR[14] = {152, 152, 143,128, 128, 143, 152, 170, 191, 191, 170, 152, 152, 170};
									
			TIM4->SR = 0x0000;
			
			for(n =0; n<14; n++){
							
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
							
				espera_decima_segundo(85);	
			}
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;
		}
		//Tecla FA
		else if((GPIOC->IDR & 0x1000) == 0){
			// "PIRATAS DEL CARIBE"
						
			//  RE  MI  FA SOL  LA SOL  FA  MI  FA SOL  LA SOL  FA SOL  LA SOL  FA  MI  FA  MI  RE  MI  DO  RE
			// 340 303 286 255 227 255 286 303 286 255 227 255 286 255 227 255 286 303 286 303 340 303 382 340
			// 170 152 143 128 114 128 143 152 143 128 114 128 143 128 114 128 143 152 143 152 170 152 191 170
						
			BSP_LCD_GLASS_ScrollSentence((uint8_t *)" PIRATAS DEL CARIBE", 1, 40);
			int notas[26]={340, 303, 286, 255, 227, 255, 286, 303, 286, 255, 227, 255, 286, 255, 227, 255, 286, 303, 286, 303, 340, 303, 382, 340};
			int duracion[26]={500, 500, 750, 250, 500, 500, 500, 500, 500, 500, 500, 1000, 250, 250, 750, 250, 500, 500, 500, 500, 750, 250, 500, 1000};
			int valorCCR[26] = {170, 152, 143, 128, 114, 128, 143, 152, 143, 128, 114, 128, 143, 128, 114, 128, 143, 152, 143, 152, 170, 152, 191, 170};
						
			TIM4->SR = 0x0000;
				
			for(n =0; n<26; n++){
				frecuenciaMelodia(notas, valorCCR, n);	
				duracionMelodia(duracion, n);
					
				espera_decima_segundo(85);
			}	
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;
		}
		//Tecla SOL
		else if((GPIOC->IDR & 0x2000) == 0){
			// ""
						
			//  RE  MI  FA SOL  LA SOL  FA  MI  FA SOL  LA SOL  FA SOL  LA SOL  FA  MI  FA  MI  RE  MI  DO  RE
			// 340 303 286 255 227 255 286 303 286 255 227 255 286 255 227 255 286 303 286 303 340 303 382 340
			// 170 152 143 128 114 128 143 152 143 128 114 128 143 128 114 128 143 152 143 152 170 152 191 170
						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" NADA");
			int notas[3]={382 , 340, 303};
			int duracion[3]={1000, 1000, 1000};
			int valorCCR[3] = {191,170,152};
						
			TIM4->SR = 0x0000;
			
			for(n =0; n<3; n++){
							
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
							
				espera_decima_segundo(85);	
			}	
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;
		}
		//Tecla LA
		else if((GPIOD->IDR & 0x0004) == 0){
			// "STAR WARS"
					
			//  RE  RE  RE SOL RE' DO'  SI  LA SOL' RE' DO'  SI  LA SOL' RE' DO'  SI DO'  LA  RE  RE  RE SOL RE' DO'  SI  LA SOL' RE' DO'  SI  LA SOL'  RE' DO'  SI DO' LA
			// 340 340 340 255 340 382 202 227 255  340 382 202 227 255  340 382 202 382 227 340 340 340 255 340 382 202 227 255  340 382 202 227 255  340  382 202 382 227
			// 170 170 170 128 170 191 101 114 128  170 191 101 114 128  170 191 101 191 114 170 170 170 128 170 191 101 114 128  170 191 101 114 128  170  170 101 170 114
						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" STAR WARS");
			int notas[38]={340, 340, 340, 255, 340, 382, 202, 227, 255, 340, 382, 202, 227, 255, 340, 382, 202, 382, 227, 340, 340, 340, 255, 340, 382, 202, 227, 255, 340, 382, 202, 227, 255, 340, 382, 202, 382, 227};
			int duracion[38]={250, 250, 250, 1000, 1000, 750, 250, 250, 1000, 750, 250, 250, 250, 1000, 500, 1000, 250, 1000, 250, 250, 250, 1000, 250, 250, 250, 1000, 250, 250, 250, 1000, 250, 1000, 250, 250, 250, 1000, 250, 1000};
			int valorCCR[38] = {170, 170, 170, 128, 170, 191, 101, 114, 128, 170, 191, 101, 114, 128, 170, 191, 101, 191, 114, 170, 170, 170, 128, 170, 191, 101, 114, 128, 170, 191, 101, 114, 128, 170, 170, 101, 170, 114};
						
			TIM4->SR = 0x0000;
			
			for(n =0; n<38; n++){
							
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
							
				espera_decima_segundo(5);	
			}
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;
		}
		//Tecla SI
		else if((GPIOH->IDR & 0x0002) == 0){
			// ""
						
			//  RE  MI  FA SOL  LA SOL  FA  MI  FA SOL  LA SOL  FA SOL  LA SOL  FA  MI  FA  MI  RE  MI  DO  RE
			// 340 303 286 255 227 255 286 303 286 255 227 255 286 255 227 255 286 303 286 303 340 303 382 340
			// 170 152 143 128 114 128 143 152 143 128 114 128 143 128 114 128 143 152 143 152 170 152 191 170
						
			BSP_LCD_GLASS_DisplayString((uint8_t *)" NADA");
			int notas[3]={382 , 340, 303};
			int duracion[3]={1000, 1000, 1000};
			int valorCCR[3] = {191,170,152};
						
			TIM4->SR = 0x0000;
			
			for(n =0; n<3; n++){
							
				duracionMelodia(duracion, n);
				frecuenciaMelodia(notas, valorCCR, n);	
							
				espera_decima_segundo(85);	
			}
			i=0;

			TIM3->CR1 = 0;
			DAC->DHR8R1 = 0;
		}
	}
	void config_OCTAVA3(void){
		periodoDO = periodoDO_Octava3;
		periodoRE = periodoRE_Octava3;
		periodoMI = periodoMI_Octava3;
		periodoFA = periodoFA_Octava3;
		periodoSOL = periodoSOL_Octava3;
		periodoLA = periodoLA_Octava3;
		periodoSI = periodoSI_Octava3;
	}
	void config_OCTAVA4(void){
		periodoDO = periodoDO_Octava4;
		periodoRE = periodoRE_Octava4;
		periodoMI = periodoMI_Octava4;
		periodoFA = periodoFA_Octava4;
		periodoSOL = periodoSOL_Octava4;
		periodoLA = periodoLA_Octava4;
		periodoSI = periodoSI_Octava4;
	}
	//----------- FUNCIONES IRQ's-----------//
	void EXTI0_IRQHandler(void){
		while (EXTI->PR!=0){
			EXTI->PR |= 0x01;
			if((GPIOC -> IDR & 0x00002000) !=0){
				if(pulsacion == 0){
					pulsacion = 1;
				}else if(pulsacion == 1){
					pulsacion =0;
				}
			}
		}
	} 

	void TIM4_IRQHandler(void){
		while((TIM4->SR & 0x0002 )!=0)		
		{
			TIM4->CNT= 0;
			TIM4->SR = 0x0000; 				// Limpio los flags del contador
			
			TIM2->CR1 &=~ (0x0001);
			TIM2->SR = 0x0000; 				// Limpio los flags del contador
		}
	}

	void TIM3_IRQHandler(void){
		if((TIM3->SR & 0x0002 )!=0)		
		{
			TIM3->SR=0;
			if(i<20){
				DAC->DHR8R1 = seno[i];
				i++;
			} 
			if(i ==19) {
				i=0;
			}
		}
		
	}
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
		HAL_UART_Receive_IT(&huart1, &valorUART, 1); // Vuelve a iniciar Rx por haber acabado el buffer
	}

	
	//Configuraci�n Perif�ricos
	//-----------------------------------------------------------------------//
	//											CONFIGURACIONES GPIO

	void config_GPIO(void){
			
			/*Configuraci�n pines para teclas siendo:
					PA11 - TECLA DO
					PA12 - TECLA RE
					PB2 - TECLA MI
					PC12 - TECLA FA
					PC13 - TECLA SOL
					PD2 - TECLA LA
					PH1 - TECLA SI*/

		// CONFIG. GPIOA

		/*
				* PA0 - EXTI0
				* PA5 - TIM2 Y TIM4
				* PA4 - DAC
				* PA11 - TECLA DO
				* PA12 - TECLA RE
				*/
				
				//Asegurar PA0 como Entrada
				//Entrada Digital (MODER = 00)
				GPIOA->MODER &= ~(0x00000003);	//Forzado a 0 PA
				
				//Asegurar PA5 como Salida de TIM2
				//Funci�n Alternativa (MODER = 10) con AF (AFR[0] = 0001) y AF (AFR[0] = 0001) 
				GPIOA->MODER &= ~(0x00000400);	//Forzado a 0 PA5
				GPIOA->AFR[0] |=0x00100000; //PA5 como TIM2 (AF1-0001)
				GPIOA->AFR[0] |=0x00200000; //PA5 como TIM4 (AF1-0001)
					
				//Asegurar PA4 como Entrada con Modo Anal�gico (MODER = 11)
				GPIOA->MODER |= (0x00000300);	//Forzado a 11 PA4

				//Asegurar PA11 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOA->MODER &= ~(0x00C00000);	//Forzado a 0 PA11
				GPIOA->PUPDR |= (0x00400000); //PA11 como Pull Up
				
				//Asegurar PA12 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOA->MODER &= ~(0x03000000);	//Forzado a 0 PA12
				GPIOA->PUPDR |= (0x01000000); //PA12 como Pull Up
			
		// CONFIG. GPIOB
			
				/*
				* PB2 - TECLA MI
				* PB6 - UART_Tx
				* PB7 - UART_Rx
				*/
				
				//Asegurar PB2 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOB->MODER &= ~(0x00000030);	//Forzado a 0 PB2
				GPIOB->PUPDR |= (0x00000010); //PB2 como Pull Up
				
				//Asegurar PB6 y PB7 como Funci�n Alternativa (MODER = 11) con AF (AFR[0] = 0001)
				GPIOB->MODER |= (0x01 << (2*7+1)); // AF => (10)
 			  GPIOB->MODER &= ~(0x01 << (2*7));
	 		  GPIOB->MODER |= (0x01 << (2*6+1)); // AF => (10)
		 	  GPIOB->MODER &= ~(0x01 << (2*6));
			  GPIOB->AFR[0] &= 0x00FFFFFF; // AF para UART => 7
			  GPIOB->AFR[0] |= 0x77000000;
				
		// CONFIG. GPIOC

				/*
				* PC12 - TECLA FA
				* PC13 - TECLA SOL
				*/
				
				//Asegurar PC12 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOC->MODER &= ~(0x03000000);	//Forzado a 0 PC12
				GPIOC->PUPDR |= (0x01000000); //PC12 como Pull Up

				//Asegurar PC13 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOC->MODER &= ~(0x0C000000);	//Forzado a 0 PC13
				GPIOC->PUPDR |= (0x04000000); //PC13 como Pull Up
			
		// CONFIG. GPIOD
			
				/*
				* PD2 - TECLA LA
				*/
				
				//Asegurar PD2 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOD->MODER &= ~(0x00000030);	//Forzado a 0 PD2
				GPIOD->PUPDR |= (0x00000010); //PD2 como Pull Up
			
		// CONFIG. GPIOH
			
				/*
				* PH1 - TECLA SI
				*/
				
				//Asegurar PH1 como Entrada Digital (MODER = 00) con Pull-Up (PUPDR = 01)
				GPIOH->MODER &= ~(0x0000000C);	//Forzado a 0 PH1
				GPIOH->PUPDR |= (0x00000004); //PH1 como Pull Up
	}
	//-----------------------------------------------------------------------//
	//							CONFIGURACION TIM2 Y TIM4
	void config_TIM2(void){
		//-----------------------------------------------------------------------//
		//									CONFIGURACION TIM2
			
				// Selecci�n del reloj interno: CR1, CR2, SMRC
				TIM2->CR1 = 0x0080;  // ARPE = 0 -> Es TOC
														 // CEN = 0; Contador apagado
				TIM2->CR2 = 0x0000;  // Siempre "0" en este curso
				TIM2->SMCR = 0x0000; // Siempre "0" en este curso
					
				// Configuraci�n del funcionamiento del contador: PSC, CNT, ARR y CCRx
				TIM2->PSC = 319; 	 				// Preescalado= 320 (Valor en PSC = Preescalado - 1) 
																//-> F_contador = 32000000/320 = 100000 pasos/segundo
				TIM2->CNT = 0; 			 		// Inicializo el valor del contador a cero
				TIM2->ARR = 382;  		// Valor recomendado = FFFF 
				TIM2->CCR1 = 191; 	 	// Registro donde se guarda el valor que marca la comparaci�n existosa. //DC
				
				//Selecci�n de IRQ o no: DIER
				TIM2->DIER = 0x0000; // No se genera INT al terminar de contar -> CCyIE = 0

				// Modo de salida del contador
				TIM2->CCMR1 = 0x0068; // CCyS = 0 (TOC, PWM)
															// OCyM = 110 (PWM con el 1er semiciclo a 1)
															// OCyPE = 1 (con precarga)
				TIM2->CCER = 0x0001;	// CCyNP = 0 (siempre en TOC)
															// CCyP = 0 (siempre en TOC)
															// CCyE = 1 (Activa la salida hardware)
															
				// Habilitaci�n de contador y limpieza de flags
				TIM2->EGR |= 0x0001; 	// UG = 1 -> Se genera evento de actualizaci�n
				TIM2->SR = 0; 				// Limpio los flags del contador
			
		
		//-----------------------------------------------------------------------//		
		}
	void config_TIM3(void){
		//-----------------------------------------------------------------------//
		//									CONFIGURACION TIM3
			
				// Selecci�n del reloj interno: CR1, CR2, SMRC
				TIM3->CR1 = 0x0000;  // ARPE = 0 -> Es TOC
														 // CEN = 0; Contador apagado
				TIM3->CR2 = 0x0000;  // Siempre "0" en este curso
				TIM3->SMCR = 0x0000; // Siempre "0" en este curso
					
				// Configuraci�n del funcionamiento del contador: PSC, CNT, ARR y CCRx
				TIM3->PSC = 319; 	 				// Preescalado= 320 (Valor en PSC = Preescalado - 1) 
																//-> F_contador = 32000000/320 = 100000 pasos/segundo
				TIM3->CNT = 0; 			 		// Inicializo el valor del contador a cero
				TIM3->ARR = 0xFFFF;  		// Valor recomendado = FFFF 
				//TIM3->CCR1 = 0; 	 	// Registro donde se guarda el valor que marca la comparaci�n existosa. //DC
				
				//Selecci�n de IRQ o no: DIER
				TIM3->DIER = 0x0001; // No se genera INT al terminar de contar -> CCyIE = 0

				// Modo de salida del contador
				TIM3->CCMR1 = 0x0068; // CCyS = 0 (TOC, PWM)
															// OCyM = 110 (PWM con el 1er semiciclo a 1)
															// OCyPE = 1 (con precarga)
				TIM3->CCER = 0x0001;	// CCyNP = 0 (siempre en TOC)
															// CCyP = 0 (siempre en TOC)
															// CCyE = 1 (Activa la salida hardware)
				
				// Habilitaci�n de contador y limpieza de flags
				TIM3->EGR |= 0x0001; 	// UG = 1 -> Se genera evento de actualizaci�n
				TIM3->SR = 0; 				// Limpio los flags del contador
				
				NVIC->ISER[0] |= (1<<29);
			
		
		//-----------------------------------------------------------------------//		
		}
	void config_TIM4(void){
		//-----------------------------------------------------------------------//
		//									CONFIGURACION TIM4 - IRQ
			
				// Selecci�n del reloj interno: CR1, CR2, SMRC
				TIM4->CR1 = 0x0000; 		// ARPE = 0 -> No es PWM, es TOC
																// CEN = 0; Contador apagado
				TIM4->CR2 = 0x0000; 		// Siempre "0" en este curso
				TIM4->SMCR = 0x0000; 		// Siempre "0" en este curso
					
				// Configuraci�n del funcionamiento del contador: PSC, CNT, ARR y CCRx
				TIM4->PSC = 319; 	 			// Preescalado= 320 (Valor en PSC = Preescalado - 1) 
																//-> F_contador = 32000000/320 = 100000 pasos/segundo
				TIM4->CNT = 0; 			 		// Inicializo el valor del contador a cero
				TIM4->ARR = 0xFFFF;  			// Valor recomendado = FFFF 
				
				//Selecci�n de IRQ o no: DIER
				TIM4->DIER = 0x0002; //Se genera INT al terminar de contar - CCyIE = 1
												
				// Modo de salida del contador
				TIM4->CCMR1 = 0x0000; // CCyS = 0 (TOC)
															// OCyM = 011(Toggle)
															// OCyPE = 0 (TOC) 
				TIM4->CCER = 0x0000;	// CCyNP = 0 (siempre en TOC)
															// CCyP = 0 (siempre en TOC)
															// CCyE = 0 (Desactiva la salida hardware)
															
				// Habilitaci�n de contador y limpieza de flags
				TIM4->EGR |= 0x0001; 	// UG = 1 -> Se genera evento de actualizaci�n
				TIM4->SR = 0; 				// Limpio los flags del contador
				
				//Habilitaci�n de la interrupci�n TIM4_IRQ en el NVIC (posici�n 30)
				NVIC->ISER[0] |= (1<<30);
		}
		
	//-----------------------------------------------------------------------//
	//									CONFIGURACION EXTI0
	void config_EXTI0(void){
			// Configuraci�n de EXTI0 por flanco de subida
			SYSCFG->EXTICR[0] = 0; 			// Dejo los bits 0-3 del GPIOA como fuente para EXTI
			EXTI->IMR |= 0x01; 					// S�lo se habilita y no se enmascara la EXTI para PA0
			EXTI->FTSR &= ~0x01; 				// Un �0� inhabilita el evento por flanco de bajada en EXTI con PA0
			EXTI->RTSR |= (0x01); 			// Un �1� habilita el evento por flanco de subida en EXT con PA0 
			NVIC->ISER[0] |= (1 << 6); 	// Habilito la EXTI0 en el NVIC (posici�n 6).
																	// Si se pulsa PA0, o sea hay una EXTI en PA0 (EXTI0), 
																	// salto a la RAI, consulto la EXTI0 en la RAI.
	}
		
	//-----------------------------------------------------------------------//

	//									CONFIGURACION DAC
	void config_DAC(void){
		//GPIOA->MODER Ya modificado
		DAC->CR = 0x00000001; // Configuration and enabling of DAC number 1
	}
		
	//-----------------------------------------------------------------------//

	//									CONFIGURACION USART
	void config_UART(void){
		USART1->CR1 = 0x0000000C; 		// Habilita transmisi�n y recepci�n
																	// UE = 0, 8 bits, 
																	// Sin Paridad, OVR8 = 16
																	// Sin Int, TE = 1, RE = 1
		USART1->CR2 = 0x00000000; 		// Numeros de bit de parada 1 (STOP=00)
		USART1->BRR = 0x00000DA7; 		// 9600 bauds
																	// Mantisa = 218 - 0xDA
																	// P. Fraccionaria = 7 - 0x07 		
		USART1->CR1 |= 0x01 << 13; 		// Habilita  USART
		USART1->CR1 |= 0x01 << 5; 		// RXNEIE=1 Una interrupcion de USART es generada 
	
		NVIC->ISER[1] |= (1 << (37-32)); //Habilita la NVIC  Como es superior a 37 se utiliza ISER[1]
	}
		
	//-----------------------------------------------------------------------//

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_LCD_Init();
  MX_TS_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
		
		//Configuraci�n Perif�ricos
		config_GPIO();
		config_TIM2();
		config_TIM3();
		config_TIM4();
		config_EXTI0();
		config_DAC();
		config_UART();
		
		//Inicializaci�n de RECV para la INT
		HAL_UART_Receive_IT(&huart1, &valorUART, 1); 
		
		//Configuraci�n LCD
		BSP_LCD_GLASS_Init();
		BSP_LCD_GLASS_BarLevelConfig(0);
		BSP_LCD_GLASS_Clear();
		BSP_LCD_GLASS_DisplayString((uint8_t *)" READY");
		espera(1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
 while (1)
  {
		BSP_LCD_GLASS_Clear();
		BSP_LCD_GLASS_DisplayString((uint8_t *)" GO");
		espera(1);
		BSP_LCD_GLASS_Clear();
		
		while(1){
			config_OCTAVA4();
			if(pulsacion == 0){
				
			//funcionPIANO();
			funcionPIANO_OndaSenoidal();
			
			} else if (pulsacion == 1){
				BSP_LCD_GLASS_DisplayString((uint8_t *)" MELODIA");
				funcionMELODIAS();
			}
				
			if(valorUART == '3'){
				
				valorUART='4';
				config_OCTAVA3();
				if(pulsacion == 0){
				
					//funcionPIANO();
					funcionPIANO_OndaSenoidal();
				
				} else if (pulsacion == 1){
					BSP_LCD_GLASS_DisplayString((uint8_t *)" MELODIA");
					funcionMELODIAS();
				}
			}
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LCD;
  PeriphClkInit.LCDClockSelection = RCC_RTCCLKSOURCE_LSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_DISABLE;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC3;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief LCD Initialization Function
  * @param None
  * @retval None
  */
static void MX_LCD_Init(void)
{

  /* USER CODE BEGIN LCD_Init 0 */

  /* USER CODE END LCD_Init 0 */

  /* USER CODE BEGIN LCD_Init 1 */

  /* USER CODE END LCD_Init 1 */
  hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_1;
  hlcd.Init.Divider = LCD_DIVIDER_31;
  hlcd.Init.Duty = LCD_DUTY_1_4;
  hlcd.Init.Bias = LCD_BIAS_1_3;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_0;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_4;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV32;
  if (HAL_LCD_Init(&hlcd) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LCD_Init 2 */

  /* USER CODE END LCD_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TS Initialization Function
  * @param None
  * @retval None
  */
static void MX_TS_Init(void)
{

  /* USER CODE BEGIN TS_Init 0 */

  /* USER CODE END TS_Init 0 */

  /* USER CODE BEGIN TS_Init 1 */

  /* USER CODE END TS_Init 1 */
  /* USER CODE BEGIN TS_Init 2 */

  /* USER CODE END TS_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : TECLA_SOL_Pin TECLA_FA_Pin */
  GPIO_InitStruct.Pin = TECLA_SOL_Pin|TECLA_FA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : TECLA_SI_Pin */
  GPIO_InitStruct.Pin = TECLA_SI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TECLA_SI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0_Pin */
  GPIO_InitStruct.Pin = PA0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PA0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ALTAVOZ___Onda_Senoidal_Pin */
  GPIO_InitStruct.Pin = ALTAVOZ___Onda_Senoidal_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ALTAVOZ___Onda_Senoidal_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TECLA_MI_Pin */
  GPIO_InitStruct.Pin = TECLA_MI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TECLA_MI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TECLA_DO_Pin TECLA_RE_Pin */
  GPIO_InitStruct.Pin = TECLA_DO_Pin|TECLA_RE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TECLA_LA_Pin */
  GPIO_InitStruct.Pin = TECLA_LA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TECLA_LA_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
