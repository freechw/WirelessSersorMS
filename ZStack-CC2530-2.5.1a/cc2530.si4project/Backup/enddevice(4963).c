/******************************************************************************
  Filename:       GenericApp.c
  Revised:        $Date: 2012-03-07 01:04:58 -0800 (Wed, 07 Mar 2012) $
  Revision:       $Revision: 29656 $

  Description:    Generic Application (no Profile).


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
  PROVIDED ìAS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
******************************************************************************/

/*********************************************************************
  This application isn't intended to do anything useful, it is
  intended to be a simple example of an application's structure.

  This application sends "Hello World" to another "Generic"
  application every 5 seconds.  The application will also
  receives "Hello World" packets.

  The "Hello World" messages are sent/received as MSG type message.

  This applications doesn't have a profile, so it handles everything
  directly - itself.

  Key control:
    SW1:
    SW2:  initiates end device binding
    SW3:
    SW4:  initiates a match description request
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include"dht11.h"
#include"beep.h"
#include "led.h"
#include"relay.h"
#include"Gas_head.h"
#include"Infrare_head.h"
#include"light.h"
#include"Temperature_head.h"
#include"stdio.h"
#include"string.h"
#include"relay.h"
#include "chinese.h"


#include "GenericApp.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"


/* RTOS */
#if defined( IAR_ARMCM3_LM )
#include "RTOS_App.h"
#endif  

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// This list should be filled with Application specific Cluster IDs.
const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID2
};

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              //  int Endpoint;
  GENERICAPP_PROFID,                //  uint16 AppProfId[2];
  GENERICAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  GENERICAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  GENERICAPP_FLAGS,                 //  int   AppFlags:4;
  GENERICAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)GenericApp_ClusterList,  //  byte *pAppInClusterList;
  GENERICAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)GenericApp_ClusterList   //  byte *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in GenericApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t GenericApp_epDesc;
extern uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
byte GenericApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // GenericApp_Init() is called.
devStates_t GenericApp_NwkState;


byte GenericApp_TransID;  // This is the unique message ID (counter)

afAddrType_t GenericApp_DstAddr;

uint8 ENDDEVICE_ID = 1;//±Ì æ∏√…Ë±∏Ω⁄µ„–Ú∫≈  £¨‘ˆº”∂‡∏ˆΩ⁄µ„÷ª–Ë“™–ﬁ∏ƒ∏√÷µº¥ø…

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void GenericApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg );
static void GenericApp_HandleKeys( byte shift, byte keys );
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
//static void GenericApp_SendTheMessage( void );
#if defined( IAR_ARMCM3_LM )
static void GenericApp_ProcessRtosMessage( void );
#endif

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void clear_lcd(void);
void clear_lcd(void)
{
	

	BACK_COLOR=MAGENTA;
	POINT_COLOR=YELLOW;
	LCD_Clear(BACK_COLOR); 


	return;
}
void show_title(void);

void show_title(void)
{
	char offset = 20;
	showhanzi16x16(0 + offset,30,0,wu,YELLOW,BACK_COLOR);
	showhanzi16x16(17+ offset,30,0,xian,YELLOW,BACK_COLOR);
	showhanzi16x16(34+ offset,30,0,chuan,YELLOW,BACK_COLOR);
   
    showhanzi16x16(51+ offset,30,0,gan,YELLOW,BACK_COLOR);
    showhanzi16x16(68+ offset,30,0,wang,YELLOW,BACK_COLOR);
	return;
}

/*********************************************************************
 * @fn      GenericApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void GenericApp_Init( uint8 task_id )
{
	GenericApp_TaskID = task_id;
	GenericApp_NwkState = DEV_INIT;
	GenericApp_TransID = 0;


	// Device hardware initialization can be added here or in main() (Zmain.c).
	// If the hardware is application specific - add it here.
	// If the hardware is other parts of the device add it in main().

	GenericApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
	GenericApp_DstAddr.endPoint = GENERICAPP_ENDPOINT;
	GenericApp_DstAddr.addr.shortAddr = 0x0000;

	// Fill out the endpoint description.
	GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
	GenericApp_epDesc.task_id = &GenericApp_TaskID;
	GenericApp_epDesc.simpleDesc
	        = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
	GenericApp_epDesc.latencyReq = noLatencyReqs;

	// Register the endpoint description with the AF
	afRegister( &GenericApp_epDesc );

	// Register for all key events - This app will handle all key events
	RegisterForKeys( GenericApp_TaskID );
	InitBeep();
	InitLed();
	InitRelay();
    	    
	Init_infrare();
	HalLcdInit();
	clear_lcd();
	// Update the display
#if defined ( LCD_SUPPORTED )
	//HalLcdWriteString( "GenericApp", HAL_LCD_LINE_1 );
#endif

	ZDO_RegisterForZDOMsg( GenericApp_TaskID, End_Device_Bind_rsp );
	ZDO_RegisterForZDOMsg( GenericApp_TaskID, Match_Desc_rsp );

#if defined( IAR_ARMCM3_LM )
	// Register this task with RTOS task initiator
	RTOS_RegisterApp( task_id, GENERICAPP_RTOS_MSG_EVT );
#endif
}

/*********************************************************************
 * @fn      GenericApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
#define HONGWAI P0_5 

struct _EnvMsg EnvMsg;
void SyncData(unsigned char  sto_no)  //ÊääÊâÄÊúâÈááÈõÜÁöÑÊï∞ÊçÆÂêåÊ≠•Âà∞ÁªìÊûÑ‰ΩìEnvMsg‰∏≠
{
	char buf[10]={0};

	memset(&EnvMsg,0,sizeof(EnvMsg));
	
	EnvMsg.head[0] = 's';
	EnvMsg.head[1] = 't';
	EnvMsg.head[2] = ':';
	EnvMsg.type = 'e'; 
	EnvMsg.ep_no = sto_no;
	
	readDHT11();

	EnvMsg.temp[0] = (unsigned char)ucharT_data_L;
	EnvMsg.temp[1] = (unsigned char)ucharT_data_H;

	EnvMsg.hum[0] = (unsigned char)ucharRH_data_L;  
	EnvMsg.hum[1] = (unsigned char)ucharRH_data_H;

	sprintf(buf,"temp:%d",EnvMsg.temp[1]);
	HalLcdWriteString(buf,HAL_LCD_LINE_5);	

	sprintf(buf,"hum:%d",EnvMsg.hum[1]);
	HalLcdWriteString(buf,HAL_LCD_LINE_6);	

	EnvMsg.lux =  (unsigned int)myApp_ReadLightLevel();

	sprintf(buf,"li:%d",EnvMsg.lux);

	HalLcdWriteString(buf,HAL_LCD_LINE_7);
	
	EnvMsg.hongwai = HONGWAI; 

	EnvMsg.gas =  (unsigned int)myApp_ReadGasLevel(); 
	sprintf(buf,"gas:%d",EnvMsg.gas);

	HalLcdWriteString(buf,HAL_LCD_LINE_8);
}
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
	afIncomingMSGPacket_t *MSGpkt;
	afDataConfirm_t *afDataConfirm;

	// Data Confirmation message fields
	byte sentEP;
	ZStatus_t sentStatus;
	byte sentTransID;       // This should match the value sent
	(void)task_id;  // Intentionally unreferenced parameter
	char sendbuf[24] = {0};
	
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_CB_MSG:
          GenericApp_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;

        case KEY_CHANGE:
          GenericApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break;

        case AF_DATA_CONFIRM_CMD:
          // This message is received as a confirmation of a data packet sent.
          // The status is of ZStatus_t type [defined in ZComDef.h]
          // The message fields are defined in AF.h
          afDataConfirm = (afDataConfirm_t *)MSGpkt;
          sentEP = afDataConfirm->endpoint;
          sentStatus = afDataConfirm->hdr.status;
          sentTransID = afDataConfirm->transID;
          (void)sentEP;
          (void)sentTransID;

          // Action taken when confirmation is received.
          if ( sentStatus != ZSuccess )
          {
            // The data wasn't delivered -- Do something
          }
          break;

        case AF_INCOMING_MSG_CMD:
          GenericApp_MessageMSGCB( MSGpkt );
          break;

        case ZDO_STATE_CHANGE:
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (GenericApp_NwkState == DEV_ZB_COORD)
              || (GenericApp_NwkState == DEV_ROUTER)
              || (GenericApp_NwkState == DEV_END_DEVICE) )
          {
            // Start sending "the" message in a regular interval.
            osal_start_timerEx( GenericApp_TaskID,
                                GENERICAPP_SEND_MSG_EVT,
                                GENERICAPP_SEND_MSG_TIMEOUT );
			show_title();
          }
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  //  (setup in GenericApp_Init()).
	if ( events & GENERICAPP_SEND_MSG_EVT )
	{ //ÂÆöÊó∂Âô®ÔºåÊØèÈöî1ÁßíÂèëÈÄÅ‰∏ÄÊ¨°‰º†ÊÑüÂô®Êï∞ÊçÆ‰ø°ÊÅØ
		SyncData(ENDDEVICE_ID);
			
		memcpy(sendbuf, &EnvMsg, 24);	
		AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
		           GENERICAPP_CLUSTERID1,
		           24, 
		           (uchar *)&EnvMsg,
		           &GenericApp_TransID, 
		           AF_DISCV_ROUTE, AF_DEFAULT_RADIUS );


		// Setup to send message again
		osal_start_timerEx( GenericApp_TaskID,
		            GENERICAPP_SEND_MSG_EVT,
		            ENDDEVICE_SEND_MSG_TIMEOUT );

		// return unprocessed events
		return (events ^ GENERICAPP_SEND_MSG_EVT);
	}

  
#if defined( IAR_ARMCM3_LM )
  // Receive a message from the RTOS queue
  if ( events & GENERICAPP_RTOS_MSG_EVT )
  {
    // Process message from RTOS queue
    GenericApp_ProcessRtosMessage();

    // return unprocessed events
    return (events ^ GENERICAPP_RTOS_MSG_EVT);
  }
#endif

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */

/*********************************************************************
 * @fn      GenericApp_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg )
{
  switch ( inMsg->clusterID )
  {
    case End_Device_Bind_rsp:
      if ( ZDO_ParseBindRsp( inMsg ) == ZSuccess )
      {
        // Light LED
        HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
      }
#if defined( BLINK_LEDS )
      else
      {
        // Flash LED to show failure
        HalLedSet ( HAL_LED_4, HAL_LED_MODE_FLASH );
      }
#endif
      break;

    case Match_Desc_rsp:
      {
        ZDO_ActiveEndpointRsp_t *pRsp = ZDO_ParseEPListRsp( inMsg );
        if ( pRsp )
        {
          if ( pRsp->status == ZSuccess && pRsp->cnt )
          {
            GenericApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
            GenericApp_DstAddr.addr.shortAddr = pRsp->nwkAddr;
            // Take the first endpoint, Can be changed to search through endpoints
            GenericApp_DstAddr.endPoint = pRsp->epList[0];

            // Light LED
            HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
          }
          osal_mem_free( pRsp );
        }
      }
      break;
  }
}

/*********************************************************************
 * @fn      GenericApp_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_4
 *                 HAL_KEY_SW_3
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void GenericApp_HandleKeys( uint8 shift, uint8 keys )
{
  zAddrType_t dstAddr;

  // Shift is used to make each button/switch dual purpose.
  if ( shift )
  {
    if ( keys & HAL_KEY_SW_1 )
    {
    }
    if ( keys & HAL_KEY_SW_2 )
    {
    }
    if ( keys & HAL_KEY_SW_3 )
    {
    }
    if ( keys & HAL_KEY_SW_4 )
    {
    }
  }
  else
  {
    if ( keys & HAL_KEY_SW_1 )
    {
      // Since SW1 isn't used for anything else in this application...
#if defined( SWITCH1_BIND )
      // we can use SW1 to simulate SW2 for devices that only have one switch,
      keys |= HAL_KEY_SW_2;
#elif defined( SWITCH1_MATCH )
      // or use SW1 to simulate SW4 for devices that only have one switch
      keys |= HAL_KEY_SW_4;
#endif
    }

    if ( keys & HAL_KEY_SW_2 )
    {
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );

      // Initiate an End Device Bind Request for the mandatory endpoint
      dstAddr.addrMode = Addr16Bit;
      dstAddr.addr.shortAddr = 0x0000; // Coordinator
      ZDP_EndDeviceBindReq( &dstAddr, NLME_GetShortAddr(),
                            GenericApp_epDesc.endPoint,
                            GENERICAPP_PROFID,
                            GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                            GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                            FALSE );
    }

    if ( keys & HAL_KEY_SW_3 )
    {
    }

    if ( keys & HAL_KEY_SW_4 )
    {
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );
      // Initiate a Match Description Request (Service Discovery)
      dstAddr.addrMode = AddrBroadcast;
      dstAddr.addr.shortAddr = NWK_BROADCAST_SHORTADDR;
      ZDP_MatchDescReq( &dstAddr, NWK_BROADCAST_SHORTADDR,
                        GENERICAPP_PROFID,
                        GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                        GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                        FALSE );
    }
  }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      GenericApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */


uint8_t DeviceStatus;

void BeepCtrl(uint8_t cmd)
{

	switch(cmd)
	{
		case BEEP_OFF:
			DeviceStatus &= ~0x02;
			BeepOff();
			//HalLcdWriteString("beep on",HAL_LCD_LINE_5);
			break;
		case BEEP_ON:
			DeviceStatus |= 0x02;
			BeepOn();
			//HalLcdWriteString("beep off",HAL_LCD_LINE_5);
			break;
		default:
			break;
	}
}
void LedCtrl(uint8_t cmd)
{
	switch(cmd)
	{
		case LED_OFF:
			DeviceStatus &= ~0x04;
			//HalLcdWriteString("led off",HAL_LCD_LINE_5);
			LedOff();
			break;
		case LED_ON:
			DeviceStatus |= 0x04;
			//HalLcdWriteString("led on",HAL_LCD_LINE_5);
			LedOn();  
			break;
		default:
			break;
	}
}
int cmdcount = 0;
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
	uint8_t cmd = 0;
	char buf[10];
	switch ( pkt->clusterId )
	{
		case GENERICAPP_CLUSTERID: //–≠µ˜∆˜∑¢¿¥ ˝æ›

           cmd = *(pkt->cmd.Data);		
			//œ‘ æµΩlcd∆¡ƒª
		
			if((cmd & STO_MASK) == (ENDDEVICE_ID << 6)) // ≤÷ø‚∫≈’˝»∑
			{
				sprintf(buf,"cmd:%x",cmd);
				
				HalUARTWrite(0, buf,strlen(buf));	
				HalLcdWriteString(buf,HAL_LCD_LINE_4);	
				switch(cmd & DEV_MASK) // ≈–∂œ…Ë±∏∫≈
				{
					case RELY: // ≈≈∑Á…»√¸¡Ó◊÷
							// ∑Á…»√¸¡Ó¥¶¿Ì∫Ø ˝
						RelyCtrl(cmd & CMD_MASK);
						//HalLcdWriteString("rely",HAL_LCD_LINE_5);
						break;
					case BEEP: // ∑‰√˘∆˜√¸¡Ó◊÷
							// ∑‰√˘∆˜√¸¡Ó¥¶¿Ì∫Ø ˝
						BeepCtrl(cmd & CMD_MASK);
						break;
					case LED: // ’’√˜µ∆√¸¡Ó◊÷
						// ’’√˜µ∆√¸¡Ó¥¶¿Ì∫Ø ˝
						LedCtrl(cmd & CMD_MASK);
						break;
					default :
						break;
				}
			}
			break;
	}
}

/*********************************************************************
 * @fn      GenericApp_Send_wenshidu_Message
 *
 * @brief   point to point.
 *
 * @param   none
 *
 * @return  none
 */

/*********************************************************************
 * @fn      GenericApp_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
/*
static void GenericApp_SendTheMessage( void )
{
  char theMessageData[] = "Hello World";

  if ( AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)&theMessageData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    // Successfully requested to be sent.
  }
  else
  {
    // Error occurred in request to send.
  }
}
*/
#if defined( IAR_ARMCM3_LM )
/*********************************************************************
 * @fn      GenericApp_ProcessRtosMessage
 *
 * @brief   Receive message from RTOS queue, send response back.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_ProcessRtosMessage( void )
{
  osalQueue_t inMsg;

  if ( osal_queue_receive( OsalQueue, &inMsg, 0 ) == pdPASS )
  {
    uint8 cmndId = inMsg.cmnd;
    uint32 counter = osal_build_uint32( inMsg.cbuf, 4 );

    switch ( cmndId )
    {
      case CMD_INCR:
        counter += 1;  /* Increment the incoming counter */
                       /* Intentionally fall through next case */

      case CMD_ECHO:
      {
        userQueue_t outMsg;

        outMsg.resp = RSP_CODE | cmndId;  /* Response ID */
        osal_buffer_uint32( outMsg.rbuf, counter );    /* Increment counter */
        osal_queue_send( UserQueue1, &outMsg, 0 );  /* Send back to UserTask */
        break;
      }
      
      default:
        break;  /* Ignore unknown command */    
    }
  }
}
#endif

/*********************************************************************
 */
