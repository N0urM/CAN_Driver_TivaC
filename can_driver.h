#include "CAN_HW_TYPES.h"
#include "common.h"
#include "tm4c123gh6pm.h"

/*Const definitions*/

#define CAN_INT_ERROR  = CAN_CTL_EIE
#define CAN_INT_MASTER = CAN_CTL_IE
#define CAN_INT_STATUS = CAN_CTL_SIE
#define CAN_STATUS_BUS_OFF
#define CAN_STATUS_EPASS
#define CAN_STATUS_EWARN
#define CAN_STATUS_LEC_ACK
#define CAN_STATUS_LEC_BIT0
#define CAN_STATUS_LEC_BIT1
#define CAN_STATUS_LEC_CRC
#define CAN_STATUS_LEC_FORM
#define CAN_STATUS_LEC_MASK
#define CAN_STATUS_LEC_MSK
#define CAN_STATUS_LEC_NONE
#define CAN_STATUS_LEC_STUFF
#define CAN_STATUS_RXOK
#define CAN_STATUS_TXOK
#define MSG_OBJ_DATA_LOST
#define MSG_OBJ_EXTENDED_ID
#define MSG_OBJ_FIFO
#define MSG_OBJ_NEW_DATA
#define MSG_OBJ_NO_FLAGS
#define MSG_OBJ_REMOTE_FRAME
#define MSG_OBJ_RX_INT_ENABLE
#define MSG_OBJ_STATUS_MASK
#define MSG_OBJ_TX_INT_ENABLE
#define MSG_OBJ_USE_DIR_FILTER
#define MSG_OBJ_USE_EXT_FILTER
#define MSG_OBJ_USE_ID_FILTER

/*
    Enumerations to contrust on demand:
*/


typedef enum {
	CAN_INT_STS_CAUSE,
	CAN_INT_STS_OBJECT
}tCANIntStsReg;

typedef enum  {
	anotherthing
}tCANStsReg;

typedef enum {
	
	CAN_MSG_OBJ_TYPE_TX = 0,
	CAN_MSG_OBJ_TYPE_RX = 1
	
}tMsgObjType;
        
typedef struct {
	//again
}tCANBitClkParms;

typedef struct {
	
	uint32_t Msg_ID;
	uint32_t Msg_ID_MSK;
	uint32_t Flags;
	uint32_t Msg_Length;
	uint8_t *Msg_Data;
	
}tCANMsgObject;

/* Prototypes*/

uint32_t CANBitRateSet (uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate);
void CANBitTimingGet (uint32_t ui32Base, tCANBitClkParms psClkParms);
void CANBitTimingSet (uint32_t ui32Base, tCANBitClkParms psClkParms);
void CANDisable (uint32_t ui32Base);
void CANEnable (uint32_t ui32Base);
bool_t CANErrCntrGet (uint32_t ui32Base, uint32_t pui32RxCount, uint32_t pui32TxCount);
void CANInit (uint32_t ui32Base);

void CANIntClear (CAN_Base ui32Base, uint32_t ui32IntClr);
void CANIntDisable (CAN_Base ui32Base, uint32_t ui32IntFlags);
void CANIntEnable (CAN_Base ui32Base, uint32_t ui32IntFlags);
void CANIntRegister (CAN_Base ui32Base, void (pfnHandler)(void));
uint32_t CANIntStatus (CAN_Base ui32Base, tCANIntStsReg eIntStsReg);
void CANIntUnregister (CAN_Base ui32Base);
void CANMessageGet (uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject psMsgObject, bool_t bClrPendingInt);
void CANMessageSet (CAN_Base Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType);
bool_t CANRetryGet (uint32_t ui32Base);
void CANRetrySet (uint32_t ui32Base, bool_t bAutoRetry);
uint32_t CANStatusGet (uint32_t ui32Base, tCANStsReg eStatusReg);

