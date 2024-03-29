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
  PROVIDED AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

#include "GenericApp.h"
#include "DebugTrace.h"

#include"stdio.h"
#include"string.h"


#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "chinese.h"

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
  GENERICAPP_CLUSTERID
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

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void GenericApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg );
static void GenericApp_HandleKeys( byte shift, byte keys );
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
static void GenericApp_SendTheMessage( void );

#if defined( IAR_ARMCM3_LM )
static void GenericApp_ProcessRtosMessage( void );
#endif

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

static void rxCB(uint8 port,uint8 event)
{
	uint8 uartbuf[4] = {0};
	
	uint8 cmd;
	
	HalUARTRead(0,&cmd,1);
        
    if((cmd & STO_MASK) == (0 << 6)
	   ||(cmd & STO_MASK) == (1 << 6)
       ||(cmd & STO_MASK) == (2 << 6)
       ||  (cmd & STO_MASK) == (3 << 6)
       ) 
    {
		sprintf(uartbuf,"cmd:%x",cmd);
		
		HalLcdWriteString(uartbuf,HAL_LCD_LINE_4);

		if(AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
		               GENERICAPP_CLUSTERID,
		               1,
		               &cmd,
		               &GenericApp_TransID,  //0
		               AF_DISCV_ROUTE, AF_DEFAULT_RADIUS )==afStatus_SUCCESS)
		{
		}
    }

}
void clear_lcd(void);
void clear_lcd(void)
{
	BACK_COLOR=MAGENTA;
	POINT_COLOR=YELLOW;
	LCD_Clear(BACK_COLOR); 

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
  GenericApp_TransID = 0;/*è®¾å¤ç¶æè®¾å®ä¸º ZDO å±ä¸­å®ä¹çåå§åç¶æ?
åå§ååºç¨è®¾å¤çç½ç»ç±»åï¼è®¾å¤ç±»åçæ¹åé½è¦äº§çä¸ä¸ªäºä»¶âZDO_STATE_CHANGEï¼ä»å­?é¢çè§£ä¸º ZDO ç¶æåçäºæ¹åãæä»¥å¨è®¾å¤åå§åçæ¶åä¸å®è¦æå®åå§åä¸ºä»ä¹ç¶æé½æ²?æãé£ä¹å®å°±è¦å»æ£æµæ´ä¸ªç¯å¢ï¼çæ¯å¦è½éæ°å»ºç«æèå å¥å­å¨çç½ç»ãä½æ¯æä¸ç§æå?ä¾å¤ï¼å°±æ¯å½ NV_RESTORE è¢«è®¾ç½®çåï¼NV_RESTORE æ¯æä¿¡æ¯ä¿å­å¨éæå¤±å­å¨å¨ä¸­ï¼ï¼é£ä¹
å½è®¾å¤æ­çµæèæç§æå¤éå¯æ¶ï¼ç±äºç½ç»ç¶æå­å¨å¨éæå¤±å­å¨å¨ä¸­ï¼é£ä¹æ­¤æ¶å°±åªéè¦?æ¢å¤å¶ç½ç»ç¶æï¼èä¸éè¦éæ°å»ºç«æèå å¥ç½ç»äº.**è¿ééè¦è®¾ç½?NV_RESTORE å®å®ä¹?*ã?*/
  
  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().
  //è®¾ç½®åéæ°æ®çæ¹å¼åç®çå°åå¯»åæ¨¡å¼ 
  //åéæ¨¡å¼ä¸ºå¹¿æ­
  GenericApp_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  GenericApp_DstAddr.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_DstAddr.addr.shortAddr = 0xFFFF;
  
  /*
  halUARTCfg_t uartConfig;
  uartConfig.configured = TRUE;
  uartConfig.baudRate = HAL_UART_BR_115200;
  uartConfig.flowControl = FALSE;
  HalUARTOpen (0,&uartConfig);
  */
  halUARTCfg_t uartConfig; 
  uartConfig.configured = TRUE; 
  uartConfig.baudRate = HAL_UART_BR_115200; 
  uartConfig.flowControl = FALSE; 
  uartConfig.flowControlThreshold = 1; 
  uartConfig.rx.maxBufSize = 255; 
  uartConfig.tx.maxBufSize = 255; 
  uartConfig.idleTimeout = 1; 
  uartConfig.intEnable = TRUE; 
  uartConfig.callBackFunc = rxCB; 
  HalUARTOpen (HAL_UART_PORT_0, &uartConfig);

  // Fill out the endpoint description.
   //å®ä¹æ¬è®¾å¤ç¨æ¥éä¿¡ç?APS å±ç«¯ç¹æè¿°ç¬¦  
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID; //ä»»å¡æè¿°ç¬¦çä»»å¡ ID
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;//å»¶æ¶ç­ç¥

  // Register the endpoint description with the AF
  /*å?AF å±ç»è®°æè¿°ç¬¦, ç»è®° endpoint description å?AF,è¦å¯¹è¯¥åºç¨è¿è¡åå§åå¹¶å¨ AF
è¿è¡ç»è®°ï¼åè¯åºç¨å±æè¿ä¹ä¸ä¸?EP å·²ç»å¼éå¯ä»¥ä½¿ç¨ï¼é£ä¹ä¸å±è¦æ¯æå³äºè¯¥åºç¨çä¿¡
æ¯æèåºç¨è¦å¯¹ä¸å±ååªäºæä½ï¼å°±èªå¨å¾å°ä¸å±çéå?*/
  afRegister( &GenericApp_epDesc );

  // Register for all key events - This app will handle all key events
   // ç»è®°ææçæé®äºä»¶
  RegisterForKeys( GenericApp_TaskID );
  HalLcdInit();

  clear_lcd();
  // Update the display
#if defined ( LCD_SUPPORTED )
  HalLcdWriteString( "GenericApp", HAL_LCD_LINE_1 );
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
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  afDataConfirm_t *afDataConfirm;

  // Data Confirmation message fields
  byte sentEP;
  ZStatus_t sentStatus;
  byte sentTransID;       // This should match the value sent
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )//æ¥æ¶ç³»ç»æ¶æ¯åè¿è¡å¤æ?
  {
  //æ¥æ¶å±äºæ¬åºç¨ä»»å?GenericAppçæ¶æ¯ï¼ä»?GenericApp_TaskIDæ è®°
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_CB_MSG:
          GenericApp_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;
		// å½æä¸å¯é¥æ¶æ¶å°
        case KEY_CHANGE://æé®äºä»¶
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

        case AF_INCOMING_MSG_CMD://æ¥æ¶æ°æ®äºä»¶,è°ç¨å½æ° AF_DataRequest()æ¥æ¶æ°æ®
          GenericApp_MessageMSGCB( MSGpkt );//è°ç¨åè°å½æ°å¯¹æ¶å°çæ°æ®è¿è¡å¤ç 
          break;// æ¯å½è®¾å¤å¨ç½ç»ä¸­æ´æ¹ç¶ææ¶æ¥æ¶

        case ZDO_STATE_CHANGE:
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (GenericApp_NwkState == DEV_ZB_COORD)
              || (GenericApp_NwkState == DEV_ROUTER)
              || (GenericApp_NwkState == DEV_END_DEVICE) )
          {
            // Start sending "the" message in a regular interval.
            //è¿ä¸ªå®æ¶å¨åªæ¯ä¸ºåéå¨æä¿¡æ¯å¼å¯çï¼è®¾å¤å¯å¨åå§ååä»è¿éå¼å§è§¦åç¬¬ä¸ä¸ªå¨æä¿¡æ¯çåéï¼ç¶åå¨èå¤å§ä¸å?          
            osal_start_timerEx( GenericApp_TaskID,
                                GENERICAPP_SEND_MSG_EVT,
                                GENERICAPP_SEND_MSG_TIMEOUT );
          }
          break;

        default:
          break;
      }

      // Release the memory
      // éæ¾åå­ //äºä»¶å¤çå®äºï¼éæ¾æ¶æ¯å ç¨çåå­
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next//æéæåä¸ä¸ä¸ªæ¾å¨ç¼å²åºçå¾å¤ççäºä»¶ï¼è¿å while ( MSGpkt )éæ°å¤çäºä»¶ï¼ç´å°ç¼å²åºæ²¡æç­å¾å¤çäºä»¶ä¸ºæ­¢
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events//è¿åæªå¤ççäºä»¶
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  // åéæ¶æ?æ­¤äºä»¶ç±è®¡æ¶å¨çæã?  if ( events & GENERICAPP_SEND_MSG_EVT )
  {
    // Send "the" message
    /*å¤çå¨ææ§äºä»¶ï¼å©ç¨ SampleApp_SendPeriodicMessage()å¤çå®å½åçå¨ææ§äºä»¶ï¼
    ç¶åå¯å¨å®æ¶å¨å¼å¯ä¸ä¸ä¸ªå¨ææ§äºæï¼è¿æ ·ä¸ç§å¾ªç¯ä¸å»ï¼ä¹å³æ¯ä¸é¢è¯´çå¨ææ§äºä»¶äº*/
    //GenericApp_SendTheMessage();

    // Setup to send message again
    // å¨æ­£å¸¸æ¶é´ååæ¬¡åéæ¶æ¯çè®¾ç½®
    osal_start_timerEx( GenericApp_TaskID,
                        GENERICAPP_SEND_MSG_EVT,
                        GENERICAPP_SEND_MSG_TIMEOUT );

    // return unprocessed eventsè¿åæªå¤ççäºä»¶
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
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
	switch ( pkt->clusterId )
	{
	 	case GENERICAPP_CLUSTERID1: 
	 	case GENERICAPP_CLUSTERID2:
	 	case GENERICAPP_CLUSTERID3:
	 	case GENERICAPP_CLUSTERID4:			
			HalUARTWrite(0, pkt->cmd.Data,24);		
			break;
	}
   
}

/*********************************************************************
 * @fn      GenericApp_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_SendTheMessage( void )
{
  /*char theMessageData[] = "Hello World";

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
  }*/
}

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
