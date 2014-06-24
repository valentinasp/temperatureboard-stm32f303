// irenginio adresas CAN tinkle (1..254)
#define DEVICE_CAN_ID   1


// irenginio aparaturine versija
#define HARDWARE_VER    0x100
//irenginio softwaro versija
#define SOFTWARE_VER    0x100


// skaitmeniniu iejimu bloku po 32 skaicius
#define DIGITAL_INPUT_BLOCKS    0

// skaitmeniniu isejimu bloku po 32 skaicius
#define DIGITAL_OUTPUT_BLOCKS   0

// analoginiu iejimu skaicius
#define ANALOG_INPUTS           4

// analoginiu isejimu skaicius
#define ANALOG_OUTPUTS          0

// timeriu f-jos iskvietimo periodiskumas, ms
#define CAN_OPEN_TIMER_TICK_MS       10

// hyperterminalo CAN magistrale ijungimas
//#define ENABLE_REMOTE_HYPERTERMINAL




#define SYS_STATE_INIT              0x00
#define SYS_STATE_PREOPERATIONAL    0x10
#define SYS_STATE_OPERATIONAL       0x20
#define SYS_STATE_STOPPED           0x30
#define SYS_STATE_ERROR             0x40


#define MAX_TABLE_RECORD  18+(DIGITAL_INPUT_BLOCKS*6)+(DIGITAL_OUTPUT_BLOCKS*3)+(ANALOG_INPUTS*5)+ANALOG_OUTPUTS

// iraso tipas - 4 baitai
typedef unsigned long  RECORD_TYPE;

// iraso tipas - 2 baitai
//typedef unsigned short  RECORD_TYPE;

typedef struct Table_struct
{
	unsigned short  index;
	unsigned char   flags;
	RECORD_TYPE     val; 	
} TABLE_RECORD;


// CAN paketu tipai filtrui
#define CAN_CMD_READ_MSG    0x01
#define CAN_ANSW_READ_MSG   0x02
#define CAN_CMD_WRITE_MSG   0x03
#define CAN_ANSW_WRITE_MSG  0x04
#define CAN_HYPERTERM_MSG   0x05

//#pragma anon_unions

typedef struct
{
    union
    {
        unsigned int Id;	    // CAN Message ID (11-bit or 29-bit)
        struct
        {
            unsigned char id0;
            unsigned char id1;
            unsigned char id2;
            unsigned char id3;
        };    
    };
    union
    {
        unsigned int Frame;     // Bits 16..19: DLC - Data Length Counter
                                // Bit 30: Set if this is a RTR message
                                // Bit 31: Set if this is a 29-bit ID message
        struct
        {
            unsigned char AccFilter0;
            unsigned char AccFilter1;
            unsigned char len;
            unsigned char rtr_ff;
        };
    };
    union 
    {
        struct 
        {
            unsigned int DataA;
            unsigned int DataB;
        };
        char data[8];
    };
} CAN_MESSAGE;



void Init_RecordTable(void);
void CanOpenProtocol(CAN_MESSAGE *msg);
void SystemStateChange(RECORD_TYPE val);
void CanOpenTimer(void);
//void SendCanMsg(CAN_MESSAGE *msg);


#if DIGITAL_INPUT_BLOCKS>0
    void DigitalInput(unsigned short bank, RECORD_TYPE input);
#endif
#if ANALOG_INPUTS>0
    void AnalogInput(unsigned short bank, RECORD_TYPE input);
#endif
#if DIGITAL_OUTPUT_BLOCKS>0
    void ClearOutputs(unsigned char bank, RECORD_TYPE val);
    void SetOutputs(unsigned char bank, RECORD_TYPE val);
#endif

