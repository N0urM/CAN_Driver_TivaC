#include "can_driver.h"

void CANInit (uint32_t ui32Base,Port_Name Port_Base,uint8_t Mode){
	
	volatile uint32_t delay ;
	
		//GPIO_Initialization :
	
	if(ui32Base == CAN0_BASE){		
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

void CANTransmitMessageSet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *MsgObject){

		 tCANFRAME_TYPE Frame_Type = STANDARD_FRAME;

		// Check the message ID
		if(((MsgObject -> Msg_ID) > CAN_MAX_STANDARD_ID) && ((MsgObject -> Msg_ID) < CAN_MAX_EXTENDED_ID) && (MsgObject->Flags & MSG_OBJ_EXTENDED_ID)){
			
			 Frame_Type = EXTENDED_FRAME;
			
		} else if((MsgObject -> Msg_ID) > CAN_MAX_EXTENDED_ID){
			 return;    //UNValid Msg ID
		}
			
		// Check using mask or not
		if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
			SET_BIT((Base+CAN_IF1MCTL_R), CAN_IF1MCTL_UMASK);
			
			SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_MASK);			 // Set Mask bit
		}


		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_WRNRD);		 // Set WRNRD bit = 1
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_ARB);			 // Set ARB bit to enable access arbitration bits
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_CONTROL);	 // Set Control bit to enable access control bits
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_CLRINTPND); // Clear Interrupt Pending Bit
		//SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_TXRQST);		 // Access Transmision request
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_DATAA);		 // Access Data Bytes 0 to 3
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_DATAB);		 // Access Data Bytes 4 to 7

		/*******************STANDARD FRAME*******************/
		if(Frame_Type == STANDARD_FRAME){
			
			CLEAR_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_XTD);

			if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
			
				ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);						   // First clear Mask2 Field
				ACCESS_REG(Base + CAN_IF1MSK2_R) |= (MsgObject->Msg_ID_MSK); // Write the ID mask
				CLEAR_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);					 // Clear MXTD Bit
				SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MDIR);					   // The message direction DIR bit is used for acceptance filtering
			}
			
			ACCESS_REG(Base + CAN_IF1ARB2_R) &= ~(0x1FFF);								 // First clear ARB2 Field
			ACCESS_REG(Base + CAN_IF1ARB2_R) |= MsgObject->Msg_ID;				 // Write the message ID
			
		}
		/*******************EXTENDED FRAME*******************/
		 else if (Frame_Type == EXTENDED_FRAME){
			
			SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_XTD);
			
			if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
				
					ACCESS_REG(Base + CAN_IF1MSK1_R) &= ~(0xFFFF);						                // First clear Mask1 Field
					ACCESS_REG(Base + CAN_IF1MSK1_R) |= (MsgObject->Msg_ID_MSK & 0x0000FFFF);	// Write first part of ID_MSK
					
					ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);						                // First clear Mask2 Field
					ACCESS_REG(Base + CAN_IF1MSK2_R) |= ((MsgObject->Msg_ID_MSK & 0x1FFF0000) >> 16);	// Write Second part of ID_MSK
					SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MXTD);						              // Set MXTD Bit
					SET_BIT(Base + CAN_IF1MSK2_R, CAN_IF1MSK2_MDIR);					                // The message direction DIR bit is used for acceptance filtering
			}
			ACCESS_REG(Base + CAN_IF1ARB1_R) &= ~(0xFFFF);													      // First clear ARB1 Field
			ACCESS_REG(Base + CAN_IF1ARB1_R) |= (MsgObject->Msg_ID & 0x0000FFFF);		      // Write first part of ID
			
			ACCESS_REG(Base + CAN_IF1ARB2_R) &= ~(0x1FFF);													      // First clear ARB2 Field
			ACCESS_REG(Base + CAN_IF1ARB2_R) |= ((MsgObject->Msg_ID & 0x1FFF0000) >> 16);	// Write Second part of ID
			
		}
		 
			// Set Direction bit to indicate transmit message object
			SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_DIR);
			
			if(MsgObject->Flags & MSG_OBJ_TX_INT_ENABLE){
					SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_TXIE);				// enable interrupt on successful transmission
			}
			

		// Set message Valid bit
		SET_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_MSGVAL);
		// Mark this bit as end of buffer because no use of FIFOs in transmit mode
		SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_EOB);
			

		ACCESS_REG((Base + CAN_IF1MCTL_R)) &= ~(0xF);			            //First clear DLC Field
		ACCESS_REG((Base + CAN_IF1MCTL_R)) |= MsgObject->Msg_Length;	// Write the message length code

		// Data
		ACCESS_REG((Base + CAN_IF1DA1_R)) |= (MsgObject -> Msg_Data[0]);
		ACCESS_REG((Base + CAN_IF1DA1_R)) |= ((MsgObject -> Msg_Data[1]) << 8);

		ACCESS_REG((Base + CAN_IF1DA2_R)) |= (MsgObject -> Msg_Data[2]);
		ACCESS_REG((Base + CAN_IF1DA2_R)) |= ((MsgObject -> Msg_Data[3]) << 8);

		ACCESS_REG((Base + CAN_IF1DB1_R)) |= (MsgObject -> Msg_Data[4]);
		ACCESS_REG((Base + CAN_IF1DB1_R)) |= ((MsgObject -> Msg_Data[5]) << 8);

		ACCESS_REG((Base + CAN_IF1DB2_R)) |= (MsgObject -> Msg_Data[6]);
		ACCESS_REG((Base + CAN_IF1DB2_R)) |= ((MsgObject -> Msg_Data[7]) << 8);

		ACCESS_REG((Base + CAN_IF1CRQ_R)) = ObjID;				// Write Message number into MNUM field to start transfer

		// Request transmission of this message object
		SET_BIT((Base + CAN_IF1MCTL_R), CAN_IF1MCTL_TXRQST);
	
}

/*********************************************************************************************************/

void CANReceiveMessageSet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *MsgObject){
	
		tCANFRAME_TYPE Frame_Type = STANDARD_FRAME;
		
		// Check the message ID
		if(((MsgObject -> Msg_ID) > CAN_MAX_STANDARD_ID) && ((MsgObject -> Msg_ID) < CAN_MAX_EXTENDED_ID) && (MsgObject->Flags & MSG_OBJ_EXTENDED_ID)){
			
			 Frame_Type = EXTENDED_FRAME;
			
		} else if((MsgObject -> Msg_ID) > CAN_MAX_EXTENDED_ID){
			 return;    //UNValid Msg ID
		}
		
		// Check using mask or not
		if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
			
			SET_BIT((Base+CAN_IF2MCTL_R), CAN_IF2MCTL_UMASK);
			SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_MASK);			 // Set Mask bit
		}
		
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_WRNRD);		 // Set WRNRD bit = 1
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_ARB);			 // Set ARB bit to enable access arbitration bits
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_CONTROL);	 // Set Control bit to enable access control bits
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_CLRINTPND); // Clear Interrupt Pending Bit
		//SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_TXRQST);		 // Access Transmision request
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_DATAA);		 // Access Data Bytes 0 to 3
		SET_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_DATAB);		 // Access Data Bytes 4 to 7
		
		/*******************STANDARD FRAME*******************/
		if(Frame_Type == STANDARD_FRAME){
			
			CLEAR_BIT((Base + CAN_IF2ARB2_R), CAN_IF2ARB2_XTD);

			if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
			
				ACCESS_REG(Base + CAN_IF2MSK2_R) &= ~(0x1FFF);						   // First clear Mask2 Field
				ACCESS_REG(Base + CAN_IF2MSK2_R) |= ((MsgObject->Msg_ID_MSK) << 2); // Write the ID mask
				CLEAR_BIT(Base + CAN_IF2MSK2_R, CAN_IF2MSK2_MXTD);					 // Clear MXTD Bit
				SET_BIT(Base + CAN_IF2MSK2_R, CAN_IF2MSK2_MDIR);					   // The message direction DIR bit is used for acceptance filtering
			}
			
			ACCESS_REG(Base + CAN_IF2ARB2_R) &= ~(0x1FFF);								  // First clear ARB2 Field
			ACCESS_REG(Base + CAN_IF2ARB2_R) |= ((MsgObject->Msg_ID) << 2);	// Write the message ID
			
		}
		/*******************EXTENDED FRAME*******************/
		 else if (Frame_Type == EXTENDED_FRAME){
			
			SET_BIT((Base + CAN_IF2ARB2_R), CAN_IF2ARB2_XTD);
			
			if(MsgObject->Flags & MSG_OBJ_USE_ID_FILTER){
				
					ACCESS_REG(Base + CAN_IF2MSK1_R) &= ~(0xFFFF);						                // First clear Mask1 Field
					ACCESS_REG(Base + CAN_IF2MSK1_R) |= (MsgObject->Msg_ID_MSK & 0x0000FFFF);	// Write first part of ID_MSK
					
					ACCESS_REG(Base + CAN_IF2MSK2_R) &= ~(0x1FFF);						                // First clear Mask2 Field
					ACCESS_REG(Base + CAN_IF2MSK2_R) |= ((MsgObject->Msg_ID_MSK & 0x1FFF0000) >> 16);	// Write Second part of ID_MSK
					SET_BIT(Base + CAN_IF2MSK2_R, CAN_IF2MSK2_MXTD);						              // Set MXTD Bit
					SET_BIT(Base + CAN_IF2MSK2_R, CAN_IF2MSK2_MDIR);					                // The message direction DIR bit is used for acceptance filtering
			}
			ACCESS_REG(Base + CAN_IF2ARB1_R) &= ~(0xFFFF);													      // First clear ARB1 Field
			ACCESS_REG(Base + CAN_IF2ARB1_R) |= (MsgObject->Msg_ID & 0x0000FFFF);		      // Write first part of ID
			
			ACCESS_REG(Base + CAN_IF2ARB2_R) &= ~(0x1FFF);													      // First clear ARB2 Field
			ACCESS_REG(Base + CAN_IF2ARB2_R) |= ((MsgObject->Msg_ID & 0x1FFF0000) >> 16);	// Write Second part of ID
			
		}
		 
			// Clear Direction bit to indicate Receive message object
			CLEAR_BIT((Base + CAN_IF2ARB2_R), CAN_IF2ARB2_DIR);
			
			if(MsgObject->Flags & MSG_OBJ_RX_INT_ENABLE){
					SET_BIT((Base + CAN_IF2MCTL_R), CAN_IF2MCTL_RXIE);		// enable interrupt on successful reception
			}
			
			
		// Set message Valid bit
		SET_BIT((Base + CAN_IF2ARB2_R), CAN_IF2ARB2_MSGVAL);
		
		// Check using FIFO or not
		if(MsgObject->Flags & MSG_OBJ_FIFO){
			CLEAR_BIT((Base + CAN_IF2MCTL_R), CAN_IF2MCTL_EOB);
		} else{
			SET_BIT((Base + CAN_IF2MCTL_R), CAN_IF2MCTL_EOB);
		}
		
		ACCESS_REG((Base + CAN_IF2MCTL_R)) &= ~(0xF);			            //First clear DLC Field
		ACCESS_REG((Base + CAN_IF2MCTL_R)) |= MsgObject->Msg_Length;	// Write the message length "DLC field"
		ACCESS_REG((Base + CAN_IF2CRQ_R)) = ObjID;										// Write Message number into MNUM field to start transfer
	
}

/*********************************************************************************************************/

void CANMessageGet (CAN_Base Base, MsgObjID ObjID, tCANMsgObject *psMsgObject, bool_t bClrPendingInt){

		CLEAR_BIT((Base + CAN_IF2CMSK_R), CAN_IF2CMSK_WRNRD);		// clear WRNRD bit = 0
		ACCESS_REG((Base + CAN_IF2CRQ_R)) = ObjID;							// Set the MNUM field to the Message object ID
	
		// Reading data and store it into the message objsct data field
		psMsgObject->Msg_Data[0] = (ACCESS_REG((Base + CAN_IF2DA1_R)) & 0x000000FF);
		psMsgObject->Msg_Data[1] = ((ACCESS_REG((Base + CAN_IF2DA1_R)) & 0x0000FF00) >> 8);
	
		psMsgObject->Msg_Data[2] = (ACCESS_REG((Base + CAN_IF2DA2_R)) & 0x000000FF);
		psMsgObject->Msg_Data[3] = ((ACCESS_REG((Base + CAN_IF2DA2_R)) & 0x0000FF00) >> 8);
	
		psMsgObject->Msg_Data[4] = (ACCESS_REG((Base + CAN_IF2DB1_R)) & 0x000000FF);
		psMsgObject->Msg_Data[5] = ((ACCESS_REG((Base + CAN_IF2DB1_R)) & 0x0000FF00) >> 8);
	
		psMsgObject->Msg_Data[6] = (ACCESS_REG((Base + CAN_IF2DB2_R)) & 0x000000FF);
		psMsgObject->Msg_Data[7] = ((ACCESS_REG((Base + CAN_IF2DB2_R)) & 0x0000FF00) >> 8);
	
		// Filling the psMsgObject struct
		if(BIT_IS_SET((Base + CAN_IF2ARB2_R), CAN_IF2ARB2_XTD) ){ // Extended ID
			
				psMsgObject->Msg_ID = ACCESS_REG((Base + CAN_IF2ARB1_R));
				psMsgObject->Msg_ID |= ((ACCESS_REG((Base + CAN_IF2ARB2_R)) & 0x1FFF) << 16);
			
		} else {		// Standard ID
				psMsgObject->Msg_ID = ((ACCESS_REG((Base + CAN_IF2ARB2_R)) & 0x1FFC) >> 2);
		}
		
		if(BIT_IS_SET((Base + CAN_IF2MSK2_R), CAN_IF2MSK2_MXTD) ){ // Extended Mask ID
				
				psMsgObject->Msg_ID_MSK = ACCESS_REG((Base + CAN_IF2MSK1_R));
				psMsgObject->Msg_ID_MSK |= ((ACCESS_REG((Base + CAN_IF2MSK2_R)) & 0x1FFF) << 16);
		
		} else {		// Standara Mask ID
				psMsgObject->Msg_ID_MSK = ((ACCESS_REG((Base + CAN_IF2MSK2_R)) & 0x1FFC) >> 2);
		}
		
		// Message Length
		psMsgObject->Msg_Length = (ACCESS_REG((Base + CAN_IF2MCTL_R)) & 0xF);
		
		//Flags
		psMsgObject->Flags = (ACCESS_REG((Base + CAN_IF2MCTL_R)) & MSG_OBJ_NEW_DATA) | (ACCESS_REG((Base + CAN_IF2MCTL_R)) & MSG_OBJ_DATA_LOST);
		
}
	
/*********************************************************************************************************/

void CANMessageClear(CAN_Base Base, uint32_t ObjID){

		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_WRNRD);		  // Set WRNRD bit = 1
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_ARB);			  // Set ARB bit to enable access arbitration bits
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_CONTROL);	  // Set Control bit to enable access control bits
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_CLRINTPND); // Clear Interrupt Pending Bit
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_DATAA);		  // Access Data Bytes 0 to 3
		SET_BIT((Base + CAN_IF1CMSK_R), CAN_IF1CMSK_DATAB);		  // Access Data Bytes 4 to 7
	
		
		ACCESS_REG(Base + CAN_IF1MSK1_R) &= ~(0xFFFF);					// Clear Mask1 Field	
		ACCESS_REG(Base + CAN_IF1MSK2_R) &= ~(0x1FFF);					// Clear Mask2 Field
		ACCESS_REG(Base + CAN_IF1ARB1_R) &= ~(0xFFFF);					// Clear ARB1 Field
		ACCESS_REG(Base + CAN_IF1ARB2_R) &= ~(0x1FFF);					// Clear ARB2 Field
	
		ACCESS_REG((Base + CAN_IF1MCTL_R)) &= ~(0xF);			  		// Clear DLC Field
		
		ACCESS_REG((Base + CAN_IF1DA1_R)) = 0;									// Clear Data Registers
		ACCESS_REG((Base + CAN_IF1DA2_R)) = 0;
		ACCESS_REG((Base + CAN_IF1DB1_R)) = 0;
		ACCESS_REG((Base + CAN_IF1DB2_R)) = 0;
	
		CLEAR_BIT((Base + CAN_IF1ARB2_R), CAN_IF1ARB2_MSGVAL);	// Clear message Valid bit
		ACCESS_REG((Base + CAN_IF1CRQ_R)) = ObjID;
		
}

/*********************************************************************************************************/

void CANRetrySet (CAN_Base Base, bool_t bAutoRetry){
		
		if(bAutoRetry == TRUE){
				CLEAR_BIT((Base + CAN_CTL_R), CAN_CTL_DAR);		// Enable Retransmission mode
		} else if(bAutoRetry == FALSE){
				SET_BIT((Base + CAN_CTL_R), CAN_CTL_DAR);			// Disable Retransmission mode
		}
			
}

/*********************************************************************************************************/

bool_t CANRetryGet (CAN_Base Base){
	
		bool_t Return_Val = BIT_IS_CLEAR((Base + CAN_CTL_R), CAN_CTL_DAR);
		return Return_Val;
	
}

/*********************************************************************************************************/

uint32_t CANStatusGet (CAN_Base Base, tCANStsReg eStatusReg){
		
		uint32_t Return_Val;
		if(eStatusReg == CAN_STS_CONTROL){
			
				Return_Val = (ACCESS_REG(Base + CAN_STS_R) & 0xFF);
			
		} else if(eStatusReg == CAN_STS_TXREQUEST){
			
				Return_Val = (ACCESS_REG(Base + CAN_TXRQ1_R) & 0xFFFF) | ((ACCESS_REG(Base + CAN_TXRQ2_R) & 0xFFFF) << 16);
			
		} else if(eStatusReg == CAN_STS_NEWDAT){
			
				Return_Val = (ACCESS_REG(Base + CAN_NWDA1_R) & 0xFFFF) | ((ACCESS_REG(Base + CAN_NWDA2_R) & 0xFFFF) << 16);
			
		} else if(eStatusReg == CAN_STS_MSGVAL){
			
				Return_Val = (ACCESS_REG(Base + CAN_MSG1VAL_R) & 0xFFFF) | ((ACCESS_REG(Base + CAN_MSG2VAL_R) & 0xFFFF) << 16);
			
		}
	
		return Return_Val;

}


