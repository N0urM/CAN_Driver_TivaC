#include "common.h"
#include "tm4c123gh6pm.h"

/*Const definitions*/

#define CAN_INT_ERROR  = CAN_CTL_EIE
#define CAN_INT_MASTER = CAN_CTL_IE
#define CAN_INT_STATUS = CAN_CTL_SIE
#define CAN_INT_INTID_STATUS = 0x8000
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
	something
}tCANIntStsReg;

typedef enum  {
	anotherthing
}tCANStsReg;

typedef enum {
	onemore
}tMsgObjType;
        
typedef enum {
	again
}tCANBitClkParms;

typedef enum {
	ok
}tCANMsgObject;

/* Prototypes*/

uint32_t CANBitRateSet (uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate);
void CANBitTimingGet (uint32_t ui32Base, tCANBitClkParms psClkParms);
void CANBitTimingSet (uint32_t ui32Base, tCANBitClkParms psClkParms);
void CANDisable (uint32_t ui32Base);
void CANEnable (uint32_t ui32Base);
bool_t CANErrCntrGet (uint32_t ui32Base, uint32_t pui32RxCount, uint32_t pui32TxCount);
void CANInit (uint32_t ui32Base);

void CANIntClear (uint32_t ui32Base, uint32_t ui32IntClr);
void CANIntDisable (uint32_t ui32Base, uint32_t ui32IntFlags);
void CANIntEnable (uint32_t ui32Base, uint32_t ui32IntFlags);
void CANIntRegister (uint32_t ui32Base, void (pfnHandler)(void));
uint32_t CANIntStatus (uint32_t ui32Base, tCANIntStsReg eIntStsReg);
void CANIntUnregister (uint32_t ui32Base);

void CANMessageClear (uint32_t ui32Base, uint32_t ui32ObjID);
void CANMessageGet (uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject psMsgObject, bool_t bClrPendingInt);
void CANMessageSet (uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject psMsgObject, tMsgObjType eMsgType);
bool_t CANRetryGet (uint32_t ui32Base);
void CANRetrySet (uint32_t ui32Base, bool_t bAutoRetry);
uint32_t CANStatusGet (uint32_t ui32Base, tCANStsReg eStatusReg);
