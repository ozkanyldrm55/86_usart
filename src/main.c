#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

char rx_buff[50];
int i = 0;

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruc;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2 Tx Pini olarak ayarladýk.
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA2 Rx Pini olarak ayarladýk.

	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // rx ve tx icin kullanilacak bacaklar
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruc);
}

void USART_Config()
{
	USART_InitTypeDef USART_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	USART_InitStruct.USART_BaudRate = 9600;		// kac saniye de bir veri gonderilecegini ayarladik.
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //donanimsal kontrol olmasin
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //mesaj alýp gonderme islemi yapacagiz.
	USART_InitStruct.USART_Parity = USART_Parity_No; //oncelik olmasin
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1 bilgisi geldiginde haberlesme duracak.
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // gonderilecek veri uzunlugu 8 bit

	USART_Init(USART2,&USART_InitStruct);
	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2 , USART_IT_RXNE ,ENABLE);//bir mesaj geldiginde interrupt aktif olur.//USART_ITConfig usart ýn interrupt fonsitonudur
}

void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct; //interrupt yöneticisi

	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//interrupt kanalý olarak USART2 yi kullanacagimizi belirttik
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //cevresel birimi aktif ediyoruz
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//oncelik yok tek interrupt var cunku //PreemptionPriority degeri dusuk olan interrupt daha oncelikli oluyor
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
}

void USART_Puts(USART_TypeDef* USARTx , volatile char *s)
{
	while(*s)
	{
		// wait until data register is empty
		while(!(USARTx -> SR & 0x00000040));

		USART_SendData(USARTx , *s);
		*s++;
	}
}

void USART2_IRQHandler()//yani bir data geldiginde ve interrup olustugunda bu fonsiyona dallanacak
{
	if(USART_GetITStatus(USART2 , USART_IT_RXNE)) // mesaj geldiginde interrupt bayragý kalkacak , kontrol ediliyor
	{
		char rx_buffer = USART_ReceiveData(USART2);//gelen datayý usart2 den al

		if(rx_buffer !='\n' && i<50) // yani enter e basilmamissa , rx_buffer icinde veri varsa demek ,&& i<50 i 50 den kucukse buraya girme sartý koyduk
		{
			rx_buff[i] = rx_buffer;
			i++;
		}
		else
		{
			//rx_buff[i] = '\0'; // son gelen parametreyi 0 la anlaminda , enter geldi yani
			rx_buff[i] = rx_buffer;
			i=0;
			USART_Puts(USART2, rx_buff);// bize gelen mesaji geri gonderdik.

			for(int a = 0; a<50; a++)
				rx_buff[a] = '\0'; //mesajý gonderdikten sonra tum verileri sýfýrla
		}
	}
}

int main(void)
{
	GPIO_Config();
	NVIC_Config();
	USART_Config();
  while (1)
  {

  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
