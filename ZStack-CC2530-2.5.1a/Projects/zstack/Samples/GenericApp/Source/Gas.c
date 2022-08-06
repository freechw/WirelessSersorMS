/******************************************************************************/
#include"hal_types.h"
#include"Gas_head.h"
/*********气体变量************/
uchar GasData[SIZE];
//char  Wrap = 0x0A;
uint16 GasLevel;
/*********************/
uint16 myApp_ReadGasLevel( void )
{
  uint16 reading = 0;
  
  /* Enable channel */
  ADCCFG |= 0x80;
  
  /* writing to this register starts the extra conversion */
  ADCCON3 = 0x87;
  
  /* Wait for the conversion to be done */
  while (!(ADCCON1 & 0x80));
  
  /* Disable channel after done conversion */
  ADCCFG &= (0x80 ^ 0xFF);
  
  /* Read the result */
  reading = ADCL;
  reading |= (int16) (ADCH << 8); 
  
  reading >>= 8;
  
  return (reading);
}
uchar *GAS_readAndsend(void)
{
      GasLevel = myApp_ReadGasLevel();
      GasData[0] ='$';
      GasData[1] = GasLevel / 100 + '0';
      GasData[2] = GasLevel / 10%10 + '0';
      GasData[3] = GasLevel % 10 + '0';
      GasData[4] ='#';
      GasData[5]='\0';
      return GasData;
}