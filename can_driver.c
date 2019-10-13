#include "can_driver.h"

void CANInit (uint32_t ui32Base,Port_Name Port_Base,uint8_t Mode){
	
	volatile uint32_t delay ;
	
		//GPIO_Initialization :
	
	if((ui32Base == CAN0_BASE)){		
		//Enable Clock :
		SET_BIT(SYSCTL_RCGC0_R , SYSCTL_RCGC0_CAN0 ) ;				 // Clock For CAN0	
		if(Port_Base == Port_B) SET_BIT(SYSCTL_RCGC2_R,GPIOB); // clock For Port B 
   	if(Port_Base == Port_E) SET_BIT(SYSCTL_RCGC2_R,GPIOE); // clock For Port E 
  	if(Port_Base == Port_F) SET_BIT(SYSCTL_RCGC2_R,GPIOF); // clock For Port F
	}else{
		SET_BIT(SYSCTL_RCGC0_R , SYSCTL_RCGC0_CAN1 ) ;  			 // Clock For CAN1
	  SET_BIT(SYSCTL_RCGC2_R , GPIOA); 											 // Clock For Port A 
	}
	
	delay = SYSCTL_RCGC2_R ;
	
	// Allow Changes to PF4-5 :
  if (Port_Base == Port_F ) SET_BIT((Port_Base+CR_R),BIT4);
  if (Port_Base == Port_F ) SET_BIT((Port_Base+CR_R),BIT5);
	
	//Disable Analog Mode :
	CLEAR_BIT ((Port_Base+AMSEL_R),BIT0);
	CLEAR_BIT ((Port_Base+AMSEL_R),BIT1);
	CLEAR_BIT ((Port_Base+AMSEL_R),BIT2);
	CLEAR_BIT ((Port_Base+AMSEL_R),BIT3);
	
	// PCTL ,DIR , AFSEL :
	if ( Port_Base == Port_A ){
		//Port Control
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PA0_CAN1RX); // PMC0 control CAN1RX
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PA1_CAN1TX); // PMC1 control CAN1TX
		//Direction Bit :
	CLEAR_BIT((Port_Base+DIR_R) ,	BIT0); // RX >> Input 
	SET_BIT ((Port_Base+DIR_R)  ,	BIT1); // TX >> Output
		//Alternate Function :
	SET_BIT ((Port_Base+AFSEL_R),	BIT0); // Enable Alternate Fun Pin0
	SET_BIT ((Port_Base+AFSEL_R),	BIT1); // Enable Alternate Fun Pin1
		
}else if ((Port_Base == Port_B) || (Port_Base == Port_E)){
	//Port Control :
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PB4_CAN0RX); // PMC4 control CAN0RX
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PB5_CAN0TX); // PMC5 control CAN0TX
	//Direction Bit :
	CLEAR_BIT((Port_Base+DIR_R) ,	BIT4); // RX >> Input 
	SET_BIT ((Port_Base+DIR_R)  ,	BIT5); // TX >> Output

	//Alternate Function :
	SET_BIT ((Port_Base+AFSEL_R),	BIT4); // Enable Alternate Fun Pin4
	SET_BIT ((Port_Base+AFSEL_R),	BIT5); // Enable Alternate Fun Pin5
		
}else { // Port_F
	//Port Control :
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PF0_CAN0RX); // PMC0 control CAN0RX
	SET_BIT ((Port_Base+PCTL_R) , GPIO_PCTL_PF3_CAN0TX);// PMC3 control CAN0TX
	//Direction Bit :
	CLEAR_BIT((Port_Base+DIR_R),	BIT0); // RX >> Input 
	SET_BIT ((Port_Base+DIR_R) ,	BIT3); // TX >> Output
			//Alternate Function 
	SET_BIT ((Port_Base+AFSEL_R),	BIT0); // Enable Alternate Fun Pin0
	SET_BIT ((Port_Base+AFSEL_R),	BIT3); // Enable Alternate Fun Pin3
}
	//CAN CONFIGURATION :
	SET_BIT((ui32Base+CAN_CTL_R) , CAN_CTL_INIT);

	while(CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY) ; //wait while CAN0 Module is BUSY,set Flag .
	
	// Check Mode :

	if(Mode == Test_Mode){
		SET_BIT((ui32Base+CAN_CTL_R) , CAN_CTL_TEST);    // Enable Test Mode	
		SET_BIT((ui32Base+CAN_TST_R) , CAN_TST_LBACK );  // Enable LoopBack	
	}else{
		CLEAR_BIT((ui32Base+CAN_CTL_R) , CAN_CTL_TEST);  // Disable Test Mode	
		CLEAR_BIT((ui32Base+CAN_CTL_R) , CAN_TST_LBACK); // Disable LoopBack	
	}
	
	//Out From Initialization Mode To Start Transmit :
	CLEAR_BIT((ui32Base+CAN_CTL_R) , CAN_CTL_INIT);
}

uint32_t CANBitRateSet (uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate){
	
	// uncompleted  
	uint32_t result;
		//Enable Access CANBIT
	SET_BIT((ui32Base+CAN_CTL_R) , CAN_CTL_CCE) ; 
	// calculate BaudRate :
	SET_BIT((ui32Base+CAN_BIT_R) , BIT0) ;
	SET_BIT((ui32Base+CAN_BIT_R) , BIT9) ;
	
	return result ;
}

void CANIntEnable (CAN_Base Base, uint32_t ui32IntFlags){

    SET_BIT((Base+ CAN_CTL_R) , ui32IntFlags);

}

/*********************************************************************************************************/

void CANIntDisable (CAN_Base Base, uint32_t ui32IntFlags){

    CLEAR_BIT( (Base + CAN_CTL_R) , ui32IntFlags);
    
}

/*********************************************************************************************************/

void CANIntClear (CAN_Base Base, uint32_t ui32IntClr){

    CLEAR_BIT ( (Base+CAN_INT_R) , ui32IntClr); 
}

/*********************************************************************************************************/

uint32_t CANIntStatus (CAN_Base Base, tCANIntStsReg eIntStsReg){
    uint32_t return_val;
	
    if (eIntStsReg == CAN_INT_STS_CAUSE){
        return_val = READ_REG( (Base + CAN_INT_R) );
    }else {
        return_val  =  READ_REG (Base+CAN_MSG1INT_R) & 0x00ff;  // not sure if it's 0x00ff or 0xff00 masking INTPND bits
        return_val |= (READ_REG(Base+CAN_MSG2INT_R) <<16 );
    }
    return return_val;
}
/*********************************************************************************************************/

void CANMessageSet (CAN_Base Base, uint32_t ObjID, tCANMsgObject *MsgObject, tMsgObjType MsgType){

	 tCANFRAME_TYPE Frame_Type = STANDARD_FRAME;
	
	if(((MsgObject -> Msg_ID) > CAN_MAX_STANDARD_ID) && ((MsgObject -> Msg_ID) < CAN_MAX_EXTENDED_ID) && (MsgObject->Flags & MSG_OBJ_EXTENDED_ID)){
		
		 Frame_Type = EXTENDED_FRAME;
		
	} else if((MsgObject -> Msg_ID) > CAN_MAX_EXTENDED_ID){
		 return;    //UNValid Msg ID
	}
		
	if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
		SET_BIT((Base+CAN_IF1MCTL_R), CAN_IF1MCTL_UMASK);
	}
	
	
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_WRNRD);		 // Set WRNRD bit = 1
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_MASK);			 // Set Mask bit
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_ARB);			 // Set ARB bit to enable access arbitration bits
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_CONTROL);	 // Set Control bit to enable access control bits
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_CLRINTPND); // Clear Interrupt Pending Bit
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_TXRQST);		 // Access Transmision request
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_DATAA);		 // Access Data Bytes 0 to 3
	SET_BIT((Base + CAN_CANIF1CMSK_R), CAN_IF1CMSK_DATAB);		 // Access Data Bytes 4 to 7
	
	/*******************STANDARD FRAME*******************/
	if(Frame_Type == STANDARD_FRAME){
		
		CLEAR_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_XTD);

		if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
		
			ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);
			ACCESS_REG(Base + CAN_IF1MSK2_R) |= (MsgObject->Msg_ID_MSK);
			CLEAR_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);					// Clear MXTD Bit
			SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MDIR);					// The message direction DIR bit is used for acceptance filtering
		}
		
		ACCESS_REG(Base + CAN_IF1ARB2_R) &= ~(0x1FFF);
		ACCESS_REG(Base + CAN_IF1ARB2_R) |= MsgObject->Msg_ID;
		
	}
	/*******************EXTENDED FRAME*******************/
	 else if (Frame_Type == EXTENDED_FRAME){
		
		SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_XTD);
		
		if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
			
				ACCESS_REG(Base + CAN_IF1MSK1_R) &= ~(0xFFFF);						// First clear Mask1 Field
				ACCESS_REG(Base + CAN_IF1MSK1_R) |= (MsgObject->Msg_ID_MSK & 0x0000FFFF);	// Write first part of ID_MSK
				
				ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);						// First clear Mask2 Field
				ACCESS_REG(Base + CAN_IF1MSK2_R) |= ((MsgObject->Msg_ID_MSK & 0x1FFF0000) >> 16);	// Write Second part of ID_MSK
				SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);						// Set MXTD Bit
				SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MDIR);					// The message direction DIR bit is used for acceptance filtering
		}
		ACCESS_REG(Base + CAN_IF1ARB1_R) &= ~(0xFFFF);													// First clear ARB1 Field
		ACCESS_REG(Base + CAN_IF1ARB1_R) |= (MsgObject->Msg_ID & 0x0000FFFF);		// Write first part of ID
		
		ACCESS_REG(Base + CAN_IF1ARB2_R) &= ~(0x1FFF);													// First clear ARB2 Field
		ACCESS_REG(Base + CAN_IF1ARB2_R) |= ((MsgObject->Msg_ID & 0x1FFF0000) >> 16);	// Write Second part of ID
		
	}
	if(MsgType == CAN_MSG_OBJ_TYPE_TX){
		SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_DIR);
		
		if(MsgObject->Flags & MSG_OBJ_TX_INT_ENABLE){
				SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_TXIE);
		}
		
	} else if(MsgType == CAN_MSG_OBJ_TYPE_RX){
		CLEAR_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_DIR);
		
		if(MsgObject->Flags & MSG_OBJ_RX_INT_ENABLE){
				SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_RXIE);
		}
	}
	
	SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_MSGVAL);
	
	if(MsgObject->Flags & MSG_OBJ_FIFO){
		CLEAR_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_EOB);
	} else{
		SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_EOB);
	}
	
	ACCESS_REG((Base + CAN_IF1MCTL_R)) &= ~(0xF);			//First clear DLC Field
	ACCESS_REG((Base + CAN_IF1MCTL_R)) |= MsgObject->Msg_Length;
	
	// Data
	ACCESS_REG((Base + CAN_IF1DA1_R)) |= (MsgObject -> Msg_Data[0]);
	ACCESS_REG((Base + CAN_IF1DA1_R)) |= ((MsgObject -> Msg_Data[1]) << 8);
	
	ACCESS_REG((Base + CAN_IF1DA2_R)) |= (MsgObject -> Msg_Data[2]);
	ACCESS_REG((Base + CAN_IF1DA2_R)) |= ((MsgObject -> Msg_Data[3]) << 8);
	
	ACCESS_REG((Base + CAN_IF1DB1_R)) |= (MsgObject -> Msg_Data[4]);
	ACCESS_REG((Base + CAN_IF1DB1_R)) |= ((MsgObject -> Msg_Data[5]) << 8);
	
	ACCESS_REG((Base + CAN_IF1DB2_R)) |= (MsgObject -> Msg_Data[6]);
	ACCESS_REG((Base + CAN_IF1DB2_R)) |= ((MsgObject -> Msg_Data[7]) << 8);
	
	ACCESS_REG((Base + CAN_IF1CRQ_R)) = ObjID;
	
	if(MsgType == CAN_MSG_OBJ_TYPE_TX){
			SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_TXRQST);
	}
	
}
