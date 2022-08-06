#include"hal_types.h"
#include"light.h"
uchar lightdata[5];
uint16 myApp_ReadLightLevel( void )
{
  uint16 reading;
  /* Enable channel */
  ADCCFG |= 0x40;
  /* writing to this register starts the extra conversion */
  ADCCON3 = 0x86;
  /* Wait for the conversion to be done */
  while (!(ADCCON1 & 0x80));
  
  /* Disable channel after done conversion */
  ADCCFG &= (0x40 ^ 0xFF);
  
  /* Read the result */
  reading = ADCL;
  reading |= (int16) (ADCH << 8); 
  reading >>= 8;
  return reading;
}
uchar *send_LightLevel(void)
{
     uint16 LightLevel = myApp_ReadLightLevel();
     lightdata[0] ='$';
     if(LightLevel/10>10)
     {
       lightdata[1] = LightLevel / 100 + '0';
     }
     else
     {
      lightdata[1] = LightLevel / 10 + '0';
     }
      lightdata[2] = LightLevel % 10 + '0';
     lightdata[3] ='#';
     lightdata[4] ='\0';
     return lightdata;
}