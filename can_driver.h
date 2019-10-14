#include "CAN_HW_TYPES.h"
#include "common.h"
#include "tm4c123gh6pm.h"

/*Const definitions*/

#define CAN_MAX_STANDARD_ID		0x7FF
#define	CAN_MAX_EXTENDED_ID		0x1FFFFFFF



#define CAN_INT_ERROR   				CAN_CTL_EIE
#define CAN_INT_MASTER  				CAN_CTL_IE
#define CAN_INT_STATUS  				CAN_CTL_SIE
#define CAN_STATUS_BUS_OFF			CAN_STS_BOFF
#define CAN_STATUS_EPASS				CAN_STS_EPASS
#define CAN_STATUS_EWARN				CAN_STS_EWARN
#define CAN_STATUS_LEC_ACK			CAN_STS_LEC_ACK
#define CAN_STATUS_LEC_BIT0			CAN_STS_LEC_BIT0
#define CAN_STATUS_LEC_BIT1			CAN_STS_LEC_BIT1
#define CAN_STATUS_LEC_CRC			CAN_STS_LEC_CRC
#define CAN_STATUS_LEC_FORM			CAN_STS_LEC_FORM
#define CAN_STATUS_LEC_MASK			CAN_STS_LEC_M
#define CAN_STATUS_LEC_MSK
#define CAN_STATUS_LEC_NONE			CAN_STS_LEC_NONE
#define CAN_STATUS_LEC_STUFF		CAN_STS_LEC_STUFF
#define CAN_STATUS_RXOK					CAN_STS_RXOK
#define CAN_STATUS_TXOK					CAN_STS_TXOK
#define MSG_OBJ_DATA_LOST				CAN_IF1MCTL_MSGLST
#define MSG_OBJ_EXTENDED_ID			CAN_IF1ARB2_XTD
#define MSG_OBJ_FIFO						1
#define MSG_OBJ_NEW_DATA				CAN_IF1MCTL_NEWDAT
#define MSG_OBJ_NO_FLAGS				0
#define MSG_OBJ_REMOTE_FRAME		CAN_IF1MCTL_RMTEN
#define MSG_OBJ_RX_INT_ENABLE		CAN_IF1MCTL_RXIE
#define MSG_OBJ_STATUS_MASK			
#define MSG_OBJ_TX_INT_ENABLE		CAN_IF1MCTL_TXIE
#define MSG_OBJ_USE_DIR_FILTER	CAN_IF1MSK2_MDIR
#define MSG_OBJ_USE_EXT_FILTER	CAN_IF1MSK2_MXTD
#define MSG_OBJ_USE_ID_FILTER		CAN_IF1MCTL_UMASK


/*
    Enumerations to contrust on demand:
*/


typedef enum {
	CAN_INT_STS_CAUSE,
	CAN_INT_STS_OBJECT
}tCANIntStsReg;

typedef enum  {
	CAN_STS_CONTROL = 0,		//the main controller status
	CAN_STS_TXREQUEST = 1,	//bit mask of objects pending transmission
	CAN_STS_NEWDAT = 2,			//bit mask of objects with new data
	CAN_STS_MSGVAL = 3,			//bit mask of objects with valid configuration
}tCANStsReg;

typedef enum {
	
	CAN_MSG_OBJ_TYPE_TX = 0,
	CAN_MSG_OBJ_TYPE_RX = 1
	
}tMsgObjType;

typedef enum {
	ZERO_BYTE = 0,
	ONE_BYTE = 1,
	TWO_BYTE = 2,
	THREE_BYTE = 3,
	FOUR_BYTE = 4,
	FIVE_BYTE = 5,
	SIX_BYTE = 6,
	SEVEN_BYTE = 7,
	EIGHT_BYTE = 8,
}tCAN_MSG_LENGTH;
        
typedef struct {
	//again
}tCANBitClkParms;

typedef struct {
	
	uint32_t Msg_ID;
	uint32_t Msg_ID_MSK;
	uint32_t Flags;
	tCAN_MSG_LENGTH Msg_Length;
	uint8_t *Msg_Data;		// Array
	
}tCANMsgObject;

typedef enum {
	STANDARD_FRAME = 0,
	EXTENDED_FRAME = 1
}tCANFRAME_TYPE;

typedef enum {
	MsgObj1 = 0x01,
	MsgObj2 = 0x02,
	MsgObj3 = 0x03,
	MsgObj4 = 0x04,
	MsgObj5 = 0x05,
	MsgObj6 = 0x06,
	MsgObj7 = 0x07,
	MsgObj8 = 0x08,
	MsgObj9 = 0x09,
	MsgObj10 = 0x0A,
	MsgObj11 = 0x0B,
	MsgObj12 = 0x0C,
	MsgObj13 = 0x0D,
	MsgObj14 = 0x0E,
	MsgObj15 = 0x0F,
	MsgObj16 = 0x10,
	MsgObj17 = 0x11,
	MsgObj18 = 0x12,
	MsgObj19 = 0x13,
	MsgObj20 = 0x14,
	MsgObj21 = 0x15,
	MsgObj22 = 0x16,
	MsgObj23 = 0x17,
	MsgObj24 = 0x18,
	MsgObj25 = 0x19,
	MsgObj26 = 0x1A,
	MsgObj27 = 0x1B,
	MsgObj28 = 0x1C,
	MsgObj29 = 0x1D,
	MsgObj30 = 0x1E,
	MsgObj31 = 0x1F,
	MsgObj32 = 0x20,
	
}MsgObjID;

/* Prototypes*/

uint32_t CANBitRateSet (uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate);
void CANBitTimingGet (uint32_t ui32Base, tCANBitClkParms *psClkParms);
void CANBitTimingSet (uint32_t ui32Base, tCANBitClkParms *psClkParms);
void CANDisable (uint32_t ui32Base);
void CANEnable (uint32_t ui32Base);
bool_t CANErrCntrGet (uint32_t ui32Base, uint32_t pui32RxCount, uint32_t *pui32TxCount);
void CANInit (uint32_t ui32Base);


void CANIntClear (CAN_Base ui32Base, uint32_t ui32IntClr);
void CANIntDisable (CAN_Base ui32Base, uint32_t ui32IntFlags);
void CANIntEnable (CAN_Base ui32Base, uint32_t ui32IntFlags);
void CANIntRegister (CAN_Base ui32Base, void (*pfnHandler)(void));
uint32_t CANIntStatus (CAN_Base ui32Base, tCANIntStsReg eIntStsReg);
void CANIntUnregister (CAN_Base ui32Base);


void CANTransmitMessageSet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *MsgObject);
void CANReceiveMessageSet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *MsgObject);
void CANMessageGet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *psMsgObject, bool_t bClrPendingInt);
void CANMessageClear(CAN_Base Base, uint32_t ObjID);
bool_t CANRetryGet (CAN_Base Base);
void CANRetrySet (CAN_Base Base, bool_t bAutoRetry);
uint32_t CANStatusGet (CAN_Base Base, tCANStsReg eStatusReg);

