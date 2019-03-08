



#ifndef INCLUDES_H
#define INCLUDES_H


typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;
extern volatile u8 flag_100us;

extern volatile u8 cmdArrived;
extern volatile u8 cntRxd;
extern volatile u8 cmdArrived2;
extern volatile u8 cntRxd2;
extern volatile u8 bufRxd[40]; //
extern volatile u8 bufRxd2[40]; //
extern void UartRxMonitor(unsigned char ms);
extern void UartRxMonitor2(unsigned char ms);


extern volatile u8 send_succ_flag;//send 1 byte data successful

extern volatile u16 g_adc_result;





#endif













