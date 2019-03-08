/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100AA
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 2019/1/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "includes.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

//timebase
enum
{
    timebase10ms=0,
    timebase20ms,
    timebase30ms,
    timebase40ms,
    timebase50ms
} timebase;


//slave status
typedef struct {
    u8 Address;
    u8 Lock_State;       //1 on ; 0 off
    u8 Box_State;        //0:null 1:right 2:error
    u8 Battery_ID[4];
    u8 Battery_Level;    //10=10%,20=20%
    u8 Battery_Temperature;
    u8 MCU_Temperature;
} slave;

slave cell_status;


u8 local_address = 0x01;  //box id


void crc16(u8 *ptr, u16 len);
//void UartRxMonitor(u8 ms);
volatile u8 flag_100us;
u8 flag_10ms;
//u8 flag_10ms;


//battery arguments
volatile u8 cmdArrived = 0;  //
volatile u8 cntRxd= 0;
volatile u8 bufRxd[40]; //
uint8_t tbuf[8];
uint8_t rbuf[16];
//mainbroad arguments
volatile u8 cmdArrived2 = 0;  //
volatile u8 cntRxd2= 0;
volatile u8 bufRxd2[40]; //
uint8_t tbuf2[13];
uint8_t rbuf2[16];

//u16 crc;
//u8 crch, crcl;
//u8 i;
//u8 len;
u8 buf[30];
u8 t_battery_command[4]={0xf0,0x03,0x05,0xb1};//read battery data command
/***********************CRC16.c***********************************/
u8 CRC[2];

void crc16(u8 *ptr, u16 len)
{
	unsigned long wcrc = 0xFFFF;
	u8 temp;
	int i = 0, j = 0;
	for (i = 0; i < len; i++)
	{
		temp = *ptr & 0x00FF;
		ptr++;
		wcrc ^= temp;
		for (j = 0; j < 8; j++)
		{
			if (wcrc & 0X0001)
			{
				wcrc >>= 1;
				wcrc ^= 0xA001;
			}
			else
			{
				wcrc >>= 1;
			}
		}
	}
	temp = wcrc;
	CRC[0] = wcrc;
	CRC[1] = wcrc >> 8;
}


/**
 * @brief  
 *
 * @param 
 *
 * @note   
 */
void delay(u16 ms) // 
{
	u16 ti;
	while(ms--)
	{
	for(ti=0;ti<112;ti++){}//112
	}
}
volatile u8 send_succ_flag=0;//send 1 byte data successful
void UartWrite(u8 *buf, u8 len) //
{
    
	P5.0=1;//enable 485 to send
    while (len--)   //????
    {
        //USART_SendData(USART3,*buf);
		R_UART2_Send(buf, 1);
        buf++;
		while(send_succ_flag==0);
		send_succ_flag=0;
        //while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
	delay(10);//about 316us
    //delay_us(50);
	P5.0=0;//enable 485 to receive
    
}



u8 UartRead(u8 *buf, u8 len) //
{
	u8 i;

	if (len > cntRxd) //
	{
		len = cntRxd; //
	}
	for (i = 0; i < len; i++) //
	{
		*buf = bufRxd[i];
		buf++;
	}
	cntRxd = 0;  //

	return len;  //
}
	
u8 able_485=0;//1:ok ,0:no ok
u8 unlock_succ=1;//default unlock status
u16 borrow_ok_count=0;//
u8 borrow_flag;//borrow successful
u16 Level[11] = {358,363,370,378,388,394,399,407,412,419,420};


u16 temperature_hex = 0;
float ref_hex,temp_hex;
void calculate_temperature(float ref_hex,float temp_hex)
{
	/* Temperature (25*C <-> 1.03V / -3.6mV/C) */
	float g_temperature = 0.0f;

	/* Temporary float variable */
	float g_temp = 0.0f;
	
	/* Calculate 1 a/d unit into mV */
    g_temp = (1450.0F / (float)ref_hex);

    /* Temperature sensor output in mV */
    g_temp = (float)temp_hex * g_temp;

    /* Subtract the offset voltage at 25 deg C */
    g_temp -= 1030.0F;

    /* Calculate the offset in deg C */
    g_temp /= 3.6F;
    
    /* Subtract the offset from nominal 25 deg C */
    g_temperature = 25.0F - g_temp;
    
    /* Save the integer and fraction parts of the calculated value as one integer value */
    //temperature_hex = (uint16_t)(g_temperature * 10.0F);
	temperature_hex = (uint16_t)g_temperature;
}

u16 ad_sample_buf[9];

void read_battery(void)
{
	static u8 count;//if count>10,clear battery buf
	u8 len;
	u8 val;

	u16 temp;
	float temp_12,temp_24,temp_36;
	
	//float ref_hex,temp_hex;
	
	if(unlock_succ==1)
	{
		if(cmdArrived==0)
		{
			borrow_ok_count++;
		}
		else
		{
			borrow_ok_count=0;
		}
		
		if(borrow_ok_count>100)//keep on 5s
		{
			borrow_flag=1;
		}	
	}
	
	if (cmdArrived)
	{
		able_485=1;//485 ok
		count=0;
		cmdArrived = 0;
		len = UartRead(rbuf, 16);
		if ((rbuf[0] == 0xf0)&&(rbuf[1]==0x0c))
		{
			crc16(rbuf, 14);
			if ((CRC[0] == rbuf[len-2]) && (CRC[1] == rbuf[len-1]))
			{
				cell_status.Box_State=0x01;//normal
				cell_status.Battery_ID[0]=rbuf[2];
				cell_status.Battery_ID[1]=rbuf[3];
				cell_status.Battery_ID[2]=rbuf[4];
				cell_status.Battery_ID[3]=rbuf[5];
				//temp=rbuf[6];
				temp=rbuf[6];
				temp=(temp<<8)|rbuf[7];
				temp_12 = temp * 0.012 / 3;
				temp=rbuf[8];
				temp=(temp<<8)|rbuf[9];
				temp_24 = temp * 0.2 / 6;
				temp=rbuf[10];
				temp=(temp<<8)|rbuf[11];
				temp_36= temp * 0.2 / 9;
				
				if(temp_12 > temp_24)
				{
					if(temp_24 > temp_36)
					{
						temp = temp_36 * 100;
					}
					else
					{
						temp = temp_24 * 100;
					}
				}
				else
				{
					if(temp_12 > temp_36)
					{
						temp = temp_36 * 100;
					}
					else
					{
						temp = temp_12 * 100;
					}
				}
				
				for(val = 0;val < 11 ;val++)
				{
					if(Level[val] > temp)
					{
						break;
					}
				}
				cell_status.Battery_Level= val * 10;
				cell_status.Battery_Temperature=rbuf[12];
				
				
				//ref_hex  = ad_sample_buf[8];
				//temp_hex = ad_sample_buf[7];
				//calculate_temperature(ref_hex,temp_hex);
				

				cell_status.MCU_Temperature=temperature_hex;
				
				
			}
		}
	}
	else
	{
		count++;
		if(count>10)
		{
			able_485=0;//485 no ok
			count=0;
			cell_status.Box_State=0x00;//null
			cell_status.Battery_ID[0]=0x00;
			cell_status.Battery_ID[1]=0x00;
			cell_status.Battery_ID[2]=0x00;
			cell_status.Battery_ID[3]=0x00;
			cell_status.Battery_Level=0x00;
			cell_status.Battery_Temperature=0x00;
			cell_status.MCU_Temperature=0x00;
		}

	}
}


u8 UartRead2(u8 *buf, u8 len) //
{
	u8 i;

	if (len > cntRxd2) //
	{
		len = cntRxd2; //
	}
	for (i = 0; i < len; i++) //
	{
		*buf = bufRxd2[i];
		buf++;
	}
	cntRxd2 = 0;  //

	return len;  //
}
u8 unlocking=0;//unlocking 
u16 unlocking_count=0;//unlock time counter,500ms

void unlock(void)
{
	if(unlocking==1)
	{
		P0.0 = 1;
		if(unlock_succ==0)
		{
			P1.0 = 1;
		}
		else
		{
			P1.0 = 0;
		}
		
		unlocking_count++;
		if(unlocking_count>=20)//1S out optical scope
		{
			if(P5.1==1)     // optocoupler enable
			{
				P1.0 = 0;
				cell_status.Lock_State=0x01;//unlock success
				unlock_succ=1;
				unlocking=0;
				P0.0 = 0;
				unlocking_count=0;
			}
		}
		
		if(unlocking_count>=100)//5s overtime
		{
				P1.0 = 0;           //relay control close
				cell_status.Lock_State=0x01;//unlock success
				unlock_succ=1;
				unlocking=0;
				P0.0 = 0;                  //optocoupler open
				unlocking_count=0;
		}
		
	}
}


u16 locking_count=0;//lock time counter,10s
//u16 lock_count=0;//lock time counter,10s
u8 locking=0;//locking 
void lock(void)
{
	if((borrow_flag==1)&&(able_485==1))//if battery out box
	{
		locking_count++;
		if(locking_count>80)
		{
			P0.0 = 1;
			P3.0=1;//close locker
			
			if(locking_count>100)//1S out optical scope
			{
				if(P5.1==1)     // optocoupler enable
				{
					P3.0 = 0;//stop close
					cell_status.Lock_State=0x00;//lock success
					unlock_succ=0;
					locking=0;
					P0.0 = 0;
					locking_count=0;
					
					borrow_flag=0;
					borrow_ok_count=0;
					
					unlocking=0;
				}
			}
		}
		if(locking_count>190)
		{
			P3.0 = 0;//stop close
			cell_status.Lock_State=0x00;//lock success
			unlock_succ=0;
			locking=0;
			P0.0 = 0;
			locking_count=0;	
			
			borrow_flag=0;
			borrow_ok_count=0;
			
			unlocking=0;
		}
	}
	else
	{
		P3.0 = 0;//stop close
		//cell_status.Lock_State=0x00;//lock success
		//unlock_succ=0;
		//locking=0;
		//P0.0 = 0;
		//locking_count=0;			
	}
}





void read_master(void)
{
	u8 len,i;
	double temp;
	
	if (cmdArrived2)
	{
		cmdArrived2 = 0;
		
		len = UartRead2(rbuf2, 7);
		
		if (rbuf2[0] == local_address)
		{
			
			crc16(rbuf2, len-2);
			if ((CRC[0] == rbuf2[len-2]) && (CRC[1] == rbuf2[len-1]))
			{
				switch(rbuf2[1])
				{
					case 0x03:
						tbuf2[0]=cell_status.Address;
						tbuf2[1]=0x03;
						tbuf2[2]=cell_status.Lock_State;
						tbuf2[3]=cell_status.Box_State;
						tbuf2[4]=cell_status.Battery_ID[0];
						tbuf2[5]=cell_status.Battery_ID[1];
						tbuf2[6]=cell_status.Battery_ID[2];
						tbuf2[7]=cell_status.Battery_ID[3];
						
						tbuf2[8]=cell_status.Battery_Level;
						
						tbuf2[9]=cell_status.Battery_Temperature;
						tbuf2[10]=cell_status.MCU_Temperature;
						crc16(tbuf2, 11);
						tbuf2[11]=CRC[0];
						tbuf2[12]=CRC[1];
						//P1.0^=1;
						//P5.0=1;//enable 485 to send
						//R_UART2_Send(tbuf2, 13);	
						UartWrite(tbuf2, 13);//send data to master

					break;
					
					case 0x06:
					
					if(rbuf2[3]==0x01)//received unlock command
					{
						unlocking=1;
						//P1.0 = 1;
					}
					
					break;
					
					default:
					
					break;
					
				}
			}	
		}
	}
	else
	{

	}
}

/**
 * @brief  
 *
 * @param 
 *
 * @note   
 */

void Delay(uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}


/* Declare a variable for A/D results */
volatile u16 g_adc_result = 0U;




u8 ad_channel[9]={_00_AD_INPUT_CHANNEL_0,_01_AD_INPUT_CHANNEL_1,_02_AD_INPUT_CHANNEL_2,_03_AD_INPUT_CHANNEL_3,
                  _10_AD_INPUT_CHANNEL_16,_12_AD_INPUT_CHANNEL_18,_13_AD_INPUT_CHANNEL_19,_80_AD_INPUT_TEMPERSENSOR_0,_81_AD_INPUT_INTERREFVOLT};
void ad_sample(void)
{
	static u8 channel_num=0,count=0;
	//ADS = ad_channel[channel_num++];
	ad_sample_buf[channel_num]=(ad_sample_buf[channel_num]+g_adc_result)>>1;
	
	count++;
	
	if(count>=10)
	{
		count=0;
		channel_num++;
		if(channel_num>=9)
		{
			channel_num=0;
		}
		
		R_ADC_Stop();
		
		ADS = ad_channel[channel_num];
		
		R_ADC_Start();
	}
}


void pwm_con(void)
{
	u16 pwm_now,pwm_max;
	static time_count=0;//over 80 oc time count;
	//channel pwm no.1
	pwm_max=TDR00;
	

	if(able_485 == 1)
	{
		//if(temperature_hex<=40||temperature_hex>255)
		if(cell_status.Battery_Temperature>0xb8)
		{
			time_count=0;//over temperature 30s time counter
			//channel pwm no.1
			pwm_now=TDR01;
			if(ad_sample_buf[0]>120)//charge current default 2A
			{
				if(pwm_now>=300)
				{
					pwm_now-=300;
				}
				else 
				{
					pwm_now=0;
				}
				
				TDR01 = pwm_now;
			}
			else if(ad_sample_buf[0]<100)
			{
				if(pwm_now<=pwm_max-300)
				{
					pwm_now+=300;
				}
				else
				{
					pwm_now=pwm_max;
				}

				TDR01 = pwm_now;
			}
			
			//channel pwm no.2
			pwm_now=TDR02;
			if(ad_sample_buf[1]>120)
			{
				if(pwm_now>=300)
				{
					pwm_now-=300;
				}
				else 
				{
					pwm_now=0;
				}
				TDR02 = pwm_now;
			}
			else if(ad_sample_buf[1]<100)
			{
				if(pwm_now<=pwm_max-300)
				{
					pwm_now+=300;
				}
				else
				{
					pwm_now=pwm_max;
				}
				TDR02 = pwm_now;
			}
			
			//channel pwm no.3
			pwm_now=TDR03;
			if(ad_sample_buf[2]>120)
			{
				if(pwm_now>=300)
				{
					pwm_now-=300;
				}
				else 
				{
					pwm_now=0;
				}
				TDR03 = pwm_now;
			}
			else if(ad_sample_buf[2]<100)
			{
				if(pwm_now<=pwm_max-300)
				{
					pwm_now+=300;
				}
				else
				{
					pwm_now=pwm_max;
				}
				TDR03 = pwm_now;
			}			
		}
		else
		{	
			time_count++;
			if(time_count>100)//>80 oc  over 5s time 
			{
				time_count=0;
				//channel pwm no.1
				pwm_now=TDR01;
				if(pwm_now>=3000)//PWM 10% range go down
				{
					pwm_now-=3000;
				}
				else 
				{
					pwm_now=0;
				}
				TDR01=pwm_now;	
				//channel pwm no.2
				pwm_now=TDR02;
				if(pwm_now>=3000)//PWM 10% range go down
				{
					pwm_now-=3000;
				}
				else 
				{
					pwm_now=0;
				}
				TDR02=pwm_now;	
				//channel pwm no.1
				pwm_now=TDR03;
				if(pwm_now>=3000)//PWM 10% range go down
				{
					pwm_now-=3000;
				}
				else 
				{
					pwm_now=0;
				}
				TDR03=pwm_now;					
			}

		}
	}
	else
	{
		TDR01 = TDR00/10;//IF BATTERY ELECTRIC IS ZERO,CHARGE FOR  IT
		TDR02 = 0;
		TDR03 = 0;
	}
}

void addr_name(void)
{
	u8 temp=0;
	if(P13.7)
	{
		temp = 0x01;
	}
	if(P12.2)
	{
		temp = temp | 0x02;
	}
	if(P12.1)
	{
		temp = temp |0x04;
	}
	local_address = temp;
	cell_status.Address=local_address;//first box address
}


/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//float ref_hex;
//float temp_hex;
void main(void)
{

    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

	R_TAU0_Channel6_Start();
	R_TAU0_Channel7_Start();
	R_UART0_Start();
	R_UART2_Start();
	
    /* Enable the ADC comparator */
    R_ADC_Set_OperationOn();
    /* Enable ADC module */
    R_ADC_Start();
	
	R_TAU0_Channel0_Start();
	

	
	P5.0=0;//enable to master receive 485 

	while (1U)
	{
		if (flag_100us==1)
		{
			flag_100us=0;
			
			//AD SAMPLE
			ad_sample();
			
			flag_10ms++;
			if(flag_10ms>100)
			{
				flag_10ms=0;
				//MASTER READ BAX CONTROLER
				read_master();
				
				switch(timebase)
				{
					case timebase10ms:
					
					//SEND READ COMMAND TO BATTERY
					R_UART0_Send(t_battery_command, 4);

					//ADDRESS JUDGE
					addr_name();
					
					timebase=timebase20ms;
					break;
					
					case timebase20ms:
					
					//RS485 READ BATTERY
					read_battery();
	
					timebase=timebase30ms;
					break;	
					
					case timebase30ms:
					
					//WATCH DOG
					R_WDT_Restart();
					
					
					timebase=timebase40ms;
					break;
					
					case timebase40ms:
					
					//LOCK OR UNLOCK
					if(unlock_succ==0)
					{
						unlock();
					}
					else
					{
						lock();	
					}
					
					//CALCULATE MCU TEMPERATURE
					ref_hex  = ad_sample_buf[8];//
					temp_hex = ad_sample_buf[7];//
					calculate_temperature(ref_hex,temp_hex);
					//cell_status.MCU_Temperature=temperature_hex;
					
					timebase=timebase50ms;
					break;
					
					case timebase50ms:
					
					//PWM CONTROL
					pwm_con();
					
					timebase=timebase10ms;
					break;
					
					default:
					
					break;
					
				}				
			}
		}
	}
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	EI();
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
