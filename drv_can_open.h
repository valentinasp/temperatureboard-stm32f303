
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_CAN_OPEN_H
#define __DRV_CAN_OPEN_H

#define  DEVICE_CAN_ID  5


#ifdef __cplusplus
 extern "C" {
#endif


// irenginio aparaturine versija
#ifndef HARDWARE_VER 
    #define HARDWARE_VER    0x100
#endif
//irenginio softwaro versija
#ifndef SOFTWARE_VER
    #define SOFTWARE_VER    0x100
#endif


// skaitmeniniu iejimu bloku po 32 skaicius
#define DIGITAL_INPUT_BLOCKS    0

// skaitmeniniu isejimu bloku po 32 skaicius
#define DIGITAL_OUTPUT_BLOCKS   0

// analoginiu iejimu skaicius
#define ANALOG_INPUTS           18

// analoginiu isejimu skaicius
#define ANALOG_OUTPUTS          0

// kalibravimo blokai
#define SPECIAL_IO_BLOCKS       1
#define SPECIAL_IO_SUBBLOCK     18


// timeriu f-jos iskvietimo periodiskumas, ms
#define CAN_OPEN_TIMER_TICK_MS       10

// hyperterminalo CAN magistrale ijungimas
//#define ENABLE_REMOTE_HYPERTERMINAL




#define SYS_STATE_INIT              0x00
#define SYS_STATE_OPERATIONAL       0x10
#define SYS_STATE_STOPPED           0x20
#define SYS_STATE_ERROR             0x30


#define MAX_TABLE_RECORD  19+(DIGITAL_INPUT_BLOCKS*6)+(DIGITAL_OUTPUT_BLOCKS*3)+(ANALOG_INPUTS*6)+ANALOG_OUTPUTS+(SPECIAL_IO_BLOCKS*SPECIAL_IO_SUBBLOCK)


#ifdef __18CXX
    // Microchip MCU
    // iraso tipas - 2 baitai
    typedef unsigned short  RECORD_TYPE;
    #define MAX_RECORD_DIGIT    0xFFFF
#else
    // ARM MCU
    // iraso tipas -  baitai
    typedef unsigned long  RECORD_TYPE;
    #define MAX_RECORD_DIGIT    0xFFFFFFFF
//    #pragma anon_unions
#endif


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


#ifndef __18CXX
typedef struct
{
    union
    {
        unsigned int Id;	    // CAN Message ID (11-bit or 29-bit)
        struct
        {
            unsigned char byte0;
            unsigned char byte1;
            unsigned char byte2;
            unsigned char byte3;
        } id;    
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
#endif

extern unsigned short SystemState;
void Init_RecordTable(char digital_input, char digital_output, char analog_input, char analog_output);
void CanOpenProtocol(CAN_MESSAGE *msg);
void SystemStateChange(RECORD_TYPE val);
void CanOpenTimer(void);
void CanOpenSendStartStatus(void);


#if DIGITAL_INPUT_BLOCKS>0
    void DigitalInput(unsigned short bank, RECORD_TYPE input);
#endif
#if ANALOG_INPUTS>0
    void AnalogInput(unsigned short bank, RECORD_TYPE input);
    void AnalogUnformatedInput(unsigned short bank, RECORD_TYPE input);
#endif
#if DIGITAL_OUTPUT_BLOCKS>0
    void ClearOutputs(unsigned char bank, RECORD_TYPE val);
    void SetOutputs(unsigned char bank, RECORD_TYPE val);
#endif
#if SPECIAL_IO_BLOCKS > 0
    void SpecialIO_Output(unsigned short bank, unsigned short index,  RECORD_TYPE val);
#endif
    
#ifdef __cplusplus
}
#endif

#endif /*__DRV_CAN_OPEN_H */
