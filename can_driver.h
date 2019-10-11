#include "common.h"
#include "CAN_HW_TYPES.h"
#include "tm4c123gh6pm.h"

/*Const definitions*/

#define CAN_MAX_STANDARD_ID		0x7FF
#define	CAN_MAX_EXTENDED_ID		0x1FFFFFFF

#define CAN_INT_ERROR						1
#define CAN_INT_MASTER					2
#define CAN_INT_STATUS					3
#define CAN_STATUS_BUS_OFF			4
#define CAN_STATUS_EPASS				5
#define CAN_STATUS_EWARN				6
#define CAN_STATUS_LEC_ACK			7
#define CAN_STATUS_LEC_BIT0			8
#define CAN_STATUS_LEC_BIT1			9
#define CAN_STATUS_LEC_CRC			10
#define CAN_STATUS_LEC_FORM			11
#define CAN_STATUS_LEC_MASK			12
#define CAN_STATUS_LEC_MSK			13
#define CAN_STATUS_LEC_NONE			14
#define CAN_STATUS_LEC_STUFF		15
#define CAN_STATUS_RXOK					16
#define CAN_STATUS_TXOK					17
#define MSG_OBJ_DATA_LOST				18
#define MSG_OBJ_EXTENDED_ID			19
#define MSG_OBJ_FIFO						20
#define MSG_OBJ_NEW_DATA				21
#define MSG_OBJ_NO_FLAGS				22
#define MSG_OBJ_REMOTE_FRAME		23
#define MSG_OBJ_RX_INT_ENABLE		24
#define MSG_OBJ_STATUS_MASK			25
#define MSG_OBJ_TX_INT_ENABLE		26
#define MSG_OBJ_USE_DIR_FILTER	27
#define MSG_OBJ_USE_EXT_FILTER	28
#define MSG_OBJ_USE_ID_FILTER		29

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

typedef enum {
	STANDARD_FRAME = 0,
	EXTENDED_FRAME = 1
}tCANFRAME_TYPE;

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
void CANMessageClear (uint32_t Base, uint32_t ui32ObjID);
void CANMessageGet (uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject psMsgObject, bool_t bClrPendingInt);
void CANMessageSet (uint32_t Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType);
bool_t CANRetryGet (uint32_t ui32Base);
void CANRetrySet (uint32_t ui32Base, bool_t bAutoRetry);
uint32_t CANStatusGet (uint32_t ui32Base, tCANStsReg eStatusReg);

