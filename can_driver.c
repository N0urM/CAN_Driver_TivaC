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

void CANIntDisable (uint32_t ui32Base, uint32_t ui32IntFlags){
    if(ui32Base == CAN0_CTL_R)
     {   
        CAN0_CTL_R &= (~ui32IntFlags);

     }
    else if (ui32Base == CAN1_CTL_R) {
        CAN1_CTL_R &= (~ui32IntFlags);
    }
    else {
        // Error wrong base
    }
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


