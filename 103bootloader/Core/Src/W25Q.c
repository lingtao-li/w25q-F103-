#include "W25Q.h"


uint16_t W25QXX_TYPE;	//默认是W25Q16


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
	W25Q_CS_1;			                //SPI FLASH不选中	
	W25QXX_TYPE=W25QXX_ReadID();	        //读取FLASH ID.
	if(W25QXX_TYPE==W25Q64 )                //SPI FLASH为W25Q16
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
				cmd[0] = W25X_ReadStatusReg1;    //读状态寄存器1指令
				break;
		case 2:
				cmd[0] = W25X_ReadStatusReg2;    //读状态寄存器2指令
				break;
		case 3:
				cmd[0] = W25X_ReadStatusReg3;    //读状态寄存器3指令
				break;
		default:
				cmd[0] = W25X_ReadStatusReg1;    
				break;
	} 			
	W25Q_CS_0;  

	HAL_SPI_TransmitReceive(&hspi1, cmd, temp, 2, 10);//发送读状态寄存器命令  
	W25Q_CS_1;                            //取消片选     
	return temp[1];   
}
//写W25QXX状态寄存器
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{ 
	uint8_t cmd[2];
	switch(regno)
	{
		case 1:
				cmd[0] = W25X_WriteStatusReg1;    //写状态寄存器1指令
				break;
		case 2:
				cmd[0] = W25X_WriteStatusReg2;    //写状态寄存器2指令
				break;
		case 3:
				cmd[0] = W25X_WriteStatusReg3;    //写状态寄存器3指令
				break;
		default:
				cmd[0] = W25X_WriteStatusReg1;    
				break;
	}  
	cmd[1] = sr;
	W25Q_CS_0;                               //使能器件   
	//Poll mode
	//发送指令和接收简单数据推荐使用查询模式
	HAL_SPI_Transmit(&hspi1, cmd, 2, 10);//发送读状态寄存器命令  
	W25Q_CS_1;                            //取消片选     	      
}  

uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	
	uint8_t cmd[6],rst[6];
	cmd[0] = W25X_ManufactDeviceID;
	
	W25Q_CS_0; 	

	HAL_SPI_TransmitReceive(&hspi1, cmd, rst, 6, 10);//发送读取ID命令		
	
	Temp=rst[4]<<8 | rst[5];   	

	W25Q_CS_1;		
	return Temp;
}   
void W25QXX_Write_Enable(void)   
{
	uint8_t cmd = W25X_WriteEnable;
	W25Q_CS_0;                             //使能器件   
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);   //发送写使能  
	W25Q_CS_1;                           //取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)   
{  
	uint8_t cmd = W25X_WriteDisable;
	W25Q_CS_0;                             //使能器件   
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);  //发送写禁止指令    
	W25Q_CS_1;     //取消片选       	      
} 
//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
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
	
	W25Q_CS_0;                            //使能器件   
	HAL_SPI_Transmit(&hspi1, cmd, 1, 10);  //发送读取命令   
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
	W25Q_CS_0;                            //使能器件   
  HAL_SPI_Transmit(&hspi1, cmd, 1, 10);   //发送写页命令   


		HAL_SPI_Transmit(&hspi1, &cmd[2], 3, 1000);  //发送24bit地址 

		HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, 4000);  //发送24bit地址 
	 
	W25Q_CS_1;                            //取消片选 
	W25QXX_Wait_Busy();					   //等待写入结束
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
  W25Q_CS_0;                            //使能器件   
  HAL_SPI_Transmit(&hspi1, cmd, 1, 10);    //发送扇区擦除指令 
	
		HAL_SPI_Transmit(&hspi1, &cmd[2], 3, 4000);  //发送24bit地址  

	W25Q_CS_1;                            //取消片选     	      
  W25QXX_Wait_Busy();   				    //等待擦除完成
}  
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;
	uint16_t NumByteToWriteNow;
	pageremain = NumByteOfPage - WriteAddr % NumByteOfPage; //单页剩余的字节数		 
	NumByteToWriteNow = NumByteToWrite;
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWriteNow;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWriteNow==pageremain)
			break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWriteNow-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWriteNow>NumByteOfPage)
				pageremain=NumByteOfPage; //一次可以写入256个字节
			else 
				pageremain=NumByteToWriteNow; 	  //不够256个字节了
		}
	}    
} 

