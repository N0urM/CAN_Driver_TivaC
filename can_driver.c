#include "can_driver.h"

void CANMessageSet (uint32_t Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType){
		
	if((Base != CAN0_BASE) && (Base != CAN1_BASE)){
		return;
	}
	
	SET_BIT((Base+CAN_IF1MCTL_R), CAN_IF1MCTL_UMASK);
	
	
}