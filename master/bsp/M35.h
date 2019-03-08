
#ifndef __M35_H
#define __M35_H


/**********M35.C结构定义*********/

/*********M35.C函数定义**********/
void M35_Start(void);
ErrorStatus M35_HTTPConnect(void);
u8 SIM_SendCmd(char *Cmd,char *Re1,char *Re2,char *Re3,u32 TimeOut,u8 time);
#endif
