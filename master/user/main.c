


#include "main.h"

typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

__IO u8 flag5ms;
__IO u16 flag30s;
__IO u16 flag10s;

enum
{
    timebase20ms=0,
    timebase40ms,
    timebase60ms,
    timebase80ms,
    timebase100ms
} timebase;

//timebase timebaseflag;

/*����״̬��Ϣ*/
typedef struct
{
    u8   cell_id;
    u8   cell_status;
    u8   battery_id[4];
    u8   battery_quantity;
} CELL;

/*����״̬��Ϣ*/
typedef struct
{
    u8   cabinet_id[4];
    CELL cell[6];
} CABINET;



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


/**
 * @brief
 *
 * @param
 *
 * @note
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
//=============================================================================
//LED -> PA0
//=============================================================================
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//=============================================================================
//RS485 CONTROL -> PA0
//=============================================================================
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;	//CS-485
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);


    GPIO_SetBits(GPIOA, GPIO_Pin_0);//�ر�LEDָʾ��
    //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//��LEDָʾ��

}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
void USART1_Initialise(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX      PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���

}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
void USART2_Initialise(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;//
    NVIC_InitTypeDef NVIC_InitStructure;//
    USART_InitTypeDef USART_InitStructure;//

    /* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /* Enable USART2 */
    USART_Cmd(USART2, ENABLE);
}


/**
 * @brief
 *
 * @param
 *
 * @note
 */
void USART3_Initialise(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
    USART_DeInit(USART3);  //��λ����3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��Pb10
    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//����USART3�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
    /* ����һ���ֽ����ݵ�USART1 */
    USART_SendData(pUSARTx,ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
/*****************  ָ�����ȵķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
    unsigned int k=0;
    do
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}

M35RbufTypeDef M35REBUFStructure;
__IO float httpdata = 0;
__IO u8 HTTPPostData[40];


CABINET cabinet; //�������

__IO u8 cabinetbuff[46];
__IO u8 hex2char[93];
/**
 * @brief
 *
 * @param
 *
 * @note
 */
u8 HexToChar(u8 bChar)
{
    if((bChar>=0x00)&&(bChar<=0x09))
        bChar += 0x30;
    else if((bChar>=0x0a)&&(bChar<=0x0f))//��д��ĸ
        bChar += 0x57;
    else if((bChar>=0x0A)&&(bChar<=0x0F))//Сд��ĸ
        bChar += 0x37;
    else bChar = 0x24;
    return bChar;
}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
void hex_to_char(u8 *bhex,u8 *bchar)
{
    u8 mb,lb,i;
    for(i=0; i<46; i++)
    {
        mb=((bhex[i]&0xf0)>>4);
        lb=(bhex[i]&0x0f);

        mb=HexToChar(mb);
        lb=HexToChar(lb);

        hex2char[i*2]=mb;
        hex2char[i*2+1]=lb;
    }

}


/**
 * @brief
 *
 * @param
 *
 * @note
 */
void u32_to_u8(u32 data1,u8* data2)
{
    *(data2+3)=(u8)(data1);
    *(data2+2)=(u8)(data1>>8);
    *(data2+1)=(u8)(data1>>16);
    *(data2)  =(u8)(data1>>24);
}



/*
void cabinet_to_buff(void)
{

    u8 i;
    u32_to_u8(cabinet.cabinet_id,&cabinetbuff[0]);
    for(i=0; i<6; i++)
    {
        *(cabinetbuff+i*7+4)=cabinet.cell[i].cell_id;
        *(cabinetbuff+i*7+5)=cabinet.cell[i].cell_status;
        u32_to_u8(cabinet.cell[i].battery_id,&cabinetbuff[i*7+6]);
        *(cabinetbuff+i*7+10)=cabinet.cell[i].battery_quantity;
    }
    hex_to_char(cabinetbuff,hex2char);
    hex2char[92]=0x1a;//ASCII�� 0x1a����sub �������

}
*/
/**
 * @brief
 *
 * @param
 *
 * @note
 */
void cabinet_to_buff(void)
{
    u8 i;
    memcpy(&cabinetbuff[0],cabinet.cabinet_id,4);
    for(i=0; i<6; i++)
    {
        *(cabinetbuff+i*7+4)=cabinet.cell[i].cell_id;
        *(cabinetbuff+i*7+5)=cabinet.cell[i].cell_status;
        //u32_to_u8(cabinet.cell[i].battery_id,);
        memcpy(&cabinetbuff[i*7+6],cabinet.cell[i].battery_id,4);
        *(cabinetbuff+i*7+10)=cabinet.cell[i].battery_quantity;
    }
    hex_to_char(cabinetbuff,hex2char);
    hex2char[92]=0x1a;//ASCII�� 0x1a����sub �������

}



/**
 * @brief  Reset the mcu by software
 *
 * @param  none
 *
 * @note   use the 3.5 version of the firmware library.
 */
void SoftReset(void)
{
    __set_FAULTMASK(1); // �ر������ж�
    NVIC_SystemReset(); // ��λ
}





__IO u8 flagOnceTxd = 0;  //���η�����ɱ�־����������һ���ֽ�
__IO u8 cmdArrived = 0;   //������־�������յ���λ���·�������
__IO u8 cntRxd = 0;
__IO u8 bufRxd[40]; //���ڽ��ջ�����


/**
 * @brief
 *
 * @param
 *
 * @note
 */
u8 UartRead(u8 *buf, u8 len) //�������ݶ�ȡ���������ݽ���ָ��buf����ȡ���ݳ���len������ֵΪʵ�ʶ�ȡ�������ݳ���
{
    u8 i;

    if (len > cntRxd) //��ȡ���ȴ��ڽ��յ������ݳ���ʱ��
    {
        len = cntRxd; //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
    }
    for (i=0; i<len; i++) //�������յ�������
    {
        *buf = bufRxd[ i];
        buf++;
    }
    cntRxd = 0;  //������ռ�����

    return len;  //����ʵ�ʶ�ȡ����
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void UartWrite(u8 *buf, u8 len) //��������д�뺯���������ڷ��ͺ���������������ָ��buf�����ݳ���len
{
    GPIO_SetBits(GPIOA,GPIO_Pin_5);//RS485����Ϊ����
    while (len--)   //��������
    {

        USART_SendData(USART3,*buf);
        buf++;
        /* �ȴ�������� */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
    delay_ms(1);
    GPIO_ResetBits(GPIOA,GPIO_Pin_5);  //RS485����Ϊ����
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void UartDriver(void) //�������������������յ������ִ����Ӧ����
{
    u8 len;
    u8 buf[30];
    if (cmdArrived) //�������ʱ����ȡ����������
    {
        cmdArrived = 0;
        len = UartRead(buf, sizeof(buf)-2); //�����յ��������ȡ����������
        buf[len++] = '\r';   //�ڽ��յ�������֡�����ӻ������з��󷢻�
        buf[len++] = '\n';
        UartWrite(buf, len);

        //GPIO_WriteBit(GPIOC, GPIO_Pin_0, flag);
    }
}

u8 crc[2];

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void crc16(u8 *ptr, u16 len)
{
    u16 wcrc = 0xFFFF;
    u8 temp;
    u16 i = 0, j = 0;
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
    crc[0] = wcrc;
    crc[1] = wcrc >> 8;
}


typedef struct {
    u8 Address;
    u8 Lock_State;//1 on ; 0 off
    u8 Box_State;   //0��null 1��right 2:error
    u8 Battery_ID[4];
    u8 Battery_Level;//10=10%,20=20%
    u8 Battery_Temperature;
    u8 MCU_Temperature;
} slave;



slave cell_status[6];





u8 tbuf[8];
u8 rbuf[13];
/**
 * @brief
 *
 * @param
 *
 * @note
 */
void t_485_03(void)
{
    static u8 address=0;//01-06
    address++;
    if(address>6)
    {
        address=0x01;
    }
    tbuf[0]=address;
    tbuf[1]=0x03;//modbus 03 ������
    crc16(tbuf, 2);
    tbuf[2]=crc[0];
    tbuf[3]=crc[1];

    UartWrite(tbuf, 4);
}


/**
 * @brief
 *
 * @param
 *
 * @note
 */
u8 lock_comm[6];//��غп�����������
u8 con_locker=0;//������״̬
u8 lock_address;//�������·���Ӧ���ӿ�����ָ��
void t_485_06(void)
{
   // static u8 address=0;//01-06
    //address++;
    //if(address>6)
    //{
    //    address=0x01;
    //}
    tbuf[0]=lock_address;
    tbuf[1]=0x06;//modbus 06 д����

    tbuf[2]=0x3c;//����¶Ȳ��ܳ���60��
    //�����״̬Ϊ�ز��ҿ�������Ϊ�������Ϳ�����������͹�������
    if((cell_status[lock_address-1].Lock_State==0x00)&&(lock_comm[lock_address-1]==0x01))
		//if(lock_comm[lock_address]==0x01)
    {
        tbuf[3]=0x01;//���Ϳ�������
        //lock_comm[lock_address]=0x00;//�����������
    }
    else
    {
        tbuf[3]=0x00;//���Ϳ�����
        lock_comm[lock_address-1]=0x00;//�����������
    }
    tbuf[4]=0x03;//��������Ĭ��Ϊ3A
    crc16(tbuf, 5);
    tbuf[5]=crc[0];
    tbuf[6]=crc[1];

    UartWrite(tbuf, 7);
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
u8 address;

void r_485(void)
{
    u8 len,i;
    u8 index;
    if (cmdArrived)
    {

        cmdArrived = 0;

        len = UartRead(rbuf, 13);
        if((rbuf[0]>=0x01)&&(rbuf[0]<=0x06)&&(rbuf[1]==0x03))
        {

            crc16(rbuf, len-2);
            if ((crc[0] == rbuf[len-2]) && (crc[1] == rbuf[len-1]))
            {
								index=rbuf[0]-1;
                cell_status[index].Address=rbuf[0];
                cell_status[index].Lock_State=rbuf[2];
                cell_status[index].Box_State=rbuf[3];
                cell_status[index].Battery_ID[0]=rbuf[4];
                cell_status[index].Battery_ID[1]=rbuf[5];
                cell_status[index].Battery_ID[2]=rbuf[6];
                cell_status[index].Battery_ID[3]=rbuf[7];
                cell_status[index].Battery_Level=rbuf[8];
                cell_status[index].Battery_Temperature=rbuf[9];
                cell_status[index].MCU_Temperature=rbuf[10];



                cabinet.cell[index].cell_id=cell_status[index].Address;
                cabinet.cell[index].cell_status=cell_status[index].Box_State;
                cabinet.cell[index].battery_id[0]=cell_status[index].Battery_ID[0];
                cabinet.cell[index].battery_id[1]=cell_status[index].Battery_ID[1];
                cabinet.cell[index].battery_id[2]=cell_status[index].Battery_ID[2];
                cabinet.cell[index].battery_id[3]=cell_status[index].Battery_ID[3];
                cabinet.cell[index].battery_quantity=cell_status[index].Battery_Level;



            }
        }



    }
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void iwdg_init()	//�������Ź���ʼ������
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܼĴ�����д����
    IWDG_SetPrescaler(IWDG_Prescaler_64);//����IWDGԤ��Ƶֵ//40K/64=1.6ms
    IWDG_SetReload(800);//����IWDG��װ��ֵ  12λ�ģ�0~4095��//800*1.6ms=1.28s
    IWDG_ReloadCounter();//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
    //IWDG_Enable();//ʹ��IWDG
}

u8 ffffff;

int main(void)
{
    u16 i=0;
    u8 se;

    GPIO_Configuration();//LED PA0ָʾ

    delay_init(72);

    TIM2_init();//��ʱ��2��ʼ��

    USART1_Initialise(115200);//printf ����
    USART2_Initialise(115200);//GPRS M35
    USART3_Initialise(9600);  //RS485

    //GPIO_SetBits(GPIOA, GPIO_Pin_0);//�ر�LEDָʾ��
    //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//��LEDָʾ��
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);//ʹ��485����
    //GPIO_SetBits(GPIOA, GPIO_Pin_5);//ʹ��485����

    iwdg_init(); //�������Ź���ʼ������

    //�Ѹ��ӵ����Ϣ������buff��������
    //cabinet_to_buff();
    cabinet.cabinet_id[0]=0x3f;
    cabinet.cabinet_id[1]=0x0b;
    cabinet.cabinet_id[2]=0x5f;
    cabinet.cabinet_id[3]=0xb1;
    while(1)
    {		
        if (M35_HTTPConnect() == ERROR)continue;	//�����������HTTP����
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//��LEDָʾ��
        printf("GPRS�Ѿ����ӵ�������\r\n");//��ӡ����һ��������

        ClearRAM((u8*)HTTPPostData,40);//���HTTPPostData���ݰ�����
        SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",2000,1);//����ǰ̨GPRS����
        PrintfClear();//��ӡ����ջ�����
        delay_ms(100);
        SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",2000,1); //�ж���Ӫ��
        PrintfClear();//��ӡ����ջ�����
        delay_ms(100);
        if (SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1) == 1)//��ȡ��վ��λ��Ϣ
        {
            //strcpy((char*)HTTPPostData,"DIDADA&LBSGPS=");//DIDADA&LBSGPS=��Ϊ������
            strcpy((char*)HTTPPostData,"3f0b5fb1=");//DIDADA&LBSGPS=��Ϊ������
            //strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&cabinet.cabinet_id[4]),4);
            //strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"=");
            //����γ�ȸ���HTTPPostData���ݰ���
            strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&M35REBUFStructure.M35Rebuf[13]),20);
            //��������־0x1A����HTTPPostData���ݰ���
            strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");
            //��M35���ͷ������ݰ�ָ����ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
            if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)break;
            //��������������ݰ�HTTPPostData,���ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
            if (SIM_SendCmd((char*)HTTPPostData,"\r\nSEND OK","NULL","NULL",2000,1) != 1)break;
            ClearRAM((u8*)HTTPPostData,40);//���HTTPPostData���ݰ�����
            PrintfClear();//��ӡ����ջ�����
        }
				
        IWDG_Enable();//ʹ��IWDG
				
        while(1)
        {
            if(flag5ms>=4)
            {
                flag5ms=0;


                switch(timebase)
                {
                case timebase20ms:
                    flag30s++;
                    if(flag30s>=30)
                    {
                        flag30s=0;
											
												USART_Cmd(USART3, DISABLE);                    //���ܴ���
												USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�ر�USART3�����ж�
                        if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)
                            SoftReset();//����ʧ�ܣ�����λ
                        //��������������ݰ�hex2char,���ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
                        if (SIM_SendCmd((char*)hex2char,"\r\nSEND OK","NULL","NULL",2000,1) != 1)
                            SoftReset();//����ʧ�ܣ�����λ
												USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
												USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//����USART3�����ж�
											
                    }
										
										t_485_03();
                    timebase=timebase40ms;
                    break;
                case timebase40ms:

                    if (M35REBUFStructure.flay == SET)
                    {
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:01"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0101"))
                        {
														con_locker=1;
														lock_address=0x01;
                            lock_comm[0]=0x01;//1�ź��ӿ���
                        }
									
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:02"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0201"))
                        {
														con_locker=1;
														lock_address=0x02;
                            lock_comm[1]=0x01;//2�ź��ӿ���
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:03"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0301"))
                        {
														con_locker=1;
														lock_address=0x03;
                            lock_comm[2]=0x01;//3�ź��ӿ���
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:04"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0401"))
                        {
														con_locker=1;
														lock_address=0x04;
                            lock_comm[3]=0x01;//4�ź��ӿ���
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:05"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0501"))
                        {
														con_locker=1;
														lock_address=0x05;
                            lock_comm[4]=0x01;//5�ź��ӿ���
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:0601"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0601"))
                        {
														con_locker=1;
														lock_address=0x06;
                            lock_comm[5]=0x01;//6�ź��ӿ���
                        }
												if(con_locker)
												{
													con_locker=0;
													ffffff^=1;
													if(ffffff)
													{
														GPIO_ResetBits(GPIOA, GPIO_Pin_0);//��LEDָʾ��
													}
													else
													{
														GPIO_SetBits(GPIOA, GPIO_Pin_0);//��LEDָʾ��
													}
													t_485_06();
												}
												 
                        PrintfClear();//��ӡ����ջ�����
                        M35REBUFStructure.flay = RESET;
                    }

                    timebase=timebase60ms;
                    break;
                case timebase60ms:

                    



                    timebase=timebase80ms;
                    break;
                case timebase80ms:

                    r_485();            //�ӽڵ�������ݲ��ϴ�buff

                    cabinet_to_buff();  //�Ѹ��ӵ����Ϣ������buff��������

                    timebase=timebase100ms;
                    break;
                case timebase100ms:
                    
										flag10s++;
                    if(flag10s>40)
                    {
                        flag10s=0;
                        t_485_06();
                    }
										

                    IWDG_ReloadCounter();	   //ι����ʱ����1.28s

                    timebase=timebase20ms;
                    break;
                default:
                    timebase=timebase20ms;
                    break;
                }

            }
        }
    }

}








/*
    cabinet.cabinet_num=0x01020304;

    cabinet.cell[0].cell_num=0x05;
    cabinet.cell[0].cell_status=0x06;
    cabinet.cell[0].battery_num=0x0708090a;
    cabinet.cell[0].battery_quantity=0x0b;

    cabinet.cell[1].cell_num=0x0c;
    cabinet.cell[1].cell_status=0x0d;
    cabinet.cell[1].battery_num=0x0e0f1011;
    cabinet.cell[1].battery_quantity=0x12;

    cabinet.cell[2].cell_num=0x13;
    cabinet.cell[2].cell_status=0x14;
    cabinet.cell[2].battery_num=0x15161718;
    cabinet.cell[2].battery_quantity=0x19;

    cabinet.cell[3].cell_num=0x1a;
    cabinet.cell[3].cell_status=0x1b;
    cabinet.cell[3].battery_num=0x1c1d1e1f;
    cabinet.cell[3].battery_quantity=0x20;

    cabinet.cell[4].cell_num=0x21;
    cabinet.cell[4].cell_status=0x22;
    cabinet.cell[4].battery_num=0x23242526;
    cabinet.cell[4].battery_quantity=0x27;

    cabinet.cell[5].cell_num=0x28;
    cabinet.cell[5].cell_status=0x29;
    cabinet.cell[5].battery_num=0x2a2b2c2d;
    cabinet.cell[5].battery_quantity=0x2e;
*/
/*
u32_to_u8(cabinet.cabinet_num,&cabinetbuff[0]);
for(i=0; i<6; i++)
{
*(cabinetbuff+i*7+4)=cabinet.cell[i].cell_num;
*(cabinetbuff+i*7+5)=cabinet.cell[i].cell_status;
u32_to_u8(cabinet.cell[i].battery_num,&cabinetbuff[i*7+6]);
*(cabinetbuff+i*7+10)=cabinet.cell[i].battery_quantity;
}
hex_to_char(cabinetbuff,hex2char);
hex2char[92]=0x1a;//ASCII�� 0x1a����sub �������
*/

//*(cabinetbuff+46)=0x1a;//ASCII�� 0x1a����sub �������
//GPIO_ResetBits ( GPIOA, GPIO_Pin_0 );//����ָʾ��


/*
while(1)
{
if (M35_HTTPConnect() == ERROR)continue;	//�����������HTTP����


printf("GPRS�Ѿ����ӵ�������\r\n");//��ӡ����һ��������

//printf((char*)(&cabinetbuff));

//GPIO_ResetBits ( GPIOC, GPIO_Pin_13 );//����ָʾ��

ClearRAM((u8*)HTTPPostData,40);//���HTTPPostData���ݰ�����
SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",2000,1);//����ǰ̨GPRS����
PrintfClear();//��ӡ����ջ�����
delay_ms(100);
SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",2000,1); //�ж���Ӫ��
PrintfClear();//��ӡ����ջ�����
delay_ms(100);
if (SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1) == 1)//��ȡ��վ��λ��Ϣ
{
    strcpy((char*)HTTPPostData,"DIDADA&LBSGPS=");
    //����γ�ȸ���HTTPPostData���ݰ���
    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&M35REBUFStructure.M35Rebuf[13]),20);
    //��������־0x1A����HTTPPostData���ݰ���
    strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");
    //��M35���ͷ������ݰ�ָ����ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
    if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)break;
    //��������������ݰ�HTTPPostData,���ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
    if (SIM_SendCmd((char*)HTTPPostData,"\r\nSEND OK","NULL","NULL",2000,1) != 1)break;
    ClearRAM((u8*)HTTPPostData,40);//���HTTPPostData���ݰ�����
    PrintfClear();//��ӡ����ջ�����
}

while(1)
{
				//��������
    if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)
        break;
    //��������������ݰ�hex2char,���ж��Ƿ�ɹ���ʧ�����˳�whileѭ��
    if (SIM_SendCmd((u8*)hex2char,"\r\nSEND OK","NULL","NULL",2000,1) != 1)
        break;

    delay_s(5);
				//��������
    if (M35REBUFStructure.flay == SET)
    {
        if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:06"))
        {
            GPIO_ResetBits ( GPIOC, GPIO_Pin_13 );//����ָʾ��
            //return 1;
        }
        PrintfClear();//��ӡ����ջ�����
        M35REBUFStructure.flay = RESET;
    }

    cabinet_to_buff();


    //PrintfClear();//��ӡ����ջ�����
}
delay_ms(2000);
SIM_SendCmd("AT+QIDEACT\r\n","\r\nDEACT OK","NULL","NULL",2000,1);
PrintfClear();//��ӡ����ջ�����
}
*/

/*
kkk^=1;
if(kkk)
{
	GPIO_ResetBits ( GPIOA, GPIO_Pin_0 );//����ָʾ��
}
else
{
	GPIO_SetBits( GPIOA, GPIO_Pin_0 );
}
*/



















