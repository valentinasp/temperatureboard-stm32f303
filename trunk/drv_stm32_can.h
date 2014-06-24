#define CAN1_RX_BUF_SIZE    32
#define CAN1_TX_BUF_SIZE    32


#define CAN_RX_SOFT_OVERFLOW    0x0001
#define CAN_RX_HARD_OVERFLOW    0x0002
#define CAN_TX_SOFT_OVERFLOW    0x0004

void SendCanMsg(CAN_MESSAGE *msg );
unsigned int ReceiveCanMsg(CAN_MESSAGE *msg);
