

#include "main.h"


/**
  * @brief  ����������M35ģ��.
  * @param  None   csic.taobao.com
  * @retval None
  */
void M35_Start(void)
{
    delay_s(2);
    M35_USARTx_Config(9600); //��ʼ��M3���ڣ���������
    delay_s(1);
    USART1_Config(9600);//��ʼ������һ
}
/**
  * @brief  M35ģ����HTTP��������.
  * @param  None	csic.taobao.com
  * @retval ErrorStatus
  */
ErrorStatus M35_HTTPConnect(void)
{
    u8 data=0,ret=0;
    SIM_SendCmd("AT+IPR=115200\r\n","\r\nOK","NULL","NULL",1000,1);//����M35ģ�鲨����Ϊ115200
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("ATE0&W\r\n","\r\nOK","NULL","NULL",1000,1);//���沨��������
    PrintfClear();
    SIM_SendCmd("NULL","\r\nCall Ready","NULL","NULL",1000,1);//��ʼ����ɣ��ϱ�Call Ready��Ϣ
    PrintfClear();
    delay_ms(100);
    SIM_SendCmd("AT+CFUN=1\r\n","\r\nOK","NULL","NULL",1000,1);//����ģ��ȫ����ģʽ��Ĭ��Ϊ1��
    PrintfClear();
    delay_ms(100);
    if (SIM_SendCmd("AT+CPIN?\r\n","\r\n+CPIN: READY\r\n\r\nOK","NULL","NULL",1000,1) != 1) //��ѯ�Ƿ���Ҫ����PIN��
    {
        PrintfClear();
        return ERROR;
    }
    PrintfClear();
    //LED_WriteBit(LED_IO, LED_D8, Bit_RESET);									      //SIM������ ����D8
    delay_ms(100);
    data = 0;
    //��ѯGSM����ע��״̬��ȷ�������ɹ�
    while (SIM_SendCmd("AT+CREG?\r\n","\r\n+CREG: 0,5","NULL","NULL",1000,1) != 1 && data < 10)
    {
        PrintfClear();
        delay_ms(100);
        data++;
    }
    PrintfClear();
    if (data == 10)
    {
        return ERROR;                                                                             //�������ɹ�ģ������
    }
    else
    {
        //LED_WriteBit(LED_IO, LED_D7 | LED_D8, Bit_RESET);						  //ȷ�������ɹ�����D7
    }
    data = 0;
    while (SIM_SendCmd("AT+CGATT?\r\n","\r\n+CGATT: 1","NULL","NULL",1000,1) != 1 && data < 10) //ȷ��GPRS���ųɹ���+CGATT: 1��ʾGPRS�Ѹ���
    {
        PrintfClear();
        delay_ms(100);
        data++;
    }
    if (data == 10)
    {
        return ERROR;                                                                            //GPRS���Ų��ɹ�ģ������
    }
    else
    {
        //LED_WriteBit(LED_IO, Relay_B | LED_D7 | LED_D8, Bit_RESET);		     //ȷ��GPRS���ųɹ�,����D6
    }
    SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",1000,1);//����ǰ̨GPRS����
    PrintfClear();
    delay_ms(100);
    data = SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",1000,1); //�ж���Ӫ��
    PrintfClear();
    delay_ms(100);
    if (data == 1)	  //�й��ƶ�
    {
        SIM_SendCmd("AT+QICSGP=1,\"CMNET\"\r\n","\r\nOK","NULL","NULL",1000,1);//ѡ��GPRS����ģʽ,�ƶ���ͨ����һ��
        PrintfClear();
        delay_ms(100);
    }
    else if (data == 2)  //�й���ͨ
    {
        SIM_SendCmd("AT+QICSGP=1,\"UNINET\"\r\n","\r\nOK","NULL","NULL",1000,1);//ѡ��GPRS����ģʽ,�ƶ���ͨ����һ��
        PrintfClear();
        delay_ms(100);
    }
    SIM_SendCmd("AT+QIREGAPP\r\n","\r\nOK","NULL","NULL",2000,1);//����APN,��ʼ��TCPIP����
    PrintfClear();
    delay_ms(100);
    SIM_SendCmd("AT+QIACT\r\n","\r\nOK","NULL","NULL",2000,1);//����GPRS����,���������Ǵ���������һ����
    PrintfClear();
		delay_ms(100);
		SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1);//��ȡ��վ��λ��Ϣ
		PrintfClear();
		delay_ms(500);

    SIM_SendCmd("AT+QINDI=0\r\n","\r\nOK","NULL","NULL",1000,1);
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QIHEAD=1\r\n","\r\nOK","NULL","NULL",1000,1);//��������ͷ��Ϣ
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QISHOWRA=0\r\n","\r\nOK","NULL","NULL",1000,1);//���ý��յ�������ͷλ���Ƿ����ӵ�ַ�Ͷ˿ں�
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QISHOWPT=0\r\n","\r\nOK","NULL","NULL",1000,1);//���ý��յ�������ͷλ���Ƿ����Ӵ���Э������
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QIMODE=0\r\n","\r\nOK","NULL","NULL",1000,1);//���ô��䷽ʽ
    delay_ms(100);
    PrintfClear();
    SIM_SendCmd("AT+QITCFG=3,2,512,1\r\n","\r\nOK","NULL","NULL",1000,1);//���õȴ�ʱ��������鳤��
    delay_ms(100);
    PrintfClear();
    data=SIM_SendCmd("AT+QIOPEN=\"TCP\",\"39.108.64.190\",\"6602\"\r\n","\r\nOK","ALREADY CONNECT","CONNECT OK",2000,1);//�����������TCP���ӣ�����TCP����
		//data=SIM_SendCmd("AT+QIOPEN=\"TCP\",\"47.98.103.160\",\"6601\"\r\n","\r\nOK","ALREADY CONNECT","CONNECT OK",2000,1);//�����������TCP���ӣ�����TCP����
    ret = SIM_SendCmd("NULL","CONNECT OK","NULL","NULL",2000,2);//�ж��Ƿ����ӳɹ�

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
  * @brief  ͨ��������M35����ATָ���������.
  * @param  *Cmd:Ҫ���͵����� *Re1:M35���ܻظ�
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
        Usart_SendString( M35_USART,Cmd);//����������ָ���M35
        printf(Cmd);//����1���͵�����
    }

    for (i = 0; i < time; i++) //��ʱ
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

