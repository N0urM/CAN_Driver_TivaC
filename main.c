#include "can_driver.h"

// TX --> PB5
// RX --> PB4

void EnableInterrupts(void);
void CAN0_Handler(void);
void Test_Nada(void);


uint8_t dataT[6] = 	{0x11, 0x22, 0x33, 0x44, 0x44, 0x44};
uint8_t dataT2[6] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88};

tCANReadRXData ReceivedData;
tCANReadRXData ReceivedData2;

int main(){
		
		// Initialization
		Test_Nada();
		//GPIO_Init(Port_B)
		CANInit(CAN0_BASE, Real_Mode);
	
		// Clock Initialization
		tCANBitClkParms BitTime = {5, 2, 2, 4};
		CANBitTimingSet(CAN0_BASE, &BitTime);		// 0x1443
		
		// Interrupt enable
		uint32_t IntEnableFlags = CAN_INT_MASTER | CAN_INT_STATUS;
		CANIntEnable(CAN0_BASE, IntEnableFlags);
		
	
		//Configuration of TX Message Object1
		tCANConfigTXMsgObj msgT = {0x8, 0, SIX_BYTE, dataT};
		CANTransmitMessageSet(CAN0_BASE, MsgObj1, &msgT);
		
		//Configuration of TX Message Object2
		tCANConfigTXMsgObj msgT2 = {0x4, 0, SIX_BYTE, dataT2};
		CANTransmitMessageSet(CAN0_BASE, MsgObj2, &msgT2);
		
		//Configuration of RX Message Object3
		tCANConfigRXMsgObj msgR = {0x2, 0x7FF, (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER)};
		CANReceiveMessageSet(CAN0_BASE, MsgObj3, &msgR);
		
		//Configuration of RX Message Object4
		tCANConfigRXMsgObj msgR2 = {0x1, 0x7FF, (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER)};
		CANReceiveMessageSet(CAN0_BASE, MsgObj4, &msgR2);
		

		CANEnable(CAN0_BASE);
		

		
	while (1)
	{
			CAN_Write(CAN0_BASE, MsgObj1, &msgT);
			for(int i = 0; i<100000000; i++){}
			CAN_Write(CAN0_BASE, MsgObj2, &msgT2);
			for(int i = 0; i<100000000; i++){}
	}	

}



void CAN0_Handler(void)
{
		if(BIT_IS_SET((CAN0_BASE + CAN_MSG1INT_R), 1 << (MsgObj3-1))){
				CAN0_IF2CMSK_R = 0x007F;
				CANMessageGet(CAN0_BASE, MsgObj3, &ReceivedData2, TRUE);
				CLEAR_BIT((CAN0_BASE + CAN_STS_R), CAN_STS_RXOK);
		}
		
		if(BIT_IS_SET((CAN0_BASE + CAN_MSG1INT_R), 1 << (MsgObj4-1))){
				CAN0_IF2CMSK_R = 0x007F;
				CANMessageGet(CAN0_BASE, MsgObj4, &ReceivedData, TRUE);
				CLEAR_BIT((CAN0_BASE + CAN_STS_R), CAN_STS_RXOK);
		}

		
		if(BIT_IS_SET((CAN0_BASE + CAN_MSG1INT_R), 1 << (MsgObj1-1))){
				CLEAR_BIT((CAN0_BASE + CAN_IF1MCTL_R), CAN_IF1MCTL_INTPND);
				CLEAR_BIT((CAN0_BASE + CAN_STS_R), CAN_STS_TXOK);
		}
		 if(BIT_IS_SET((CAN0_BASE + CAN_MSG1INT_R), 1 << (MsgObj2-1))){
				CLEAR_BIT((CAN0_BASE + CAN_IF1MCTL_R), CAN_IF1MCTL_INTPND);
				CLEAR_BIT((CAN0_BASE + CAN_STS_R), CAN_STS_TXOK);
		}
}


void Test_Nada(void){

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
	
}

