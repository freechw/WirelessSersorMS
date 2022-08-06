#include "ioCC2530.h" 
#include "hal_types.h"
#include "gas.h"

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

