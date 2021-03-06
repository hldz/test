


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

/*格子状态信息*/
typedef struct
{
    u8   cell_id;
    u8   cell_status;
    u8   battery_id[4];
    u8   battery_quantity;
} CELL;

/*机柜状态信息*/
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
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);


    GPIO_SetBits(GPIOA, GPIO_Pin_0);//关闭LED指示灯
    //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//打开LED指示灯

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
    //GPIO端口设置
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

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//一般设置为115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断

    USART_Cmd(USART1, ENABLE);                    //使能串口

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
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3，GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
    USART_DeInit(USART3);  //复位串口3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化Pb10
    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启USART3接收中断
    USART_Cmd(USART3, ENABLE);                    //使能串口
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(pUSARTx,ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/**
 * @brief
 *
 * @param
 *
 * @note
 */
/*****************  指定长度的发送字符串 **********************/
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
/*****************  发送字符串 **********************/
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


CABINET cabinet; //机柜变量

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
    else if((bChar>=0x0a)&&(bChar<=0x0f))//大写字母
        bChar += 0x57;
    else if((bChar>=0x0A)&&(bChar<=0x0F))//小写字母
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
    hex2char[92]=0x1a;//ASCII码 0x1a代表sub 必须加上

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
    hex2char[92]=0x1a;//ASCII码 0x1a代表sub 必须加上

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
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}





__IO u8 flagOnceTxd = 0;  //单次发送完成标志，即发送完一个字节
__IO u8 cmdArrived = 0;   //命令到达标志，即接收到上位机下发的命令
__IO u8 cntRxd = 0;
__IO u8 bufRxd[40]; //串口接收缓冲区


/**
 * @brief
 *
 * @param
 *
 * @note
 */
u8 UartRead(u8 *buf, u8 len) //串口数据读取函数，数据接收指针buf，读取数据长度len，返回值为实际读取到的数据长度
{
    u8 i;

    if (len > cntRxd) //读取长度大于接收到的数据长度时，
    {
        len = cntRxd; //读取长度设置为实际接收到的数据长度
    }
    for (i=0; i<len; i++) //拷贝接收到的数据
    {
        *buf = bufRxd[ i];
        buf++;
    }
    cntRxd = 0;  //清零接收计数器

    return len;  //返回实际读取长度
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void UartWrite(u8 *buf, u8 len) //串口数据写入函数，即串口发送函数，待发送数据指针buf，数据长度len
{
    GPIO_SetBits(GPIOA,GPIO_Pin_5);//RS485设置为发送
    while (len--)   //发送数据
    {

        USART_SendData(USART3,*buf);
        buf++;
        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
    delay_ms(1);
    GPIO_ResetBits(GPIOA,GPIO_Pin_5);  //RS485设置为接收
}

/**
 * @brief
 *
 * @param
 *
 * @note
 */
void UartDriver(void) //串口驱动函数，检测接收到的命令并执行相应动作
{
    u8 len;
    u8 buf[30];
    if (cmdArrived) //有命令到达时，读取处理该命令
    {
        cmdArrived = 0;
        len = UartRead(buf, sizeof(buf)-2); //将接收到的命令读取到缓冲区中
        buf[len++] = '\r';   //在接收到的数据帧后添加换车换行符后发回
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
    u8 Box_State;   //0：null 1：right 2:error
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
    tbuf[1]=0x03;//modbus 03 读命令
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
u8 lock_comm[6];//电池盒开锁命令数组
u8 con_locker=0;//控制锁状态
u8 lock_address;//服务器下发对应盒子控制锁指令
void t_485_06(void)
{
   // static u8 address=0;//01-06
    //address++;
    //if(address>6)
    //{
    //    address=0x01;
    //}
    tbuf[0]=lock_address;
    tbuf[1]=0x06;//modbus 06 写命令

    tbuf[2]=0x3c;//电池温度不能超过60度
    //如果锁状态为关并且开锁命令为开，则发送开锁命令；否则发送关锁命令
    if((cell_status[lock_address-1].Lock_State==0x00)&&(lock_comm[lock_address-1]==0x01))
		//if(lock_comm[lock_address]==0x01)
    {
        tbuf[3]=0x01;//发送开锁命令
        //lock_comm[lock_address]=0x00;//清除开锁命令
    }
    else
    {
        tbuf[3]=0x00;//发送空命令
        lock_comm[lock_address-1]=0x00;//清除开锁命令
    }
    tbuf[4]=0x03;//最大充电电流默认为3A
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
void iwdg_init()	//独立看门狗初始化配置
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能寄存器，写功能
    IWDG_SetPrescaler(IWDG_Prescaler_64);//设置IWDG预分频值//40K/64=1.6ms
    IWDG_SetReload(800);//设置IWDG重装载值  12位的（0~4095）//800*1.6ms=1.28s
    IWDG_ReloadCounter();//按照IWDG重装载寄存器的值重装载IWDG计数器
    //IWDG_Enable();//使能IWDG
}

u8 ffffff;

int main(void)
{
    u16 i=0;
    u8 se;

    GPIO_Configuration();//LED PA0指示

    delay_init(72);

    TIM2_init();//定时器2初始化

    USART1_Initialise(115200);//printf 测试
    USART2_Initialise(115200);//GPRS M35
    USART3_Initialise(9600);  //RS485

    //GPIO_SetBits(GPIOA, GPIO_Pin_0);//关闭LED指示灯
    //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//打开LED指示灯
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);//使能485接收
    //GPIO_SetBits(GPIOA, GPIO_Pin_5);//使能485发送

    iwdg_init(); //独立看门狗初始化配置

    //把格子电池信息往发送buff数组中填
    //cabinet_to_buff();
    cabinet.cabinet_id[0]=0x3f;
    cabinet.cabinet_id[1]=0x0b;
    cabinet.cabinet_id[2]=0x5f;
    cabinet.cabinet_id[3]=0xb1;
    while(1)
    {		
        if (M35_HTTPConnect() == ERROR)continue;	//与服务器建立HTTP链接
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0);//打开LED指示灯
        printf("GPRS已经连接到服务器\r\n");//打印串口一测试数据

        ClearRAM((u8*)HTTPPostData,40);//清空HTTPPostData数据包缓存
        SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",2000,1);//设置前台GPRS场景
        PrintfClear();//打印和清空缓冲区
        delay_ms(100);
        SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",2000,1); //判断运营商
        PrintfClear();//打印和清空缓冲区
        delay_ms(100);
        if (SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1) == 1)//获取基站定位信息
        {
            //strcpy((char*)HTTPPostData,"DIDADA&LBSGPS=");//DIDADA&LBSGPS=改为机柜编号
            strcpy((char*)HTTPPostData,"3f0b5fb1=");//DIDADA&LBSGPS=改为机柜编号
            //strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&cabinet.cabinet_id[4]),4);
            //strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"=");
            //将经纬度复制HTTPPostData数据包中
            strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&M35REBUFStructure.M35Rebuf[13]),20);
            //将结束标志0x1A复制HTTPPostData数据包中
            strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");
            //向M35发送发送数据包指令，并判断是否成功，失败则退出while循环
            if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)break;
            //向服务器发送数据包HTTPPostData,并判断是否成功，失败则退出while循环
            if (SIM_SendCmd((char*)HTTPPostData,"\r\nSEND OK","NULL","NULL",2000,1) != 1)break;
            ClearRAM((u8*)HTTPPostData,40);//清空HTTPPostData数据包缓存
            PrintfClear();//打印和清空缓冲区
        }
				
        IWDG_Enable();//使能IWDG
				
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
											
												USART_Cmd(USART3, DISABLE);                    //禁能串口
												USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭USART3接收中断
                        if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)
                            SoftReset();//发送失败，软复位
                        //向服务器发送数据包hex2char,并判断是否成功，失败则退出while循环
                        if (SIM_SendCmd((char*)hex2char,"\r\nSEND OK","NULL","NULL",2000,1) != 1)
                            SoftReset();//发送失败，软复位
												USART_Cmd(USART3, ENABLE);                    //使能串口
												USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启USART3接收中断
											
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
                            lock_comm[0]=0x01;//1号盒子开锁
                        }
									
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:02"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0201"))
                        {
														con_locker=1;
														lock_address=0x02;
                            lock_comm[1]=0x01;//2号盒子开锁
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:03"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0301"))
                        {
														con_locker=1;
														lock_address=0x03;
                            lock_comm[2]=0x01;//3号盒子开锁
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:04"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0401"))
                        {
														con_locker=1;
														lock_address=0x04;
                            lock_comm[3]=0x01;//4号盒子开锁
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:05"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0501"))
                        {
														con_locker=1;
														lock_address=0x05;
                            lock_comm[4]=0x01;//5号盒子开锁
                        }
												
                        //if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:0601"))
												if (strstr((char*)(M35REBUFStructure.M35Rebuf),"0601"))
                        {
														con_locker=1;
														lock_address=0x06;
                            lock_comm[5]=0x01;//6号盒子开锁
                        }
												if(con_locker)
												{
													con_locker=0;
													ffffff^=1;
													if(ffffff)
													{
														GPIO_ResetBits(GPIOA, GPIO_Pin_0);//打开LED指示灯
													}
													else
													{
														GPIO_SetBits(GPIOA, GPIO_Pin_0);//打开LED指示灯
													}
													t_485_06();
												}
												 
                        PrintfClear();//打印和清空缓冲区
                        M35REBUFStructure.flay = RESET;
                    }

                    timebase=timebase60ms;
                    break;
                case timebase60ms:

                    



                    timebase=timebase80ms;
                    break;
                case timebase80ms:

                    r_485();            //子节点接收数据并上传buff

                    cabinet_to_buff();  //把格子电池信息往发送buff数组中填

                    timebase=timebase100ms;
                    break;
                case timebase100ms:
                    
										flag10s++;
                    if(flag10s>40)
                    {
                        flag10s=0;
                        t_485_06();
                    }
										

                    IWDG_ReloadCounter();	   //喂狗的时间是1.28s

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
hex2char[92]=0x1a;//ASCII码 0x1a代表sub 必须加上
*/

//*(cabinetbuff+46)=0x1a;//ASCII码 0x1a代表sub 必须加上
//GPIO_ResetBits ( GPIOA, GPIO_Pin_0 );//点亮指示灯


/*
while(1)
{
if (M35_HTTPConnect() == ERROR)continue;	//与服务器建立HTTP链接


printf("GPRS已经连接到服务器\r\n");//打印串口一测试数据

//printf((char*)(&cabinetbuff));

//GPIO_ResetBits ( GPIOC, GPIO_Pin_13 );//点亮指示灯

ClearRAM((u8*)HTTPPostData,40);//清空HTTPPostData数据包缓存
SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",2000,1);//设置前台GPRS场景
PrintfClear();//打印和清空缓冲区
delay_ms(100);
SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",2000,1); //判断运营商
PrintfClear();//打印和清空缓冲区
delay_ms(100);
if (SIM_SendCmd("AT+QCELLLOC=1\r\n","\r\n+QCELLLOC:","NULL","NULL",5000,1) == 1)//获取基站定位信息
{
    strcpy((char*)HTTPPostData,"DIDADA&LBSGPS=");
    //将经纬度复制HTTPPostData数据包中
    strncpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),(char*)(&M35REBUFStructure.M35Rebuf[13]),20);
    //将结束标志0x1A复制HTTPPostData数据包中
    strcpy((char*)(HTTPPostData+strlen((char*)HTTPPostData)),"\x1a");
    //向M35发送发送数据包指令，并判断是否成功，失败则退出while循环
    if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)break;
    //向服务器发送数据包HTTPPostData,并判断是否成功，失败则退出while循环
    if (SIM_SendCmd((char*)HTTPPostData,"\r\nSEND OK","NULL","NULL",2000,1) != 1)break;
    ClearRAM((u8*)HTTPPostData,40);//清空HTTPPostData数据包缓存
    PrintfClear();//打印和清空缓冲区
}

while(1)
{
				//发送数据
    if (SIM_SendCmd("AT+QISEND\r\n","\r\n>","NULL","NULL",2000,1) != 1)
        break;
    //向服务器发送数据包hex2char,并判断是否成功，失败则退出while循环
    if (SIM_SendCmd((u8*)hex2char,"\r\nSEND OK","NULL","NULL",2000,1) != 1)
        break;

    delay_s(5);
				//接收数据
    if (M35REBUFStructure.flay == SET)
    {
        if (strstr((char*)(M35REBUFStructure.M35Rebuf),"IPD2:06"))
        {
            GPIO_ResetBits ( GPIOC, GPIO_Pin_13 );//点亮指示灯
            //return 1;
        }
        PrintfClear();//打印和清空缓冲区
        M35REBUFStructure.flay = RESET;
    }

    cabinet_to_buff();


    //PrintfClear();//打印和清空缓冲区
}
delay_ms(2000);
SIM_SendCmd("AT+QIDEACT\r\n","\r\nDEACT OK","NULL","NULL",2000,1);
PrintfClear();//打印和清空缓冲区
}
*/

/*
kkk^=1;
if(kkk)
{
	GPIO_ResetBits ( GPIOA, GPIO_Pin_0 );//点亮指示灯
}
else
{
	GPIO_SetBits( GPIOA, GPIO_Pin_0 );
}
*/




















