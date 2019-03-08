

#include "main.h"


/**
  * @brief  启动或重启M35模块.
  * @param  None   csic.taobao.com
  * @retval None
  */
void M35_Start(void)
{
    delay_s(2);
    M35_USARTx_Config(9600); //初始化M3串口（串口三）
    delay_s(1);
    USART1_Config(9600);//初始化串口一
}
/**
  * @brief  M35模块与HTTP建立链接.
  * @param  None	csic.taobao.com
  * @retval ErrorStatus
  */
ErrorStatus M35_HTTPConnect(void)
{
    u8 data=0,ret=0;
    SIM_SendCmd("AT+IPR=115200\r\n","\r\nOK","NULL","NULL",1000,1);//设置M35模块波特率为115200
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("ATE0&W\r\n","\r\nOK","NULL","NULL",1000,1);//保存波特率设置
    PrintfClear();
    SIM_SendCmd("NULL","\r\nCall Ready","NULL","NULL",1000,1);//初始化完成，上报Call Ready信息
    PrintfClear();
    delay_ms(100);
    SIM_SendCmd("AT+CFUN=1\r\n","\r\nOK","NULL","NULL",1000,1);//设置模块全功能模式（默认为1）
    PrintfClear();
    delay_ms(100);
    if (SIM_SendCmd("AT+CPIN?\r\n","\r\n+CPIN: READY\r\n\r\nOK","NULL","NULL",1000,1) != 1) //查询是否需要输入PIN码
    {
        PrintfClear();
        return ERROR;
    }
    PrintfClear();
    //LED_WriteBit(LED_IO, LED_D8, Bit_RESET);									      //SIM卡就绪 点亮D8
    delay_ms(100);
    data = 0;
    //查询GSM网络注册状态，确认找网成功
    while (SIM_SendCmd("AT+CREG?\r\n","\r\n+CREG: 0,5","NULL","NULL",1000,1) != 1 && data < 10)
    {
        PrintfClear();
        delay_ms(100);
        data++;
    }
    PrintfClear();
    if (data == 10)
    {
        return ERROR;                                                                             //找网不成功模块重启
    }
    else
    {
        //LED_WriteBit(LED_IO, LED_D7 | LED_D8, Bit_RESET);						  //确认找网成功点亮D7
    }
    data = 0;
    while (SIM_SendCmd("AT+CGATT?\r\n","\r\n+CGATT: 1","NULL","NULL",1000,1) != 1 && data < 10) //确认GPRS附着成功，+CGATT: 1表示GPRS已附着
    {
        PrintfClear();
        delay_ms(100);
        data++;
    }
    if (data == 10)
    {
        return ERROR;                                                                            //GPRS附着不成功模块重启
    }
    else
    {
        //LED_WriteBit(LED_IO, Relay_B | LED_D7 | LED_D8, Bit_RESET);		     //确认GPRS附着成功,点亮D6
    }
    SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",1000,1);//设置前台GPRS场景
    PrintfClear();
    delay_ms(100);
    data = SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",1000,1); //判断运营商
    PrintfClear();
    delay_ms(100);
    if (data == 1)	  //中国移动
    {
        SIM_SendCmd("AT+QICSGP=1,\"CMNET\"\r\n","\r\nOK","NULL","NULL",1000,1);//选择GPRS连接模式,移动联通都不一样
        PrintfClear();
        delay_ms(100);
    }
    else if (data == 2)  //中国联通
    {
        SIM_SendCmd("AT+QICSGP=1,\"UNINET\"\r\n","\r\nOK","NULL","NULL",1000,1);//选择GPRS连接模式,移动联通都不一样
        PrintfClear();
        delay_ms(100);
    }
    SIM_SendCmd("AT+QIREGAPP\r\n","\r\nOK","NULL","NULL",2000,1);//设置APN,初始化TCPIP任务
    PrintfClear();
    delay_ms(100);
    SIM_SendCmd("AT+QIACT\r\n","\r\nOK","NULL","NULL",2000,1);//激活GPRS链接,（就像我们打开数据流量一样）
    PrintfClear();
		delay_ms(100);
		SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1);//获取基站定位信息
		PrintfClear();
		delay_ms(500);

    SIM_SendCmd("AT+QINDI=0\r\n","\r\nOK","NULL","NULL",1000,1);
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QIHEAD=1\r\n","\r\nOK","NULL","NULL",1000,1);//设置数据头信息
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QISHOWRA=0\r\n","\r\nOK","NULL","NULL",1000,1);//设置接收到的数据头位置是否增加地址和端口号
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QISHOWPT=0\r\n","\r\nOK","NULL","NULL",1000,1);//设置接收到的数据头位置是否增加传输协议类型
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QIMODE=0\r\n","\r\nOK","NULL","NULL",1000,1);//设置传输方式
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QITCFG=3,2,512,1\r\n","\r\nOK","NULL","NULL",1000,1);//设置等待时间和数据组长度
    delay_ms(100);
    PrintfClear();
    data=SIM_SendCmd("AT+QIOPEN=\"TCP\",\"39.108.64.190\",\"6602\"\r\n","\r\nOK","ALREADY CONNECT","CONNECT OK",2000,1);//向服务器发送TCP链接，建立TCP连接
		//data=SIM_SendCmd("AT+QIOPEN=\"TCP\",\"47.98.103.160\",\"6601\"\r\n","\r\nOK","ALREADY CONNECT","CONNECT OK",2000,1);//向服务器发送TCP链接，建立TCP连接
    ret = SIM_SendCmd("NULL","CONNECT OK","NULL","NULL",2000,2);//判断是否连接成功

    printf("***************data=%d ,ret = %d\r\n",data,ret);
    PrintfClear();

    if (data == 1 || data == 2 || data == 3 || ret==1)
    {
        //LED_WriteBit(RELAY_IO, RELAY_A | Relay_B , Bit_RESET);
        //LED_WriteBit(LED_IO,  LED_D7 | LED_D8, Bit_RESET);
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}
/**
  * @brief  通过串口往M35发送AT指令或者数据.
  * @param  *Cmd:要发送的内容 *Re1:M35可能回复
  * @retval None	csic.taobao.com
  */
u8 SIM_SendCmd(char *Cmd,char *Re1,char *Re2,char *Re3,u32 TimeOut,u8 time)
{
    u8 i;
    u32 j;

    M35REBUFStructure.flay = RESET;
    M35REBUFStructure.cnt	= 0;

    if (strcmp(Cmd,"NULL"))
    {
        Usart_SendString( M35_USART,Cmd);//串口三发送指令给M35
        printf(Cmd);//串口1发送到串口
    }

    for (i = 0; i < time; i++) //超时
    {
        for (j = 0; j < TimeOut; j++)
        {
            delay_ms(1);
            if (M35REBUFStructure.flay == SET)
            {
                if (strstr((char*)(M35REBUFStructure.M35Rebuf),Re1))
                {
                    return 1;
                }
                if (strstr((char*)(M35REBUFStructure.M35Rebuf),Re2))
                {
                    return 2;
                }
                if (strstr((char*)(M35REBUFStructure.M35Rebuf),Re3))
                {
                    return 3;
                }
            }
        }
    }
    return 0;
}

