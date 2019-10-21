#include "can_driver.h"

void EnableInterrupts(void);
void CAN0_Handler(void);
void Test_Yomna(void);
void CAN_Cnfg_TX_Msg(CAN_Base Base ,MsgObjID ObjID, uint32_t msg_ID, tCAN_MSG_LENGTH msg_Length, uint8_t *msg_Data, uint32_t flags);
void CAN_Cnfg_RX_Msg(CAN_Base Base , MsgObjID ObjID, uint32_t msg_ID, uint32_t msg_ID_MSK, uint32_t flags);

uint8_t dataT[8] = "Helloooo";
uint8_t dataR[8];
tCANReadRXData ReceivedData;
tCANReadRXData* PReceivedData = &ReceivedData;

int main(){
		
		ReceivedData.Msg_Data = dataR;
		uint32_t IntEnableFlags = CAN_INT_MASTER | CAN_INT_STATUS;
		
		GPIO_Init(Port_B);
		CANInit(CAN0_BASE, Real_Mode);
		CANIntEnable(CAN0_BASE, IntEnableFlags);
		
	
		//Configuration of TX Message Object1
		//CAN_Cnfg_TX_Msg(CAN0_BASE, MsgObj1, 0x12, EIGHT_BYTE, dataT, MSG_OBJ_TX_INT_ENABLE);
		
		//Configuration of RX Message Object2
		CAN_Cnfg_RX_Msg(CAN0_BASE, MsgObj2, 0x12, 0x0, 0);
		
	
		//BIT RATE :
		CAN0_CTL_R |= 0x40;									  //Write accesses to the CANBIT register are allowed
		CAN0_BIT_R = 0x1443;								  //  calculate BaudRate
	  CAN0_CTL_R &=~ 0x40;		
		
		CANEnable(CAN0_BASE);
		//CAN_Write(CAN0_BASE, MsgObj1, PMsgObjT);
		
	while (1)
	{
		
	}	

}

void CAN_Cnfg_TX_Msg(CAN_Base Base ,MsgObjID ObjID, uint32_t msg_ID, tCAN_MSG_LENGTH msg_Length, uint8_t *msg_Data, uint32_t flags){

		tCANConfigTXMsgObj msgT;
		tCANConfigTXMsgObj* PMsgObjT = &msgT;
		
		msgT.Msg_ID = msg_ID;
		msgT.Msg_Length = msg_Length;
		msgT.Msg_Data = msg_Data;
		msgT.Flags = flags;
		
		CANTransmitMessageSet(Base, ObjID, PMsgObjT);
	
}


void CAN_Cnfg_RX_Msg(CAN_Base Base , MsgObjID ObjID, uint32_t msg_ID, uint32_t msg_ID_MSK, uint32_t flags){

		tCANConfigRXMsgObj msgR;
		tCANConfigRXMsgObj* PMsgObjR = &msgR;
		
		msgR.Msg_ID = msg_ID;
		msgR.Msg_ID_MSK = msg_ID_MSK;
		msgR.Flags = flags;
		
		CANReceiveMessageSet(Base, ObjID, PMsgObjR);

}

void CAN0_Handler(void)
{
		if(CAN0_MSG1INT_R == 0x2){
				CAN0_IF2CMSK_R = 0x007F;
				CAN0_IF2CRQ_R = 0x02;						// Message number
				CANMessageGet(CAN0_BASE, MsgObj2, PReceivedData, TRUE);
				//CAN0_IF2MCTL_R &= ~(1<<13);
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

