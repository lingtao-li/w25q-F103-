#include "W25Q.h"


uint16_t W25QXX_TYPE;	//Ĭ����W25Q16


extern SPI_HandleTypeDef hspi1;
void light(void)
	{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_Delay(1000);
}

uint8_t W25QXX_Init(void)
{ 	
	W25Q_CS_1;			                //SPI FLASH��ѡ��	
	W25QXX_TYPE=W25QXX_ReadID();	        //��ȡFLASH ID.
	if(W25QXX_TYPE==W25Q64 )                //SPI FLASHΪW25Q16
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t cmd[2];
	uint8_t temp[2];
	switch(regno)
	{
		case 1:
				cmd[0] = W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
				break;
		case 2:
				cmd[0] = W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
				break;
		case 3:
				cmd[0] = W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
				break;
		default:
				cmd[0] = W25X_ReadStatusReg1;    
				break;
	} 			
	W25Q_CS_0;  

	HAL_SPI_TransmitReceive(&hspi1, cmd, temp, 2, 10);//���Ͷ�״̬�Ĵ�������  
	W25Q_CS_1;                            //ȡ��Ƭѡ     
	return temp[1];   
}
//дW25QXX״̬�Ĵ���
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{ 
	uint8_t cmd[2];
	switch(regno)
	{
		case 1:
				cmd[0] = W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
				break;
		case 2:
				cmd[0] = W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
				break;
		case 3:
				cmd[0] = W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
				break;
		default:
				cmd[0] = W25X_WriteStatusReg1;    
				break;
	}  
	cmd[1] = sr;
	W25Q_CS_0;                               //ʹ������   
	//Poll mode
	//����ָ��ͽ��ռ������Ƽ�ʹ�ò�ѯģʽ
	HAL_SPI_Transmit(&hspi1, cmd, 2, 10);//���Ͷ�״̬�Ĵ�������  
	W25Q_CS_1;                            //ȡ��Ƭѡ     	      
}  

uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	
	uint8_t cmd[6],rst[6];
	cmd[0] = W25X_ManufactDeviceID;
	
	W25Q_CS_0; 	

	HAL_SPI_TransmitReceive(&hspi1, cmd, rst, 6, 10);//���Ͷ�ȡID����		
	
	Temp=rst[4]<<8 | rst[5];   	

	W25Q_CS_1;		
	return Temp;
}   
void W25QXX_Write_Enable(void)   
{
	uint8_t cmd = W25X_WriteEnable;
	W25Q_CS_0;                             //ʹ������   
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);   //����дʹ��  
	W25Q_CS_1;                           //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	uint8_t cmd = W25X_WriteDisable;
	W25Q_CS_0;                             //ʹ������   
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);  //����д��ָֹ��    
	W25Q_CS_1;     //ȡ��Ƭѡ       	      
} 
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}  

void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i; 
	uint8_t cmd[5];
	cmd[0] = W25X_ReadData;
	cmd[1] = (uint8_t)((ReadAddr)>>24);
	cmd[2] = (uint8_t)((ReadAddr)>>16);
	cmd[3] = (uint8_t)((ReadAddr)>>8);
	cmd[4] = (uint8_t)(ReadAddr);
	
	W25Q_CS_0;                            //ʹ������   
	HAL_SPI_Transmit(&hspi1, cmd, 1, 10);  //���Ͷ�ȡ����   
	HAL_SPI_Transmit(&hspi1, &cmd[2],3, 30);  
	HAL_SPI_Receive(&hspi1,pBuffer, NumByteToRead,10*NumByteToRead);
	W25Q_CS_1;  
	
}  
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint32_t i; 
	uint8_t cmd[5];
	cmd[0] = W25X_PageProgram;
	cmd[1] = (uint8_t)((WriteAddr)>>24);
	cmd[2] = (uint8_t)((WriteAddr)>>16);
	cmd[3] = (uint8_t)((WriteAddr)>>8);
	cmd[4] = (uint8_t)(WriteAddr);
	
  W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_Wait_Busy();
	W25Q_CS_0;                            //ʹ������   
  HAL_SPI_Transmit(&hspi1, cmd, 1, 10);   //����дҳ����   


		HAL_SPI_Transmit(&hspi1, &cmd[2], 3, 1000);  //����24bit��ַ 

		HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, 4000);  //����24bit��ַ 
	 
	W25Q_CS_1;                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
} 

void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	uint8_t cmd[5];
	  
	cmd[0] = W25X_SectorErase;
	cmd[1] = (uint8_t)((Dst_Addr*NumByteOfSector)>>24);
	cmd[2] = (uint8_t)((Dst_Addr*NumByteOfSector)>>16);
	cmd[3] = (uint8_t)((Dst_Addr*NumByteOfSector)>>8);
	cmd[4] = (uint8_t)(Dst_Addr*NumByteOfSector);

  W25QXX_Write_Enable();                  //SET WEL 	 
  W25QXX_Wait_Busy();   
  W25Q_CS_0;                            //ʹ������   
  HAL_SPI_Transmit(&hspi1, cmd, 1, 10);    //������������ָ�� 
	
		HAL_SPI_Transmit(&hspi1, &cmd[2], 3, 4000);  //����24bit��ַ  

	W25Q_CS_1;                            //ȡ��Ƭѡ     	      
  W25QXX_Wait_Busy();   				    //�ȴ��������
}  
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;
	uint16_t NumByteToWriteNow;
	pageremain = NumByteOfPage - WriteAddr % NumByteOfPage; //��ҳʣ����ֽ���		 
	NumByteToWriteNow = NumByteToWrite;
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWriteNow;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWriteNow==pageremain)
			break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWriteNow-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWriteNow>NumByteOfPage)
				pageremain=NumByteOfPage; //һ�ο���д��256���ֽ�
			else 
				pageremain=NumByteToWriteNow; 	  //����256���ֽ���
		}
	}    
} 

