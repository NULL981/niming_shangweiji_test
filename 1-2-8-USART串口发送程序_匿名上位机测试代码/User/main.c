/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	USART串口发送程序
编写人：	杜洋	
编写时间：	2018年1月9日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"

#include "usart.h"
#include <math.h>



//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++){
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		USART_SendData(USART1,send_buf[i]);
		
	}		//发送数据到串口1 
}
int i = 0;
u8 data1 = 0x00;
u8 data2[4];
int main (void){//主程序
	//u8 a=7,b=8;
	u8 fun = 0xA1;
	u8 data[] = {2,100,2,100,2,100,2,100,2,100,2,100};
	unsigned int a = 0;
	 //
	
	//初始化程序
	RCC_Configuration(); //时钟设置

	USART1_Init(115200); //串口初始化（参数是波特率）

	//主循环
	while(1){

		/* 发送方法1 */
		//USART_SendData(USART1 , 0x55); //发送单个数值
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); //检查发送中断标志位

		/* 发送方法2 */
//		printf("STM32F103 "); //纯字符串发送数据到串口

//		printf("STM32 %d %d ",a,b); //纯字符串和变量发送数据到串口,a符号变量
		 
		/* 发送方法3 */
//		USART1_printf("STM32 %d %d ",a,b);
		
		
		/*  */
		data2[0] = a & 0xFF000000 >> 24;
		data2[1] = a & 0x00FF0000 >> 16;
		data2[2] = a & 0x0000FF00 >> 8;
		data2[3] = a & 0x000000FF;
		//usart1_niming_report(fun,data,12);
		usart1_niming_report(fun,data2,4);//data1
		a++;
		if(a == 1000){
			a = 0;
		}
		
		//data[1]++;
        delay_ms(5); //延时
	}
}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

*/



