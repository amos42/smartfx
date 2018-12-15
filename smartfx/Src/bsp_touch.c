#include "bsp_touch.h"
#include "threads.h"

#define I2c3Handle				hi2c3
#define I2C3_TIMEOUT			1000       // Timeout Zeit in ms
#define dbg_prn						printf


Touch_Data_t Touch_Data = {TOUCH_RELEASED, 0, 0}; 
MultiTouch_Data_t MultiTouch_Data; 
//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
uint8_t P_Touch_ReadID(void);
uint8_t P_Touch_GetContacts(void);
uint8_t P_Touch_GetPositions(void);




//--------------------------------------------------------------
uint8_t FT5336_TOUCH_REG[5][4]; // fuer Register Adressen

//--------------------------------------------------------------
#define    I2C3_MULTIBYTE_ANZ   10         // anzahl der Bytes
uint8_t    I2C3_DATA[I2C3_MULTIBYTE_ANZ];  // Array



int16_t UB_I2C3_ReadByte(uint8_t slave_adr, uint8_t adr)
{
  int16_t ret_wert=0;

  I2C3_DATA[0]=adr;

  // adresse senden
  if(HAL_I2C_Master_Transmit(&I2c3Handle, (uint16_t)slave_adr, (uint8_t*)I2C3_DATA, 1, I2C3_TIMEOUT)!= HAL_OK)
  {
    ret_wert=-1;
    if (HAL_I2C_GetError(&I2c3Handle) != HAL_I2C_ERROR_AF) ret_wert=-2;
  }  
  if(ret_wert!=0) return(ret_wert);
  
  // daten lesen
  if(HAL_I2C_Master_Receive(&I2c3Handle, (uint16_t)slave_adr, (uint8_t *)I2C3_DATA, 1, I2C3_TIMEOUT) != HAL_OK)
  {
    ret_wert=-3;
    if (HAL_I2C_GetError(&I2c3Handle) != HAL_I2C_ERROR_AF) ret_wert=-4;
  } 
  if(ret_wert!=0) return(ret_wert);

  // daten
  ret_wert=I2C3_DATA[0];

  return(ret_wert);
}


//--------------------------------------------------------------
// Beschreiben einer Adresse per I2C von einem Slave
// slave_adr => I2C-Basis-Adresse vom Slave
// adr       => Register Adresse die beschrieben wird
// wert      => Bytewert der geschrieben wird
//
// Return_wert :
//    0   , Ok
//  < 0   , Error
//--------------------------------------------------------------
int16_t UB_I2C3_WriteByte(uint8_t slave_adr, uint8_t adr, uint8_t wert)
{
  int16_t ret_wert=0;  

  I2C3_DATA[0]=adr;
  I2C3_DATA[1]=wert;
  
  // adresse und wert senden
  if(HAL_I2C_Master_Transmit(&I2c3Handle, (uint16_t)slave_adr, (uint8_t*)I2C3_DATA, 2, I2C3_TIMEOUT)!= HAL_OK)
  {
    ret_wert=-1;
    if (HAL_I2C_GetError(&I2c3Handle) != HAL_I2C_ERROR_AF) ret_wert=-2;
  } 

  return(ret_wert);
}


#if 1


//--------------------------------------------------------------
// Init vom Touch
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//-------------------------------------------------------------- 
ErrorStatus UB_Touch_Init(void)
{
  uint8_t touch_id=0,n; 

  Touch_Data.status=TOUCH_RELEASED;
  Touch_Data.xp=0;
  Touch_Data.yp=0;  

  MultiTouch_Data.cnt=0;
  for(n=0;n<TOUCH_MAX_CONTACT;n++) {
    MultiTouch_Data.p[n].xp=0;
    MultiTouch_Data.p[n].yp=0;
  }  

  // init der Touch Register Adressen
  FT5336_TOUCH_REG[0][0]=FT5336_P1_XL_REG;
  FT5336_TOUCH_REG[0][1]=FT5336_P1_XH_REG;
  FT5336_TOUCH_REG[0][2]=FT5336_P1_YL_REG;
  FT5336_TOUCH_REG[0][3]=FT5336_P1_YH_REG;

  FT5336_TOUCH_REG[1][0]=FT5336_P2_XL_REG;
  FT5336_TOUCH_REG[1][1]=FT5336_P2_XH_REG;
  FT5336_TOUCH_REG[1][2]=FT5336_P2_YL_REG;
  FT5336_TOUCH_REG[1][3]=FT5336_P2_YH_REG;

  FT5336_TOUCH_REG[2][0]=FT5336_P3_XL_REG;
  FT5336_TOUCH_REG[2][1]=FT5336_P3_XH_REG;
  FT5336_TOUCH_REG[2][2]=FT5336_P3_YL_REG;
  FT5336_TOUCH_REG[2][3]=FT5336_P3_YH_REG;

  FT5336_TOUCH_REG[3][0]=FT5336_P4_XL_REG;
  FT5336_TOUCH_REG[3][1]=FT5336_P4_XH_REG;
  FT5336_TOUCH_REG[3][2]=FT5336_P4_YL_REG;
  FT5336_TOUCH_REG[3][3]=FT5336_P4_YH_REG;

  FT5336_TOUCH_REG[4][0]=FT5336_P5_XL_REG;
  FT5336_TOUCH_REG[4][1]=FT5336_P5_XH_REG;
  FT5336_TOUCH_REG[4][2]=FT5336_P5_YL_REG;
  FT5336_TOUCH_REG[4][3]=FT5336_P5_YH_REG;

  // init vom I2C
  //UB_I2C3_Init();

  // kleine Pause
  HAL_Delay(TOUCH_INIT_DELAY);    
  
  touch_id=P_Touch_ReadID();
  if(touch_id!=FT5336_ID) return ERROR;  

  return(SUCCESS);
}


//--------------------------------------------------------------
// auslesen vom Touch-Status und der Touch-Koordinaten
//
// es kann entweder der Single-Touch "Touch_Data"
// oder der Multi-Touch "MultiTouch_Data" benutzt werden 
//
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//
// Touch_Data.status : [TOUCH_PRESSED, TOUCH_RELEASED]
// Touch_Data.xp     : [0...479]
// Touch_Data.yp     : [0...271] 
//
// MultiTouch_Data.cnt            : [0...5]
// MultiTouch_Data.p[0..4].xp     : [0...479]
// MultiTouch_Data.p[0..4].yp     : [0...271] 
//-------------------------------------------------------------- 
ErrorStatus UB_Touch_Read(void)
{
  uint8_t check,n;

  // init
  Touch_Data.status=TOUCH_RELEASED;
  Touch_Data.xp=0;
  Touch_Data.yp=0;

  MultiTouch_Data.cnt=0;
  for(n=0;n<TOUCH_MAX_CONTACT;n++) {
    MultiTouch_Data.p[n].xp=0;
    MultiTouch_Data.p[n].yp=0;
  }

  check=P_Touch_GetContacts();
  if(check>TOUCH_MAX_CONTACT) return ERROR;

  if(check>0) {
    // wenn touch betaetigt
    MultiTouch_Data.cnt=check;
    Touch_Data.status=TOUCH_PRESSED;
    check=P_Touch_GetPositions();
	 
    if(check>TOUCH_MAX_CONTACT) {
      // fehler beim lesen
      Touch_Data.status=TOUCH_RELEASED;
      Touch_Data.xp=0;
      Touch_Data.yp=0;

      MultiTouch_Data.cnt=0;
      for(n=0;n<TOUCH_MAX_CONTACT;n++) {
        MultiTouch_Data.p[n].xp=0;
        MultiTouch_Data.p[n].yp=0;
      }
      return ERROR;
    }    
    // erste koordinate fuer single touch speichern
    Touch_Data.xp=MultiTouch_Data.p[0].xp;
    Touch_Data.yp=MultiTouch_Data.p[0].yp;
  }

  return(SUCCESS);
}


//--------------------------------------------------------------
// interne Funktion
// ID auslesen (mehrmals versuchen)
// bei Fehler -> return = 0
//--------------------------------------------------------------
uint8_t P_Touch_ReadID(void)
{  
  int16_t i2c_wert;
  uint8_t n;

  for(n=0;n<5;n++) {
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_ID_REG);
	 dbg_prn("touch device id = 0x%02x\r\n", i2c_wert);
	 
    if(i2c_wert>0) {
      if((i2c_wert&0xFF)==FT5336_ID) return(FT5336_ID);
    }
  }  

  return 0;
}


//--------------------------------------------------------------
// interne Funktion
// anzahl der Touch Kontakte ermitteln
// ret_wert : 0...5 = Anzahl der Kontakte
//               99 = Error
//--------------------------------------------------------------
// contact 된게 있나 읽어보기...
uint8_t P_Touch_GetContacts(void)
{
  uint8_t ret_wert=0;
  int16_t i2c_wert;  
  
  i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_STATUS_REG);
  if(i2c_wert<0) return 99;

  // wert maskieren
  ret_wert = (uint8_t)(i2c_wert & FT5336_STATUS_MASK);

  if(ret_wert>TOUCH_MAX_CONTACT) return 0;

  #if USE_MULTITOUCH==0
    // bei Singletouch maximal ein Kontakt
    if(ret_wert>1) ret_wert=1;
  #endif

  return(ret_wert);
}


//--------------------------------------------------------------
// interne Funktion
// die Touch positionen von allen Kontakten ermitteln
// ret_wert :     0 = OK
//               99 = Error
//--------------------------------------------------------------
// interne Funktion
// P_Touch_GetContacts() 함수 이용해서 touch가 있으면 position 읽어 들임.
uint8_t P_Touch_GetPositions(void)
{
  uint8_t n,adr,wert_lo,wert_hi;
  int16_t i2c_wert;
  uint16_t position;

  if(MultiTouch_Data.cnt==0) return 0;

  // alle daten einlesen
  for(n=0;n<MultiTouch_Data.cnt;n++) {
    // x_lo
    adr=FT5336_TOUCH_REG[n][0];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_lo = i2c_wert & FT5336_LO_MASK;
    // x_hi
    adr=FT5336_TOUCH_REG[n][1];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_hi = i2c_wert & FT5336_HI_MASK;    
    position=(wert_hi<<8)|wert_lo;
    if(position>=TOUCH_MAXY) position=(TOUCH_MAXY-1);
    MultiTouch_Data.p[n].yp=position;
    // y_lo
    adr=FT5336_TOUCH_REG[n][2];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_lo = i2c_wert & FT5336_LO_MASK;
    // y_hi
    adr=FT5336_TOUCH_REG[n][3];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_hi = i2c_wert & FT5336_HI_MASK;    
    position=(wert_hi<<8)|wert_lo;
    if(position>=TOUCH_MAXX) position=(TOUCH_MAXX-1);
    MultiTouch_Data.p[n].xp=position;
  }

  return 0;
}

void UB_Touch_EnableIT(void)
{
	uint8_t regValue = 0x01;
	 /* Set interrupt polling mode in FT5336_GMODE_REG */
	UB_I2C3_WriteByte(FT5336_I2C_ADDR, FT5336_GMODE_REG, regValue);
}

void UB_Touch_DisableIT(void)
{
	uint8_t regValue = 0;
	 /* Set interrupt polling mode in FT5336_GMODE_REG */
	UB_I2C3_WriteByte(FT5336_I2C_ADDR, FT5336_GMODE_REG, regValue);
}

uint32_t UB_Touch_GetGestureID(void)
{
  volatile uint8_t ucReadData = 0;

  ucReadData = UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_GEST_ID_REG);

  return ucReadData;
}

extern void _touchcall(int up_down, int x, int y);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static Touch_Status_t sts_pre =  TOUCH_RELEASED, sts_cur = TOUCH_RELEASED;
	static uint16_t xp_pre = 0, yp_pre = 0;
	static uint16_t xp_cur, yp_cur;
	int8_t edge;			// falling edge = 0, rising edge = 1;
	//dbg_prn("*");
	if (UB_Touch_Read() == SUCCESS) {
		sts_cur = Touch_Data.status;
		xp_cur = Touch_Data.xp;
		yp_cur = Touch_Data.yp;

		if (sts_pre == TOUCH_PRESSED && sts_cur == TOUCH_RELEASED) 				{ edge = 1;	 	} //printf("Rising Edge\r\n"); } //gBleObj.connected = D_BSP_TRUE; 	}		// rising edge
		else if (sts_pre == TOUCH_RELEASED && sts_cur == TOUCH_PRESSED)	{ edge = 0;		} //printf("Falling Edge\r\n"); } //gBleObj.connected = D_BSP_FALSE;	}		// falling edge
		else edge = -1;
		sts_pre = sts_cur;
//		dbg_prn("ts_cnt=%d\r\n", MultiTouch_Data.cnt);
//		dbg_prn("sts=%d\r\n", Touch_Data.status);
//		dbg_prn("coord=%4d,%4d\r\n", Touch_Data.yp, Touch_Data.xp);
		if (edge == 0) {
			gAppObj.touch_sts = 0;
			gAppObj.touch_xp = Touch_Data.yp;
			gAppObj.touch_yp = Touch_Data.xp;
			osSignalSet(D_EVENT_THREAD, D_EVT_TOUCH);
			//_touchcall(0, Touch_Data.yp, Touch_Data.xp);
			//dbg_prn("coord=%4d,%4d\r\n", Touch_Data.yp, Touch_Data.xp);
			
		} else if (edge == 1) {
			gAppObj.touch_sts = 1;
			gAppObj.touch_xp = yp_pre;
			gAppObj.touch_yp = xp_pre;
			osSignalSet(D_EVENT_THREAD, D_EVT_TOUCH);
			//_touchcall(1, yp_pre, xp_pre);
			//dbg_prn("coord=%4d,%4d\r\n", yp_pre, xp_pre);
		}
		
		xp_pre = xp_cur; //Touch_Data.xp;
		yp_pre = yp_cur; //Touch_Data.yp;
	}
}

//////////////////
// touch screen driver

// touch reset (low active)
void bsp_ts_rst_enable(void)
{
	HAL_GPIO_WritePin(D_LCD_TS_RST_N_GPIO_Port, D_LCD_TS_RST_N_Pin, GPIO_PIN_RESET); 	
}

void bsp_ts_rst_disable(void)
{
	HAL_GPIO_WritePin(D_LCD_TS_RST_N_GPIO_Port, D_LCD_TS_RST_N_Pin, GPIO_PIN_SET); 	
}

/*
#define D_DEVADDR_				0xe0				// back light

int8_t bsp_ts_i2c_tx(uint8_t *pData, uint16_t Size)
{
	while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)D_DEVADDR_SC620, pData, Size, 3000)!= HAL_OK) 
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	}
	return -1;
}

int8_t bsp_ts_i2c_rx(uint8_t *pData, uint16_t Size)
{
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)D_DEVADDR_SC620, (uint8_t *)pData, Size, 3000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	 }
	 return -1;
}
*/

#else

//--------------------------------------------------------------
// Init vom Touch
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//-------------------------------------------------------------- 
ErrorStatus UB_Touch_Init(void)
{
  uint8_t touch_id=0,n; 

  Touch_Data.status=D_TS_KEY_NONE;
  Touch_Data.xp=0;
  Touch_Data.yp=0;  

  MultiTouch_Data.evt_prev = 0; // no event
  MultiTouch_Data.evt_cur = 0; // no event
  MultiTouch_Data.cnt=0;
  for(n=0;n<TOUCH_MAX_CONTACT;n++) 
  {
  
    MultiTouch_Data.p[n].xp=0;
    MultiTouch_Data.p[n].yp=0;
  }  

  // init der Touch Register Adressen
  FT5336_TOUCH_REG[0][0]=FT5336_P1_XL_REG;
  FT5336_TOUCH_REG[0][1]=FT5336_P1_XH_REG;
  FT5336_TOUCH_REG[0][2]=FT5336_P1_YL_REG;
  FT5336_TOUCH_REG[0][3]=FT5336_P1_YH_REG;

  FT5336_TOUCH_REG[1][0]=FT5336_P2_XL_REG;
  FT5336_TOUCH_REG[1][1]=FT5336_P2_XH_REG;
  FT5336_TOUCH_REG[1][2]=FT5336_P2_YL_REG;
  FT5336_TOUCH_REG[1][3]=FT5336_P2_YH_REG;

  FT5336_TOUCH_REG[2][0]=FT5336_P3_XL_REG;
  FT5336_TOUCH_REG[2][1]=FT5336_P3_XH_REG;
  FT5336_TOUCH_REG[2][2]=FT5336_P3_YL_REG;
  FT5336_TOUCH_REG[2][3]=FT5336_P3_YH_REG;

  FT5336_TOUCH_REG[3][0]=FT5336_P4_XL_REG;
  FT5336_TOUCH_REG[3][1]=FT5336_P4_XH_REG;
  FT5336_TOUCH_REG[3][2]=FT5336_P4_YL_REG;
  FT5336_TOUCH_REG[3][3]=FT5336_P4_YH_REG;

  FT5336_TOUCH_REG[4][0]=FT5336_P5_XL_REG;
  FT5336_TOUCH_REG[4][1]=FT5336_P5_XH_REG;
  FT5336_TOUCH_REG[4][2]=FT5336_P5_YL_REG;
  FT5336_TOUCH_REG[4][3]=FT5336_P5_YH_REG;

  // init vom I2C
  //UB_I2C3_Init();

  // kleine Pause
  HAL_Delay(TOUCH_INIT_DELAY);    
  
  touch_id=P_Touch_ReadID();
  if(touch_id!=FT5336_ID) return ERROR;  

  return(SUCCESS);
}


//--------------------------------------------------------------
// auslesen vom Touch-Status und der Touch-Koordinaten
//
// es kann entweder der Single-Touch "Touch_Data"
// oder der Multi-Touch "MultiTouch_Data" benutzt werden 
//
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//
// Touch_Data.status : [TOUCH_PRESSED, TOUCH_RELEASED]
// Touch_Data.xp     : [0...479]
// Touch_Data.yp     : [0...271] 
//
// MultiTouch_Data.cnt            : [0...5]
// MultiTouch_Data.p[0..4].xp     : [0...479]
// MultiTouch_Data.p[0..4].yp     : [0...271] 
//-------------------------------------------------------------- 

#define D_TS_KEY_STS_PUSH			1		// key를 눌렀을 때 key 값
#define D_TS_KEY_STS_RELEASE		0		// key를 안눌렀을 때 key 값

ErrorStatus UB_Touch_Read(void)
{
  uint8_t check, n;


  // init
  Touch_Data.status = D_TS_KEY_NONE;
  Touch_Data.xp = 0;
  Touch_Data.yp = 0;

  MultiTouch_Data.cnt = 0;
  
  for (n=0; n<TOUCH_MAX_CONTACT; n++) 
  {
		MultiTouch_Data.p[n].xp = 0;
		MultiTouch_Data.p[n].yp = 0;
  }

  check=P_Touch_GetContacts();
//  dbg_prn("cont = %d\r\n", check);
  
  if (check>TOUCH_MAX_CONTACT) return ERROR;

  if (check > 0) 
  {
		// wenn touch betaetigt
		MultiTouch_Data.cnt = check;
		Touch_Data.status=D_TS_KEY_RELEASE;

		check = P_Touch_GetPositions();
	 
		if (check > TOUCH_MAX_CONTACT) 
		{
			// fehler beim lesen
			Touch_Data.status = D_TS_KEY_NONE; //TOUCH_RELEASED;
			Touch_Data.xp = 0;
			Touch_Data.yp = 0;

			MultiTouch_Data.cnt = 0;
			
			for (n = 0; n < TOUCH_MAX_CONTACT; n++) 
			{
				MultiTouch_Data.p[n].xp = 0;
				MultiTouch_Data.p[n].yp = 0;
			}
		
			return ERROR;
		}    
		// erste koordinate fuer single touch speichern
		Touch_Data.xp = MultiTouch_Data.p[0].xp;
		Touch_Data.yp = MultiTouch_Data.p[0].yp;
	}

	return(SUCCESS);
}


//--------------------------------------------------------------
// interne Funktion
// ID auslesen (mehrmals versuchen)
// bei Fehler -> return = 0
//--------------------------------------------------------------
uint8_t P_Touch_ReadID(void)
{  
	int16_t i2c_wert;
	uint8_t n;

	for (n = 0; n < 5; n++) {
		i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_ID_REG);
		dbg_prn("touch device id = 0x%02x\r\n", i2c_wert);

		if (i2c_wert > 0) {
			if ((i2c_wert & 0xFF) == FT5336_ID) return (FT5336_ID);
		}
	}  

	return 0;
}


//--------------------------------------------------------------
// interne Funktion
// anzahl der Touch Kontakte ermitteln
// ret_wert : 0...5 = Anzahl der Kontakte
//               99 = Error
//--------------------------------------------------------------
// contact 된게 있나 읽어보기...
uint8_t P_Touch_GetContacts(void)
{
	uint8_t ret_wert=0;
	int16_t i2c_wert;  

	i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_STATUS_REG);
	if (i2c_wert < 0) return 99;

	// wert maskieren
	ret_wert = (uint8_t)(i2c_wert & FT5336_STATUS_MASK);

	if (ret_wert > TOUCH_MAX_CONTACT) return 0;

#if USE_MULTITOUCH==0
    // bei Singletouch maximal ein Kontakt
    if (ret_wert > 1) ret_wert =1;
#endif

	return(ret_wert);
}


//--------------------------------------------------------------
// interne Funktion
// die Touch positionen von allen Kontakten ermitteln
// ret_wert :     0 = OK
//               99 = Error
//--------------------------------------------------------------
// interne Funktion
// P_Touch_GetContacts() 함수 이용해서 touch가 있으면 position 읽어 들임.
uint8_t P_Touch_GetPositions(void)
{
	static uint8_t evt_prev = 0, evt_cur = 0;
	
	uint8_t n,adr,wert_lo,wert_hi;
	int16_t i2c_wert;
	uint16_t position;
	uint8_t event;

  if (MultiTouch_Data.cnt==0) return 0;

  // alle daten einlesen
  for (n = 0; n < MultiTouch_Data.cnt; n++) 
  {
		// x_lo
		adr = FT5336_TOUCH_REG[n][0];
		i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
		//dbg_prn("1.adr=%02x, data = %02x\r\n", adr, i2c_wert);
		
		if (i2c_wert < 0) return 99;
		wert_lo = i2c_wert & FT5336_LO_MASK;
		
		// x_hi
		adr = FT5336_TOUCH_REG[n][1];
		i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
		//dbg_prn("2.adr=%02x, data = %02x\r\n", adr, i2c_wert);

		if (i2c_wert < 0) return 99;
		wert_hi = i2c_wert & FT5336_HI_MASK;    

		position = (wert_hi << 8) | wert_lo;
		event = (i2c_wert & 0xc0) >> 6;

		if (position >= TOUCH_MAXY) position = (TOUCH_MAXY-1);
		MultiTouch_Data.p[n].yp = position;
		MultiTouch_Data.evt_cur = event;
		
		// y_lo
		adr = FT5336_TOUCH_REG[n][2];
		i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
		//dbg_prn("3.adr=%02x, data = %02x\r\n", adr, i2c_wert);

		if (i2c_wert < 0) return 99;
		wert_lo = i2c_wert & FT5336_LO_MASK;
		// y_hi
		adr = FT5336_TOUCH_REG[n][3];
		i2c_wert = UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
		//dbg_prn("4.adr=%02x, data = %02x\r\n", adr, i2c_wert);

		if (i2c_wert < 0) return 99;
		wert_hi = i2c_wert & FT5336_HI_MASK;    
		position = (wert_hi <<8) | wert_lo;
		if (position >= TOUCH_MAXX) position = (TOUCH_MAXX-1);
		MultiTouch_Data.p[n].xp = position;
	}

	return 0;
}

void UB_Touch_EnableIT(void)
{
	uint8_t regValue = 0x01;
	 /* Set interrupt polling mode in FT5336_GMODE_REG */
	UB_I2C3_WriteByte(FT5336_I2C_ADDR, FT5336_GMODE_REG, regValue);
}

void UB_Touch_DisableIT(void)
{
	uint8_t regValue = 0;
	 /* Set interrupt polling mode in FT5336_GMODE_REG */
	UB_I2C3_WriteByte(FT5336_I2C_ADDR, FT5336_GMODE_REG, regValue);
}

uint32_t UB_Touch_GetGestureID(void)
{
  volatile uint8_t ucReadData = 0;

  ucReadData = UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_GEST_ID_REG);

  return ucReadData;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	dbg_prn("*");
	if (UB_Touch_Read() == SUCCESS) {
		dbg_prn("ts_cnt=%d\r\n", MultiTouch_Data.cnt);
		dbg_prn("sts=%d\r\n", Touch_Data.status);
//		dbg_prn("evt=%d\r\n", Touch_Data.
		dbg_prn("coord=%4d,%4d\r\n", Touch_Data.yp, Touch_Data.xp);
	}
}


//////////////////
// touch screen driver

// touch reset (low active)
void bsp_ts_rst_enable(void)
{
	HAL_GPIO_WritePin(D_LCD_TS_RST_N_GPIO_Port, D_LCD_TS_RST_N_Pin, GPIO_PIN_RESET); 	
}

void bsp_ts_rst_disable(void)
{
	HAL_GPIO_WritePin(D_LCD_TS_RST_N_GPIO_Port, D_LCD_TS_RST_N_Pin, GPIO_PIN_SET); 	
}

/*
#define D_DEVADDR_				0xe0				// back light

int8_t bsp_ts_i2c_tx(uint8_t *pData, uint16_t Size)
{
	while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)D_DEVADDR_SC620, pData, Size, 3000)!= HAL_OK) 
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	}
	return -1;
}

int8_t bsp_ts_i2c_rx(uint8_t *pData, uint16_t Size)
{
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)D_DEVADDR_SC620, (uint8_t *)pData, Size, 3000) != HAL_OK)
	{
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{
			return -1;
		}
		return 0;
	 }
	 return -1;
}
*/
#endif

