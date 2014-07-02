#include <stm32f30x.h> 
#include "drv_can_open.h"
#include "drv_stm32_can.h"

static unsigned int CAN_hw_wr(CAN_MESSAGE *msg);
static void CAN_hw_rd (CAN_MESSAGE *msg);


CAN_MESSAGE  Can1RxBuf[CAN1_RX_BUF_SIZE],Can1TxBuf[CAN1_TX_BUF_SIZE];
volatile unsigned int Can1_RxIn,Can1_RxOut,Can1_TxIn,Can1_TxOut,Can1Status;


// leisti trukius
void Init_CanIrq(void)
{
    Can1Status = Can1_RxIn = Can1_RxOut = Can1_TxIn = Can1_TxOut = 0;
    
    CAN1->IER = ((1 << 1) | (1 << 0));
    /* Enable CAN1 interrupts */
    NVIC_SetPriority (USB_HP_CAN1_TX_IRQn,  1);
    NVIC_SetPriority (USB_LP_CAN1_RX0_IRQn, 1);
    
    NVIC_EnableIRQ   (USB_HP_CAN1_TX_IRQn);
    NVIC_EnableIRQ   (USB_LP_CAN1_RX0_IRQn);
}


// tx irq
void USB_HP_CAN1_TX_IRQHandler (void) 
{
    if(Can1_TxIn != Can1_TxOut)
    {
        if (CAN1->TSR & (1 << 0)) // request completed mbx 0 
        {                     
            CAN1->TSR |= (1 << 0); // reset request complete mbx 0 

        
            if(CAN_hw_wr( &Can1TxBuf[Can1_TxOut]))
            {
                Can1_TxOut++;
                if(Can1_TxOut >= CAN1_TX_BUF_SIZE) Can1_TxOut = 0;
            }       
        } 
    }
    else    CAN1->IER &= ~(1 << 0);            // disable  TME interrupt
}

// rx irq
void USB_LP_CAN1_RX0_IRQHandler (void) 
{
     volatile unsigned int temp;
    
    if (CAN1->RF0R & 3) // message pending ?
    {                                 
        CAN_hw_rd( &Can1RxBuf[Can1_RxIn]);
        temp = Can1_RxIn + 1;
        if(temp >= CAN1_RX_BUF_SIZE) temp = 0;
        if(temp == Can1_RxOut) Can1Status |= CAN_RX_SOFT_OVERFLOW;
        else Can1_RxIn = temp;
        
        CAN1->RF0R |= (1 << 5);                 // Release FIFO 0 output mailbox 
    }
}

// nuskaityti is registru CAN paketa
static void CAN_hw_rd(CAN_MESSAGE *msg)  
{
    // Read identifier information                                         
    if ((CAN1->sFIFOMailBox[0].RIR & (1 << 2)) == 0) 
    {   //Standard ID   
        msg->Frame  = 0;
        msg->Id     = 0x000007FFUL & (CAN1->sFIFOMailBox[0].RIR >> 21);
    }  
    else  
    {   // Extended ID   
        msg->Frame  = 1UL<<31;
        msg->Id     = 0x1FFFFFFFUL & (CAN1->sFIFOMailBox[0].RIR >> 3);
    }

    // Read type information                                                   
    if ((CAN1->sFIFOMailBox[0].RIR & (1 << 1)) == 0) 
    {
        //msg->type =   DATA_FRAME;    // DATA   FRAME  
    }  
    else  
    {
        msg->Frame |= 1<<30;   // REMOTE FRAME  
    }

    // Read length (number of received bytes)                                  
    msg->len = 0xF & (unsigned char)CAN1->sFIFOMailBox[0].RDTR;    
    msg->DataA = CAN1->sFIFOMailBox[0].RDLR;
    msg->DataB = CAN1->sFIFOMailBox[0].RDHR;
}



// irasyti i registrus CAN paketa
static unsigned int CAN_hw_wr(CAN_MESSAGE *msg)  
{
    if((CAN1->TSR & CAN_TSR_TME0) == 0) return 0;
        
    // Reset TIR register                                                      
    CAN1->sTxMailBox[0].TIR  = 0; // reset TXRQ bit 

    // Setup the identifier information                                        
    if((msg->Frame & (1UL<<31)) == 0)  
    {   //    Standard ID 
        CAN1->sTxMailBox[0].TIR |= (msg->Id << 21);
    }  
    else  
    {   // Extended ID    
        CAN1->sTxMailBox[0].TIR |= (msg->Id <<  3) | (1 << 2);
    }

    // Setup type information                                                  
    if(msg->Frame & (1<<30))  
    {   // REMOTE FRAME   
        CAN1->sTxMailBox[0].TIR |= (1 << 1);
    }

    // Setup data bytes                                                     
    CAN1->sTxMailBox[0].TDLR = msg->DataA;
    CAN1->sTxMailBox[0].TDHR = msg->DataB;

    // Setup length              
    CAN1->sTxMailBox[0].TDTR &= ~0x0000000F;
    CAN1->sTxMailBox[0].TDTR |=  ((unsigned int)msg->len & 0x0000000F);

    //  transmit message                
    CAN1->sTxMailBox[0].TIR  |=  (1 << 0);   //   set TXRQ bit 

    return 1;
}



//============================================================
// siusti CAN paketa
//------------------------------------------------------------
void SendCanMsg(CAN_MESSAGE *msg )
{
    unsigned int temp;
    if( CAN_hw_wr(msg) == 0)
    {
        Can1TxBuf[Can1_TxIn] = *msg;
        temp = Can1_TxIn + 1;
        if(temp >= CAN1_TX_BUF_SIZE) temp = 0;
        if(temp == Can1_TxOut) Can1Status |= CAN_TX_SOFT_OVERFLOW;
        else Can1_TxIn = temp;
        CAN1->IER |= (1 << 0);  //  enable  TME interrupt 
    }
}


//============================================================
// paimti CAN paketa
//------------------------------------------------------------
unsigned int ReceiveCanMsg(CAN_MESSAGE *msg)
{  
    if(Can1_RxIn != Can1_RxOut)
    {
        *msg = Can1RxBuf[Can1_RxOut]; 
        Can1_RxOut++;
        if(Can1_RxOut >= CAN1_RX_BUF_SIZE) Can1_RxOut = 0;    
        return 1;

    }
    return 0;
}

