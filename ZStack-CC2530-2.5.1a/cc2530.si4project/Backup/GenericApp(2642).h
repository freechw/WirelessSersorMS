/**************************************************************************************************
  Filename:       GenericApp.h
  Revised:        $Date: 2012-02-12 15:58:41 -0800 (Sun, 12 Feb 2012) $
  Revision:       $Revision: 29216 $

  Description:    This file contains the Generic Application definitions.


  Copyright 2004-2012 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef GENERICAPP_H
#define GENERICAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"

/*********************************************************************
 * CONSTANTS
 */

// These constants are only for example and should be changed to the
// device's needs
#define GENERICAPP_ENDPOINT           10

#define GENERICAPP_PROFID             0x0F04
#define GENERICAPP_DEVICEID           0x0001
#define GENERICAPP_DEVICE_VERSION     0
#define GENERICAPP_FLAGS              0
/*********************************************************************/
//web 上位机新建
typedef  unsigned char uint8_t;
typedef  char int8_t;
typedef  unsigned int uint32_t;


struct _EnvMsg
{
	uint8_t head[3];	//标识位st:   73 74 3A 65 
	uint8_t type;			//数据类型

	uint8_t temp[2];	//温度 00 00 00 00 
	uint8_t hum[2];		//湿度 
	uint8_t ep_no;			//仓库编号    01
	
	int8_t x;			 		//三轴信息
	int8_t y;			 
	int8_t z;
	unsigned int hongwai;   //01 00
	unsigned int lux;			//光照  2个字节  6F 00 
	unsigned int rsv2;
	unsigned int gas;			//烟雾	   7F 00
	unsigned int rsv3;
	unsigned int adc; 		
};


#define GENERICAPP_MAX_CLUSTERS           4
#define GENERICAPP_CLUSTERID              1      //协调器

#define GENERICAPP_CLUSTERID1              1   //节点1
#define GENERICAPP_CLUSTERID2              2  //节点2
#define GENERICAPP_CLUSTERID3              3
#define GENERICAPP_CLUSTERID4              4


#define STO_MASK	0xc0 // 节点号掩码
#define DEV_MASK	0x30 // 设备号掩码 
#define CMD_MASK	0x0f // 命令字掩码

#define RELY		0x00 // 继电器命令字
#define BEEP	0x10 // 蜂鸣器命令字
#define LED		0x20 // 照明灯命令字
//#define SEG		0x30 // 数码管命令字

#define RELY_OFF			0x00
#define RELY_ON				0x01


#define BEEP_OFF			0x00
#define BEEP_ON				0x01
#define BEEP_AUTO_OFF	0x02
#define BEEP_AUTO_ON	0x03

#define LED_OFF		0x00
#define LED_ON		0x01

#define SEG_MIN		0x00
#define SEG_MAX		0x09
#define SEG_OFF		0x0f

#define ENDDEVICE_SEND_MSG_TIMEOUT   300*12     // Every 1  seconds

void FanCtrl(uint8_t cmd);
void BeepCtrl(uint8_t cmd);
void LedCtrl(uint8_t cmd);
void SyncData(unsigned char  sto_no)  ;
/*********************************************************************/
// Send Message Timeout
#define GENERICAPP_SEND_MSG_TIMEOUT   1500     // Every 5 seconds


// Application Events (OSAL) - These are bit weighted definitions.
#define GENERICAPP_SEND_MSG_EVT       0x0001

#if defined( IAR_ARMCM3_LM )
#define GENERICAPP_RTOS_MSG_EVT       0x0002
#endif  

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the Generic Application
 */
extern void GenericApp_Init( byte task_id );
/*
 * Task Event Processor for the Generic Application
 */
extern UINT16 GenericApp_ProcessEvent( byte task_id, UINT16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GENERICAPP_H */
