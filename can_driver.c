#include "can_driver.h"

void CANIntEnable (uint32_t ui32Base, uint32_t ui32IntFlags){
    if(ui32Base == CAN0_CTL_R)
     {   
        CAN0_CTL_R |= ui32IntFlags;

     }
    else if (ui32Base == CAN1_CTL_R) {
        CAN1_CTL_R |= ui32IntFlags;
    }
    else {
        // Error wrong base
    }
    
}

void CANIntDisable (uint32_t Base, uint32_t ui32IntFlags){
    uint32 CAN_Base;
    if(Base == 0)
     {   
        CAN_Base = CAN0_BASE;

     }
    else if (ui32Base == CAN1_CTL_R) {
        CAN_Base = CAN1_BASE;
    }
    else {
        // Error wrong base
    }
    CAN1_CTL_R &= (~ui32IntFlags);
}

void CANIntClear (uint32_t ui32Base, uint32_t ui32IntClr){
    if(ui32Base == CAN0_CTL_R)
     {   
        CAN0_INT_R &= ~ui32IntClr;
     }
    else if (ui32Base == CAN1_CTL_R) {
        CAN1_INT_R &= ~ui32IntClr;
    }
    else {
        // Error wrong base
    }
}

uint32_t CANIntStatus (uint32_t ui32Base, tCANIntStsReg eIntStsReg){

    
}



void CANMessageSet (uint32_t Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType){
		
	if((Base != CAN0_BASE) && (Base != CAN1_BASE)){
		return;
	}
	
	
	
}
