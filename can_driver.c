#include "can_driver.h"

void CANIntEnable (CAN_Base Base, uint32_t ui32IntFlags){

    SET_BIT((Base+ CAN_CTL_R) , ui32IntFlags);

}

void CANIntDisable (CAN_Base Base, uint32_t ui32IntFlags){

    CLEAR_BIT( (Base + CAN_CTL_R) , ui32IntFlags);
    
}

void CANIntClear (CAN_Base Base, uint32_t ui32IntClr){

    CLEAR_BIT ( (Base+CAN_INT_R) , ui32IntClr); 
}

uint32_t CANIntStatus (CAN_Base Base, tCANIntStsReg eIntStsReg){
    uint32_t return_val;
    if (eIntStsReg == CAN_INT_STS_CAUSE){
        return_val = READ_REG( (Base + CAN_INT_R) );
    }else {
        return_val =  READ_REG (Base+CAN_MSG1INT_R) & 0x00ff;  // not sure if it's 0x00ff or 0xff00 masking INTPND bits
        return_val |= (READ_REG(Base+CAN_MSG2INT_R) <<16 );
    }
    return return_val;
}



void CANMessageSet (CAN_Base Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType){
	
	SET_BIT((Base+CAN_IF1MCTL_R), CAN_IF1MCTL_UMASK);
	
}
