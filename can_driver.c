#include "can_driver.h"

void CANMessageSet (uint32_t Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType){
		
	tCANFRAME_TYPE Frame_Type = STANDARD_FRAME;
	
	if((Base != CAN0_BASE) && (Base != CAN1_BASE)){
		return;			//UNValid Base Address
	}
	
	if(((MsgObject -> Msg_ID) > CAN_MAX_STANDARD_ID) && ((MsgObject -> Msg_ID) < CAN_MAX_EXTENDED_ID)){
		
		 Frame_Type = EXTENDED_FRAME;
		
	} else if((MsgObject -> Msg_ID) > CAN_MAX_EXTENDED_ID){
		 return;    //UNValid Msg ID
	}
		
	SET_BIT((Base+CAN_IF1MCTL_R), CAN_IF1MCTL_UMASK);
	
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_WRNRD);		 // Set WRNRD bit = 1
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_MASK);			 // Set Mask bit
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_ARB);			 // Set ARB bit to enable access arbitration bits
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_CONTROL);	 // Set Control bit to enable access control bits
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_CLRINTPND); // Clear Interrupt Pending Bit
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_TXRQST);		 // Access Transmision request
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_DATAA);		 // Access Data Bytes 0 to 3
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_DATAB);		 // Access Data Bytes 4 to 7
	
	if(Frame_Type == STANDARD_FRAME){
		#ifdef MSG_OBJ_USE_ID_FILTER
		
			ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);
			ACCESS_REG(Base + CAN_IF1MSK2_R) |= (MsgObject->Msg_ID_MSK);
			CLEAR_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);					// Clear MXTD Bit
		#else
			
		#endif
	} else if (Frame_Type == EXTENDED_FRAME){
		
		ACCESS_REG(Base + CAN_IF1MSK1_R) &= ~(0xFFFF);						// First clear Mask1 Field
		ACCESS_REG(Base + CAN_IF1MSK1_R) |= (MsgObject->Msg_ID_MSK & 0x0000FFFF);	// Write first part of ID_MSK
		
		ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);						// First clear Mask2 Field
		ACCESS_REG(Base + CAN_IF1MSK2_R) |= ((MsgObject->Msg_ID_MSK & 0x1FFF0000) >> 15);	// Write Second part of ID_MSK
		SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);						// Set MXTD Bit
	}
	
	SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MDIR);					// The message direction DIR bit is used for acceptance filtering
	
}