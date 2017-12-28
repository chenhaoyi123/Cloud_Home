#include "nRF24L01_API.h"
#include "sys.h"

SPI_InitTypeDef  SPI_InitStructure; //È«¾Ö±äÁ¿spi



/****************************      ³õÊ¼»¯NRF24L01_Init      *************************/
void NRF24L01_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure; 
	  
		//Ê¹ÄÜGPIOA, SPI1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );    
	
		//³õÊ¼»¯SPI1µÄGPIO¿Ú
    //PA5.SPI1_SCK; PA6.SPI_MISO; PA7.SPI_MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //¸´ÓÃÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

		//PA4.CSN<-->SPI_NSS; 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
    //PC4.CE 24L01Æ¬Ñ¡ĞÅºÅ;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

		//PC5.IRQ IRQÖ÷»úÊı¾İÊäÈë
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;     //ÉÏÀ­ÊäÈë
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  


    //PC5(ÖĞ¶ÏĞÅºÅÒı½Å)ÖÃ¸ß,ÖĞ¶ÏÊ±±äÎªµÍµçÆ½;PC4(24L01Æ¬Ñ¡ĞÅºÅ)ÖÃ¸ß
    GPIO_SetBits(GPIOC,GPIO_Pin_5|GPIO_Pin_4);

		/** ÉèÖÃSPI **/
    //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊı¾İÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
		
		//ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		
		//ÉèÖÃSPIµÄÊı¾İ´óĞ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		
		//Ñ¡ÔñÁË´®ĞĞÊ±ÖÓµÄÎÈÌ¬:Ê±ÖÓĞü¿ÕµÍµçÆ½
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//¼´(CPOL=0)
		
		//Êı¾İ²¶»ñÓÚµÚÒ»¸öÊ±ÖÓÑØ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//¼´(CPHA=0)
    
		//NSSĞÅºÅÓÉÓ²¼ş£¨NSS¹Ü½Å£©»¹ÊÇÈí¼ş£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSĞÅºÅÓĞSSIÎ»¿ØÖÆ
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		
    //¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª8;spiËÙ¶ÈÎª9Mhz£¨24L01µÄ×î´óSPIÊ±ÖÓÎª10Mhz£© 
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	
		
    //Ö¸¶¨Êı¾İ´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êı¾İ´«Êä´ÓMSBÎ»¿ªÊ¼
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		
		//CRCÖµ¼ÆËãµÄ¶àÏîÊ½
    SPI_InitStructure.SPI_CRCPolynomial = 7;
		
		//¸ù¾İSPI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ 
		SPI_Init(SPI1, &SPI_InitStructure);
		
		//Ê¹ÄÜSPI1
    SPI_Cmd(SPI1,ENABLE);

    //CEµÍµçÆ½ ´ı»úÄ£Ê½
    NRF24L01_CE=0;  //Ê¹ÄÜ24L01
		
    //CSNÎªµÍµçÆ½Ğ¾Æ¬¹¤×÷
    NRF24L01_CSN=1; //SPIÆ¬Ñ¡È¡Ïû            
}


/**********************             SPI1µÄ·¢ËÍÓë½ÓÊÕ         ************************/

//Í¨¹ıSPI1·¢ËÍ»òÕß½ÓÊÕÒ»¸ö×Ö½ÚµÄÊı¾İ
u8 SPI1_ReadWriteByte(u8 TxData)
{       
    u8 retry=0;                 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //¼ì²é·¢ËÍ»º´æ¿Õ±êÖ¾Î»ÉèÖÃÓë·ñ
        {
        retry++;
        if(retry>200)return 0;
        }             
    SPI_I2S_SendData(SPI1, TxData); //Í¨¹ıÍâÉèSPI1·¢ËÍÒ»¸öÊı¾İ
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//¼ì²é½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»ÉèÖÃÓë·ñ
        {
        retry++;
        if(retry>200)return 0;
        }                               
    return SPI_I2S_ReceiveData(SPI1); //·µ»ØÍ¨¹ıSPI1×î½ü½ÓÊÕµÄÊı¾İ                     
}


/*********************    ÏònRF24L01Ö¸¶¨¼Ä´æÆ÷Ğ´Èë»òÕß¶ÁÈ¡     ************************/

//SPIĞ´¼Ä´æÆ÷
//Èë¿Ú²ÎÊı: reg:Ö¸¶¨¼Ä´æÆ÷µØÖ·; value:Ğ´ÈëµÄÖµ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;  
    NRF24L01_CSN=0;                 //Ê¹ÄÜSPI´«Êä
    status =SPI1_ReadWriteByte(reg);//·¢ËÍ¼Ä´æÆ÷ºÅ 
    SPI1_ReadWriteByte(value);      //Ğ´Èë¼Ä´æÆ÷µÄÖµ
    NRF24L01_CSN=1;                 //½ûÖ¹SPI´«Êä      
    return(status);                 //·µ»Ø×´Ì¬Öµ
}

//¶ÁÈ¡SPI¼Ä´æÆ÷Öµ
//Èë¿Ú²ÎÊı:reg:Òª¶ÁµÄ¼Ä´æÆ÷
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;     
    NRF24L01_CSN = 0;          //Ê¹ÄÜSPI´«Êä        
    SPI1_ReadWriteByte(reg);   //·¢ËÍ¼Ä´æÆ÷ºÅ
    reg_val=SPI1_ReadWriteByte(0XFF);//¶ÁÈ¡¼Ä´æÆ÷ÄÚÈİ
    NRF24L01_CSN = 1;          //½ûÖ¹SPI´«Êä            
    return(reg_val);           //·µ»Ø×´Ì¬Öµ
}  


/**********************       ÏònRF24L01Ö¸¶¨¼Ä´æÆ÷Ğ´Èë»òÕß¶ÁÈ¡      *********************/

//ÔÚÖ¸¶¨Î»ÖÃ¶Á³öÖ¸¶¨³¤¶ÈµÄÊı¾İ
//Èë¿Ú²ÎÊı:reg:¼Ä´æÆ÷(Î»ÖÃ); *pBuf:Êı¾İÖ¸Õë; len:Êı¾İ³¤¶È
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;          
  NRF24L01_CSN = 0;           //Ê¹ÄÜSPI´«Êä
  status=SPI1_ReadWriteByte(reg);//·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ       
    for(u8_ctr=0;u8_ctr<len;u8_ctr++)
        pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//¶Á³öÊı¾İ
  NRF24L01_CSN=1;       //¹Ø±ÕSPI´«Êä
  return status;        //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}

//ÔÚÖ¸¶¨Î»ÖÃĞ´Ö¸¶¨³¤¶ÈµÄÊı¾İ
//Èë¿Ú²ÎÊı:reg:¼Ä´æÆ÷(Î»ÖÃ); *pBuf:Êı¾İÖ¸Õë; len:Êı¾İ³¤¶È
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;       
    NRF24L01_CSN = 0;          //Ê¹ÄÜSPI´«Êä
  status = SPI1_ReadWriteByte(reg);//·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
            SPI1_ReadWriteByte(*pBuf++); //Ğ´ÈëÊı¾İ  
  NRF24L01_CSN = 1;       //¹Ø±ÕSPI´«Êä
  return status;          //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}


/**********************    NRF24L01µÄ·¢ËÍ(TX)ºÍ½ÓÊÕ(RX)Ä£Ê½    ************************/

//¸Ãº¯Êı³õÊ¼»¯NRF24L01µ½½ÓÊÕÄ£Ê½(RXÄ£Ê½)
//ÉèÖÃRXµØÖ·,Ğ´RXÊı¾İ¿í¶È,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊı¾İÁË          
void NRF24L01_RX_Mode(u8* Address)
{
		NRF24L01_CE=0;    
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,Address,RX_ADR_WIDTH);
		//Ğ´RX½ÚµãµØÖ·  
		NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ğ    
		NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·     
		NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);           //ÉèÖÃRFÍ¨ĞÅÆµÂÊ        
		NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//Ñ¡ÔñÍ¨µÀ0µÄÓĞĞ§Êı¾İ¿í¶È
		//¼´32¸ö×Ö½Ú
		NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //ÉèÖÃTX·¢Éä²ÎÊı,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
		NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);       //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊı;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½ 
		//ÏòCONFIG(00)Ğ´Èë0000 1111;PRIM_RX=1 ½ÓÊÕÄ£Ê½; PWR_UP=1 ÉÏµç; CRCO=1 16Î»CRCĞ£Ñé; EN_CRC =1 CRCÊ¹ÄÜ
		NRF24L01_CE = 1; //CEÎª¸ß,½øÈë½ÓÊÕÄ£Ê½ 
}

//¸Ãº¯Êı³õÊ¼»¯NRF24L01µ½·¢ËÍ(TXÄ£Ê½)
//ÉèÖÃTXµØÖ·,Ğ´TXÊı¾İ¿í¶È,ÉèÖÃRX×Ô¶¯Ó¦´ğµÄµØÖ·,Ìî³äTX·¢ËÍÊı¾İ,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//PWR_UP,CRCÊ¹ÄÜ
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊı¾İÁË          
//CEÎª¸ß´óÓÚ10us,ÔòÆô¶¯·¢ËÍ.  
void NRF24L01_TX_Mode(u8* Address)
{                                                        
		NRF24L01_CE=0;      
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,Address,TX_ADR_WIDTH);
		//Ğ´TX½ÚµãµØÖ· 
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,Address,RX_ADR_WIDTH); 
		//ÉèÖÃTX½ÚµãµØÖ·,Ö÷ÒªÎªÁËÊ¹ÄÜACK      
		NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ğ    
		NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  
		NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//ÉèÖÃ×Ô¶¯ÖØ·¢¼ä¸ôÊ±¼ä:500us + 86us;×î´ó×Ô¶¯ÖØ·¢´ÎÊı:10´Î
		NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //ÉèÖÃRFÍ¨µÀÎª40
		NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //ÉèÖÃTX·¢Éä²ÎÊı,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
		NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊı;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½,¿ªÆôËùÓĞÖĞ¶Ï
		//ÏòCONFIG(00)Ğ´Èë0000 1111;PRIM_RX=0 ·¢ÉäÄ£Ê½; PWR_UP=1 ÉÏµç; CRCO=1 16Î»CRCĞ£Ñé; EN_CRC   =1 CRCÊ¹ÄÜ
		NRF24L01_CE=1;//CEÎª¸ß,10usºóÆô¶¯·¢ËÍ
}


/*********************      Æô¶¯NRF24L01½ÓÊÕºÍ·¢ËÍÒ»´ÎÊı¾İ      **********************/

//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊı¾İ
//txbuf:´ı·¢ËÍÊı¾İÊ×µØÖ·
//·µ»ØÖµ:·¢ËÍÍê³É×´¿ö
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;  
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//Ğ´Êı¾İµ½TX BUF  32¸ö×Ö½Ú
    NRF24L01_CE=1;//Æô¶¯·¢ËÍ       
    while(NRF24L01_IRQ!=0);//µÈ´ı·¢ËÍÍê³É
    sta=NRF24L01_Read_Reg(STATUS);  //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ    
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ¾
    if(sta&MAX_TX)//´ïµ½×î´óÖØ·¢´ÎÊı
    {
        NRF24L01_Write_Reg(FLUSH_TX,0xff);//Çå³ıTX FIFO¼Ä´æÆ÷ 
        return MAX_TX; 
    }
    if(sta&TX_OK)//·¢ËÍÍê³É
    {
        return TX_OK;
    }
    return 0xff;//ÆäËûÔ­Òò·¢ËÍÊ§°Ü
}

//Æô¶¯NRF24L01½ÓÊÕÒ»´ÎÊı¾İ
//txbuf:´ı·¢ËÍÊı¾İÊ×µØÖ·
//·µ»ØÖµ:0£¬½ÓÊÕÍê³É£»ÆäËû£¬´íÎó´úÂë
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    u8 sta;                                           
    sta=NRF24L01_Read_Reg(STATUS);  //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ      
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ¾
    if(sta&RX_OK)//½ÓÊÕµ½Êı¾İ
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//¶ÁÈ¡Êı¾İ
        NRF24L01_Write_Reg(FLUSH_RX,0xff);//Çå³ıRX FIFO¼Ä´æÆ÷ 
        return 0; 
    }      
    return 1;//Ã»ÊÕµ½ÈÎºÎÊı¾İ
}   


//¼ì²â24L01ÊÇ·ñ´æÔÚ
//·µ»ØÖµ:0£¬³É¹¦;1£¬Ê§°Ü 
u8 NRF24L01_Check(void)
{
    u8 buf[5]={0XAA,0XAA,0XAA,0XAA,0XAA};//Ğ´Èë5¸ö0XAA×Ö½Ú
    u8 i;    
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//Ğ´Èë5¸ö×Ö½ÚµÄµØÖ·.    
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //¶Á³öĞ´ÈëµÄµØÖ·  
    for(i=0;i<5;i++)
        if(buf[i]!=0XAA)
            break;                                 
    if(i!=5)return 1;//¼ì²â24L01´íÎó    
    return 0;        //¼ì²âµ½24L01
}
