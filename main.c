#include "can_driver.h"

void EnableInterrupts(void);
void Test_Yomna(void);
void CAN0_Handler(void);

uint8_t dataT[8] = "Helloyom";
uint8_t dataR[8];
tCANReadRXData ReceivedData;
tCANReadRXData* PReceivedData = &ReceivedData;

int main(){

	ReceivedData.Msg_Data = dataR;
	Test_Yomna();
	
//	tCANConfigTXMsgObj msgT;
	tCANConfigRXMsgObj msgR;
	
	/*msgT.Msg_ID = 0x12;
	msgT.Msg_Length = EIGHT_BYTE;
	msgT.Msg_Data = dataT;
	msgT.Flags = 0;
	
	tCANConfigTXMsgObj* PMsgObjT = &msgT;
	*/
	
	msgR.Msg_ID = 0x12;
	msgR.Msg_ID_MSK = 0x0;
	msgR.Flags = MSG_OBJ_RX_INT_ENABLE;
	
	tCANConfigRXMsgObj* PMsgObjR = &msgR;
	
	//CANTransmitMessageSet(CAN0_BASE, MsgObj1, PMsgObjT);
	CANReceiveMessageSet(CAN0_BASE, MsgObj2, PMsgObjR);
	
		//BIT RATE :
		CAN0_CTL_R |= 0x40;									  //Write accesses to the CANBIT register are allowed
		CAN0_BIT_R = 0x1443;								  //  calculate BaudRate
	  CAN0_CTL_R &=~ 0x40;		
		CAN0_CTL_R &= ~(CAN_CTL_INIT);		// Clear INIT bit in CAN0_CTL Register to Leave initialization mode and operate normal
		
		//CAN0_CTL_R |= (1<<7);						  // The CAN Controller is in Test Mode
		//CAN0_TST_R |= (1<<4);							// Enable LoopBack Mode
	//	CAN_Write(CAN0_BASE, MsgObj1, PMsgObjT);
		
	while (1)
	{
		//CANMessageGet(CAN0_BASE, MsgObj2, PMsgObjR, FALSE);
		//if(ACCESS_REG(CAN0_BASE + CAN_NWDA1_R) & 0x2 ){
				
			//	CAN0_IF2CMSK_R = 0x007F;
				//CAN0_IF2CRQ_R = 0x02;
			
		//}
	}	

}


void Test_Yomna(void){

		SYSCTL_RCGC0_R |= (1<<24);  		// enable CAN module0 clock
		SYSCTL_RCGC2_R |= (1<<1);				// enable GPIO PortF Clock
	  GPIO_PORTB_LOCK_R |= 0x4c4f434b;	
    GPIO_PORTB_CR_R |= 0x30;	
	  GPIO_PORTB_DEN_R |= 0x30;	      // 
   	GPIO_PORTB_DIR_R |= 0x20;       //	output transmit PB5
	  GPIO_PORTB_DIR_R &=~ 0x10;      //  Input Rx PB4
	  GPIO_PORTB_CR_R |= 0x30;	
	  GPIO_PORTB_AMSEL_R &=~ 0x30;
		GPIO_PORTB_AFSEL_R |= (1<<4);		// enable alternate function for PortF Pin0
		GPIO_PORTB_AFSEL_R |= (1<<5); 	// enable alternate function for PortF Pin3
	  GPIO_PORTB_PCTL_R &=~ 0xFF0000;	
		GPIO_PORTB_PCTL_R |= 0x80000;				// Set PMC0 field for PB4
		GPIO_PORTB_PCTL_R |= 0x800000;		// Set PMC3 field for PB5
	
		NVIC_EN1_R |= (1<<7);					// enable CAN0 interrupts
	
		CAN0_CTL_R |= CAN_CTL_INIT;							 // Set INIT bit in CAN0_CTL Register to enter initialization mode
		CAN0_CTL_R |= (1<<1);										 // Enable interrupts
		CAN0_CTL_R |= (1<<2);										 // Enable interrupt when a message has been transmitted or received
	
}

void CAN0_Handler(void)
{
		if(CAN0_MSG1INT_R == 0x2){
			CAN0_IF2CMSK_R = 0x007F;
			CAN0_IF2CRQ_R = 0x02;						// Message number
			CANMessageGet(CAN0_BASE, MsgObj2, PReceivedData, FALSE );
		}
		
		CAN0_IF2MCTL_R &= ~(1<<13);

}
