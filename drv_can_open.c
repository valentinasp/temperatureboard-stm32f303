#include "drv_can_open.h"
#include "drv_stm32_can.h"
#include "calibration.h"
#include "ADCmeasurement.h"
#include "i2c.h"
#include "delay.h"


#define FLAG_READ_ONLY   0x01
#define FLAG_WRITE_ONLY  0x02
#define FLAG_READ_WRITE  0x03




TABLE_RECORD TableRecord[MAX_TABLE_RECORD];

unsigned short SystemState;
volatile unsigned int HeartBeatTimer,GuardTimer;//,RecordHeartBeatTime,RecordGuardTime;


static void RegisterRecord(unsigned short adr, unsigned char f, RECORD_TYPE def_val, unsigned short *index);
static unsigned char TableRead(unsigned short adr, RECORD_TYPE *val);
static unsigned short TableWrite(unsigned short adr, RECORD_TYPE val, unsigned char flag);
static unsigned char ReadToCanMsg(unsigned short adr, CAN_MESSAGE *msg);
static void WriteToCanMsg(CAN_MESSAGE *msg);
#if DIGITAL_INPUT_BLOCKS>0
    static void SendDigitalInput(unsigned short bank);
#endif
#if ANALOG_INPUTS>0
    static void SendAnalogInput(unsigned short bank);
#endif
#if DIGITAL_OUTPUT_BLOCKS>0
    static void DigitalOutput(unsigned short bank, RECORD_TYPE val, unsigned char mode);
    static void UpdateDigitalOutputs(void);
#endif

//====================================================================================================
// irasu lenteles inicializacija
//----------------------------------------------------------------------------------------------------
void Init_RecordTable(char digital_input, char digital_output, char analog_input, char analog_output)
{
    unsigned short i,j,adr;
    #if SPECIAL_IO_BLOCKS > 0
        unsigned short k;
    #endif
    
    if(digital_input  > DIGITAL_INPUT_BLOCKS)  digital_input  = DIGITAL_INPUT_BLOCKS;
    if(digital_output > DIGITAL_OUTPUT_BLOCKS) digital_output = DIGITAL_OUTPUT_BLOCKS;
    if(analog_input   > ANALOG_INPUTS)         analog_input   = ANALOG_INPUTS;
    if(analog_output  > ANALOG_OUTPUTS)        analog_output  = ANALOG_OUTPUTS;
    
    i = 0;
    HeartBeatTimer = GuardTimer = 0;
    
    RegisterRecord(0x0000,FLAG_READ_ONLY,0x0300,&i);                 // protokolo ID
    RegisterRecord(0x0100,FLAG_READ_ONLY,0x0002,&i);                 // Prietaiso ID / serijinis numeris
    RegisterRecord(0x0101,FLAG_READ_ONLY,HARDWARE_VER,&i);           // prietaiso aparatines dalies versija
    RegisterRecord(0x0102,FLAG_READ_ONLY,SOFTWARE_VER,&i);           // prietaiso programines dalies versija
    RegisterRecord(0x0103,FLAG_READ_ONLY,(RECORD_TYPE)digital_input,&i);   // skaitmeniniu iejimu skaicius
    RegisterRecord(0x0104,FLAG_READ_ONLY,(RECORD_TYPE)digital_output,&i);  // skaitmeniniu isejimu skaicius
    RegisterRecord(0x0105,FLAG_READ_ONLY,(RECORD_TYPE)analog_input,&i);    // analoginiu iejimu skaicius
    RegisterRecord(0x0106,FLAG_READ_ONLY,(RECORD_TYPE)analog_output,&i);   // analoginiu isejimu skaicius
    
    RegisterRecord(0x0200,FLAG_READ_ONLY,0x0001,&i);// starto statusas
  
    RegisterRecord(0x0300,FLAG_READ_ONLY,0x0000,&i); // aparaturines klaidos kodas
    RegisterRecord(0x0301,FLAG_READ_ONLY,0x0000,&i); // rysio klaidos kodas
  
    RegisterRecord(0x0400,FLAG_READ_WRITE,0x0001,&i); // programos busena
  
    RegisterRecord(0x0500,FLAG_READ_WRITE,0x0000,&i); // programos statuso siuntimo periodiskumas
    RegisterRecord(0x0501,FLAG_READ_WRITE,0x0000,&i); // centrinio valdiklio informavimo apie save periodiskumas
    RegisterRecord(0x0502,FLAG_READ_WRITE,0x0000,&i); // serverio sekimo taimeri
    RegisterRecord(0x0503,FLAG_READ_ONLY, 0x0000,&i); // statuso taimeris

    RegisterRecord(0x0600,FLAG_WRITE_ONLY,0x0000,&i); // duomenu issaugojimas flash atmintyje
    
    #if DIGITAL_INPUT_BLOCKS>0
        for(j=0; j<DIGITAL_INPUT_BLOCKS; j++)
        {
            adr = (0x100*j)+0x1000;
            RegisterRecord(adr,FLAG_READ_ONLY,0x0000,&i);    // skaitmeniniai iejimai
            RegisterRecord(adr+1,FLAG_READ_WRITE,0x0000,&i); // kauke, bet koks pokytis
            RegisterRecord(adr+2,FLAG_READ_WRITE,0x0000,&i); // kauke, pokytis 0->1
            RegisterRecord(adr+3,FLAG_READ_WRITE,0x0000,&i); // kauke, pokytis 1->0
            RegisterRecord(adr+4,FLAG_READ_WRITE,0x0000,&i); // pranesimo siuntimo periodiskumas
            RegisterRecord(adr+5,FLAG_READ_ONLY, 0x0000,&i); // timeris
        }
    #endif
      
    #if DIGITAL_OUTPUT_BLOCKS>0
        for(j=0; j<DIGITAL_OUTPUT_BLOCKS; j++)
        {
            adr = (0x100*j)+0x2000;
            RegisterRecord(adr,FLAG_READ_WRITE,0x0000,&i);   // skaitmeniniai isejimai
            RegisterRecord(adr+1,FLAG_WRITE_ONLY,0x0000,&i); // tik ijungimas
            RegisterRecord(adr+2,FLAG_WRITE_ONLY,0x0000,&i); // tik isjungimas
        }
    #endif
    
    #if ANALOG_INPUTS>0
        for(j=0; j<ANALOG_INPUTS; j++)
        {
            adr = (0x100*j)+0x3000;
            RegisterRecord(adr,FLAG_READ_ONLY,   0x0000,&i); // analoginis iejimas
            RegisterRecord(adr+1,FLAG_READ_ONLY, 0x0000,&i); // analoginis iejimas
            RegisterRecord(adr+2,FLAG_READ_WRITE,0x0000,&i); // lygis, kuri pereinant didejancia kryptimi, siunciamas pranesimas
            RegisterRecord(adr+3,FLAG_READ_WRITE,0x0000,&i); // lygis, kuri pereinant mazejancia kryptimi, siunciamas pranesimas 
            RegisterRecord(adr+4,FLAG_READ_WRITE,0x0000,&i); // pranesimo siuntimo periodiskumas
            RegisterRecord(adr+5,FLAG_READ_ONLY, 0x0000,&i); // timeris
        }
    #endif
   
    #if ANALOG_OUTPUTS>0
        for(j=0; j<ANALOG_OUTPUTS; j++)
        {
            adr = (0x100*j)+0x4000;
            RegisterRecord(adr,FLAG_READ_WRITE,0x0000,&i);  // analoginis isejimas
        }
    #endif
        
    #if SPECIAL_IO_BLOCKS > 0
        for(j=0; j<SPECIAL_IO_BLOCKS; j++)
        {
           adr = (0x100*j)+0x5000;
           for(k=0; k<SPECIAL_IO_SUBBLOCK; k++) RegisterRecord(adr+k,FLAG_READ_WRITE,0x0000,&i); // spec. iejimas/isejimas
        }
    #endif
   
}


//=========================================================================================================
// uregistruoti irasa strukturoje
//---------------------------------------------------------------------------------------------------------
static void RegisterRecord(unsigned short adr, unsigned char f, RECORD_TYPE def_val, unsigned short *index)
{
    unsigned char i;
    
    i = *index;
    if(i < MAX_TABLE_RECORD)
    {   
        TableRecord[i].index = adr;
        TableRecord[i].flags = f;
        TableRecord[i].val = def_val;
    }
    (*index)++;
}


//=========================================================================================================
// skaitymas is irasu lenteles
//---------------------------------------------------------------------------------------------------------
static unsigned char TableRead(unsigned short adr, RECORD_TYPE *val)
{
    unsigned short i;
    for(i=0; i<MAX_TABLE_RECORD; i++)
    {
        if(TableRecord[i].index == adr)
        {
            if(!(TableRecord[i].flags & FLAG_READ_ONLY)) return 2; // klaida - tik rasymas   
            *val = TableRecord[i].val;
            return 0;   // OK
        }
    }
    return 1;   // klaida - nera tokio iraso
}


//========================================================================================================
// rasymas i irasu lenteles
//--------------------------------------------------------------------------------------------------------
static unsigned short TableWrite(unsigned short adr, RECORD_TYPE val, unsigned char flag)
{
    unsigned short i;
    for(i=0; i<MAX_TABLE_RECORD; i++)
    {
        if(TableRecord[i].index == adr)
        {
            if(!(TableRecord[i].flags & FLAG_WRITE_ONLY))
            {
                if(flag) TableRecord[i].val = val;
                else return 2;   // klaida - tik skaitymas
            }
            else 
            {
                TableRecord[i].val = val;
                //if(adr == 0x0500) RecordHeartBeatTime = val;   // statuso periodiskumo siuntimo inicializavimas
                //if(adr == 0x0501) RecordGuardTime = val;
                if(adr == 0x0502) GuardTimer = 0;                // centrinio valdiklio veikimo sekimas 
                if(adr == 0x0400)                                // SystemState?
                {
                    if(flag == 0) SystemStateChange(val);
                    GuardTimer = 0;
                }
               // if((adr == 0x1004))                             // periodinio iejimu siuntimo nustatymas
               // {
               //     DigInputTimer = 0;
               //     RecordDigInputTime = val;
                //}

                // pasizymeti apie skaitmeniniu isejimu atnaujinima   
                #if DIGITAL_OUTPUT_BLOCKS>0
                    if((adr & 0xFF00) == 0x2000) DigitalOutput(0, val, adr & 0xF);
                #endif
                #if DIGITAL_OUTPUT_BLOCKS>1
                    if((adr & 0xFF00) == 0x2100) DigitalOutput(1, val, adr & 0xF);
                #endif
                #if DIGITAL_OUTPUT_BLOCKS>2
                    if((adr & 0xFF00) == 0x2200) DigitalOutput(2, val, adr & 0xF);
                #endif
                #if DIGITAL_OUTPUT_BLOCKS>3
                    if((adr & 0xFF00) == 0x2300) DigitalOutput(3, val, adr & 0xF);
                #endif
                
                #if SPECIAL_IO_BLOCKS > 0
                    if((adr & 0xF000) == 0x5000) SpecialIO_Output((adr >> 16) & 0xF, adr & 0xFF, val);
                #endif
                
                // pasizymeti apie analoginiu isejimu isejimu atnaujinima
                // if(i == Index_AnalogOutput1) TableRecordStatus |= A_O_UPDATED;
            }
            return 0;  //OK
        }
    }
    return 1; // nera tokio adreso
}


//==============================================
// paketo paskirties nustatymas
//----------------------------------------------
void CanOpenProtocol(CAN_MESSAGE *msg)
{
    unsigned short adr;
    switch(msg->id1)
    {
        case 0x01: //skaitymas
            if((msg->len & 0x0F) >= 2)
            {
                GuardTimer = 0;
                adr = ((unsigned short)msg->data[0] << 8) | (unsigned short)msg->data[1];
                ReadToCanMsg(adr,msg);
                SendCanMsg(msg);
            }
            break;
      
        case 0x02: //atsakymas i skaitymo komanda
            if(msg->id0 == DEVICE_CAN_ID)
            {
                // tinkle yra antras irenginys su tuo paciu ID
                TableWrite(0x0300, 0x0001, 1);
                ReadToCanMsg(0x0300,msg);
                SendCanMsg(msg);
                
                // pereiti i klaidos busena
                SystemStateChange(4);
            }
            break;
      
        case 0x03: //rasyti
            if((msg->len & 0x0F) >= 3)
            {   
                GuardTimer = 0;
                
                if((msg->data[0] == 0x05) && (msg->data[1] == 0x02)) 
                {
                    WriteToCanMsg(msg);
                }
                else
                {
                    WriteToCanMsg(msg);
                    SendCanMsg(msg);
                } 
            }
            break;
      
        case 0x04: //atsakymas i rasymo komanda
            if(msg->id0 == DEVICE_CAN_ID)
            {
                // tinkle yra antras irenginys su tuo paciu ID
                TableWrite(0x0300, 0x0001, 1);
                ReadToCanMsg(0x0300,msg);
                SendCanMsg(msg);   
                SystemStateChange(4);   // pereiti i klaidos busena
            }
            break;
      
        case 0x05: // nuotolinis hyperterminalas
            #ifdef ENABLE_REMOTE_HYPERTERMINAL
            if(msg->id0 == DEVICE_CAN_ID)
            {
                if(WorkSettings & BIT_REMOTE_HYPERTERM)
                {
                    if((msg->data[0] == REMOTE_CLOSE_REQ))
                    {							
                        //msg->data[0] = REMOTE_CLOSE_ACK;
                        //msg->len = 1;
                        //AddCanTxBuffer(msg);
                        //WorkSettings &= ~(BIT_REMOTE_HYPERTERM | BIT_REDIRECT_TO_CAN);
						//print(0,"\n\rRemote hyperterm close"); 
                        #ifdef USE_OS
						    HYPERTERM_CLOSE_EVT 
						#endif
                    }
                    else 
                    {
                        ExtendedKey = (WORD)msg->data[1] << 8;
                        ExtendedKey |= (WORD)msg->data[2];   
                        #ifdef USE_OS                 
                            HYPERTERM_KEY_EVT
                        #endif
                    }
                }
                else
                {
                    // rysio uzmezgimas       
                    if(msg->data[0] == REMOTE_INIT_REQ) 
                    { 
                        msg->data[0] = REMOTE_INIT_ACK;
                        msg->len = 1;
                        AddCanTxBuffer(msg);	
                        WorkSettings |= BIT_REMOTE_HYPERTERM | BIT_REDIRECT_TO_CAN;
                        print_const_string("\n\rRemote hyperterm init");
                        //CanTermTimer = 0;
                        #ifdef USE_OS
                            HYPERTERM_INIT_EVT
                        #endif
                    }
                }
            }
            #endif
            break;
      
        default:     //neatpazintas paketas
            TableWrite(0x0301, 0x0304, 1);
            ReadToCanMsg(0x0301,msg);
            SendCanMsg(msg);
    }
}


//=======================================================
// skaityti irasa is lenteles ir sufomuoti CAN atsakyma
//-------------------------------------------------------
static unsigned char ReadToCanMsg(unsigned short adr, CAN_MESSAGE *msg)
{
    RECORD_TYPE temp;
    unsigned char res;
  
    res = TableRead(adr,&temp);
    msg->id1     = 0x02;
    msg->id0     = DEVICE_CAN_ID;
    msg->len     = 7;
    msg->data[0] = (unsigned char)(adr >> 8);
    msg->data[1] = (unsigned char)adr;
    msg->data[2] = 0;
    msg->data[3] = 0;
    msg->data[4] = 0;
    msg->data[5] = 0;
    msg->data[6] = 0;
    msg->data[7] = 0;

    switch(res)
    {
        case 0: // viskas gerai
            //TableWrite(0x0C, 0x0000, TRUE);
            msg->data[2] = 0;
            msg->data[3] = 0;
            msg->data[4] = 0;
            if(sizeof(RECORD_TYPE) == 4)
            {
                msg->data[3] = (unsigned char)(temp >> 24);
                msg->data[4] = (unsigned char)(temp >> 16);
            }
            msg->data[5] = (unsigned char)(temp >> 8);
            msg->data[6] = (unsigned char)temp;
            break;
        case 1:  // irasas lenteleje neegzistuoja
            TableWrite(0x0301, 0x0001, 1);
            msg->data[2] = 1;
            break;
        case 2:  // iraso negalima skaityti
            TableWrite(0x0301, 0x0003, 1);
            msg->data[2] = 3;
            break;
        default: // neatpazinta komanda: vidine klaida
            TableWrite(0x0301, 0x0104, 1);
            msg->data[2] = 4;
    }  
    return res;
}


//=======================================================
// rasyti i lentele ir sufomuoti CAN atsakyma
//-------------------------------------------------------
static void WriteToCanMsg(CAN_MESSAGE *msg)
{
    RECORD_TYPE val=0;
    unsigned long temp = 0;
    unsigned char res;
 
    if(msg->len >= 3) temp = (RECORD_TYPE)msg->data[2];
    if(msg->len >= 4) temp = (temp << 8) | (unsigned long)msg->data[3];
    if(msg->len >= 5) temp = (temp << 8) | (unsigned long)msg->data[4];
    if(msg->len >= 6) temp = (temp << 8) | (unsigned long)msg->data[5];

    val = (RECORD_TYPE)temp;
    temp = ((unsigned long)msg->data[0] << 8) | (unsigned long)msg->data[1];
    res = TableWrite((unsigned short)temp, val, 0);

    msg->id1 = 0x04;
    msg->id0 = DEVICE_CAN_ID;
    msg->len = 4;

    switch(res)
    {
        case 0: // viskas gerai
            msg->data[2] = 0;
            msg->data[3] = 0;
            break;
        case 1: // irasas neegzistuoja
            TableWrite(0x0301, 0x0001, 1);
            msg->data[2] = 0;
            msg->data[3] = 1;
            break;
        case 2: // iraso negalima modifikuoti
            TableWrite(0x0301, 0x0002, 1);
            msg->data[2] = 0;
            msg->data[3] = 2;
            break;
        default: // vidine klaida
            TableWrite(0x0301, 0x0204, 1);
            msg->data[2] = 0x02;
            msg->data[3] = 0x04;
    }  
}


//===================================================================================
// issiusti skaitmeniniu iejimu busenas
//-----------------------------------------------------------------------------------
#if DIGITAL_INPUT_BLOCKS>0
static void SendDigitalInput(unsigned short bank)
{       
    CAN_MESSAGE msg;
    unsigned short adr;
    adr = (bank*0x100) + 0x1000 ;
    if(!ReadToCanMsg(adr, &msg)) SendCanMsg(&msg);
}
#endif


//===================================================================================
// issiusti analoginiu iejimu busenas
//-----------------------------------------------------------------------------------
#if ANALOG_INPUTS>0
static void SendAnalogInput(unsigned short bank)
{
    CAN_MESSAGE msg;
    unsigned short adr;
    adr = (bank*0x100) + 0x3000 ;
    if(!ReadToCanMsg(adr, &msg)) SendCanMsg(&msg);
}
#endif


//===================================================================================
// ivesti skaitmeninius iejimus
//-----------------------------------------------------------------------------------
#if DIGITAL_INPUT_BLOCKS>0
void DigitalInput(unsigned short bank, RECORD_TYPE input)
{  
    RECORD_TYPE temp,pokytis,old;
    unsigned short adr;
    unsigned char flag=0;
    
    adr = (bank*0x100) + 0x1000 ;
    if(TableRead(adr, &old) != 0) return; // nuskaityti sena verte     
    temp = old ^ input; // pokytis
    
    // siusti pranesima, esant bet kokiam pokyciui?
    if(TableRead(adr+1, &old) != 0) return; 
    if(temp & old) flag = 1;

    // siusti pranesima, kai yra pokytis 0->1?
    pokytis = temp & input;
    if(TableRead(adr+2, &old) != 0) return; 
    if(pokytis & old) flag = 1;
      
    // siusti pranesima, kai yra pokytis 1->0?
    if(TableRead(adr, &old) != 0) return;
    pokytis = temp & old;
    if(TableRead(adr+3, &old) != 0) return;
    if(pokytis & old) flag = 1;

    TableWrite(adr, input, 1); // issaugom verte

    if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL)
    {
        if(flag) SendDigitalInput(bank);
    }
}
#endif


//===================================================================================
// ivesti skaitmeninius isejimus
//-----------------------------------------------------------------------------------
#if DIGITAL_OUTPUT_BLOCKS>0
static void DigitalOutput(unsigned short bank, RECORD_TYPE val, unsigned char mode)
{    
    unsigned short adr;    
    RECORD_TYPE temp;
    
    adr = (bank*0x100) + 0x2000 ;
    if(mode == 0) // nustatomi visi bitai
    { 
        TableWrite(adr, val, 1); // issaugom verte
        if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL)
        {   
            ClearOutputs(bank, 0xFF);
            SetOutputs(bank, val); 
        }
    }
    if(mode == 1) // tik ijungiami bitai
    {
        if(TableRead(adr, &temp) != 0) return; 
        TableWrite(adr, val | temp, 1);         // issaugom verte 
        if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL) SetOutputs(bank, val);
    }
    if(mode == 2) // tik isjungiami bitai
    {
        if(TableRead(adr, &temp) != 0) return; 
        TableWrite(adr, temp & (~val) , 1); 
        if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL) ClearOutputs(bank, val);
    }
}
#endif


//===================================================================================
// nustatyti visus skaimeninius isejimus
//-----------------------------------------------------------------------------------
#if DIGITAL_OUTPUT_BLOCKS>0
static void UpdateDigitalOutputs(void)
{
    RECORD_TYPE temp;
    if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL)
    {
        #if DIGITAL_OUTPUT_BLOCKS>0
            ClearOutputs(0,0xFF);
            if(TableRead(0x2000, &temp) != 0) return; 
            SetOutputs(0, temp);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>1
            ClearOutputs(1,0xFF);
            if(TableRead(0x2100, &temp) != 0) return; 
            SetOutputs(1, temp);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>2
            ClearOutputs(2,0xFF);
            if(TableRead(0x2200, &temp) != 0) return; 
            SetOutputs(2, temp);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>3
            ClearOutputs(3,0xFF);
            if(TableRead(0x2300, &temp) != 0) return; 
            SetOutputs(3, temp);
        #endif
    }
} 
#endif



#if ANALOG_INPUTS>0
//===================================================================================
// ivesti analoginio iejimo verte
//-----------------------------------------------------------------------------------
void AnalogInput(unsigned short bank, RECORD_TYPE input)
{   
    RECORD_TYPE old,temp;
    unsigned short adr;
    unsigned char flag = 0;
    
    adr = (bank*0x100) + 0x3000;
    if(TableRead(adr, &old) != 0) return;

    // tikrinamas pokytis didejancia kryptimi
    if(TableRead(adr+2, &temp) != 0) return;
    if(temp != 0)
    {
        if((old < temp) && (input > temp)) flag |= 1;
    }
        
    // tikrinamas pokytis mazejancia kryptimi
    if(TableRead(adr+3, &temp) != 0) return;    
    if(temp != 0)
    {
        if((old > temp) && (input < temp)) flag |= 1;
    }

    // tikrinamas ar siusti is karto
    if(TableRead(adr+4, &temp) != 0) return; 
    if(sizeof(RECORD_TYPE) == 2) 
    {
        if(temp == 0xFFFF) flag |= 1; 
    }
    else
    {
        if(temp == 0xFFFFFFFF) flag |= 1; 
    }
    
    TableWrite(adr, input, 1); // issaugom verte
    
    if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL)
    {
        if(flag) SendAnalogInput(bank);
    }
}

//===================================================================================
// ivesti analoginio iejimo verte, neformatuota ADC verte
//-----------------------------------------------------------------------------------
void AnalogUnformatedInput(unsigned short bank, RECORD_TYPE input)
{
    unsigned short adr;
    
    adr = (bank*0x100) + 0x3000;
    TableWrite(adr+1, input, 1); // issaugom verte
}
#endif


//===================================================================================
// periodiniai darbai
//-----------------------------------------------------------------------------------
void CanOpenTimer(void)
{
    RECORD_TYPE rec,timer;
    CAN_MESSAGE msg;
    unsigned short i,adr;
    
    if(TableRead(0x0502, &rec) == 0) TableWrite(0x0502, rec+CAN_OPEN_TIMER_TICK_MS, 1); 
    if(TableRead(0x0503, &rec) == 0) TableWrite(0x0503, rec+CAN_OPEN_TIMER_TICK_MS, 1); 
    
    #if ANALOG_INPUTS>0
        for(i=0; i<ANALOG_INPUTS; i++)
        {
            adr = (i*0x100) + 0x3000;
            // timeris
            if(TableRead(adr+5, &rec) == 0) 
            {
                timer = rec+CAN_OPEN_TIMER_TICK_MS;
                TableWrite(adr+5, timer, 1); 
            }
            // ar siusti paketa?
            if(TableRead(adr+4, &rec) == 0)
            {
                if(sizeof(RECORD_TYPE)==2)
                {
                    if((rec != 0) && (rec != 0xFFFF)) 
                    {
                        if(timer > rec)
                        {
                            SendAnalogInput(i);
                            TableWrite(adr+5, 0, 1); 
                        }
                    }
                }
                else
                {
                    if((rec != 0) && (rec != 0xFFFFFFFF)) 
                    {
                        if(timer > rec)
                        {
                            SendAnalogInput(i);
                            TableWrite(adr+5, 0, 1); 
                        }
                    } 
                }    
            }            
        }
    #endif
    
    #if DIGITAL_INPUT_BLOCKS>0
        for(i=0; i<DIGITAL_INPUT_BLOCKS; i++)
        {
            adr = (i*0x100) + 0x0000;
            // timeris
            if(TableRead(adr+5, &rec) == 0) 
            {
                timer = rec+CAN_OPEN_TIMER_TICK_MS;
                TableWrite(adr+5, timer, 1); 
            }
            // ar siusti paketa?
            if(TableRead(adr+4, &rec) == 0)
            {
                if(sizeof(RECORD_TYPE)==2)
                {
                    if((rec != 0) && (rec != 0xFFFF)) 
                    {
                        if(timer > rec)
                        {
                            SendDigitalInput(i);
                            TableWrite(adr+5, 0, 1); 
                        }
                    }
                }
                else
                {
                    if((rec != 0) && (rec != 0xFFFFFFFF)) 
                    {
                        if(timer > rec)
                        {
                            SendDigitalInput(i);
                            TableWrite(adr+5, 0, 1); 
                        }
                    } 
                }    
            }            
        }
    #endif
    
    if((SystemState & 0xF0) == SYS_STATE_OPERATIONAL)
    {     
        // statuso periodinis siuntimas
        if((TableRead(0x0500, &rec) == 0) && (TableRead(0x0503, &timer) == 0))
        {
            if(rec)
            {
                if(timer > rec)
                {   
                    TableWrite(0x0503, 0, 1);
                    ReadToCanMsg(0x0400,&msg);
                    SendCanMsg(&msg);
                }
            }
        }
        
        // ar centrinis valdiklis atsiliepia?
        if((TableRead(0x0501, &rec) == 0) && (TableRead(0x0502, &timer) == 0))
        {
            if(rec)
            {
                if(timer > rec)
                {
                    TableWrite(0x0502, 0, 1);
                    TableWrite(0x0503, 0, 1);
                    TableWrite(0x0300, 0x0002, 1); // klaidos kodas
                    ReadToCanMsg(0x0300,&msg);
                    SendCanMsg(&msg);
                    SystemStateChange(3);          // stabdom                   
                }
            }
        } 
    }

}
  


//==============================================================================================
// irenginio busenos keitimas
//----------------------------------------------------------------------------------------------
void SystemStateChange(RECORD_TYPE val)
{  
    CAN_MESSAGE msg;    
    if(val == 0) 
    {
        SystemState = SYS_STATE_INIT;
        TableWrite(0x0200, 0x0001, 1); // starto statusas
        TableWrite(0x0300, 0x0000, 1); // klaidos kodas
        TableWrite(0x0400, 0x0001, 1); 
        
        ReadToCanMsg(0x0200,&msg);
        SendCanMsg(&msg);
        ReadToCanMsg(0x0400,&msg);
        SendCanMsg(&msg);
    }
    if(val == 2) 
    {
        SystemState = SYS_STATE_OPERATIONAL;
        TableWrite(0x0400, 0x0002, 1); 
        #if DIGITAL_OUTPUT_BLOCKS>0
            UpdateDigitalOutputs();
        #endif
        #if ANALOG_OUTPUTS>0
            UpdateDigitalOutputs();
        #endif
        ReadToCanMsg(0x0400,&msg);
        SendCanMsg(&msg);
    }
    if(val == 3) 
    {
        SystemState = SYS_STATE_STOPPED;
        TableWrite(0x0400, 0x0003, 1); 
        #if DIGITAL_OUTPUT_BLOCKS>0
            ClearOutputs(0, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>1
            ClearOutputs(1, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>2
            ClearOutputs(2, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>3
            ClearOutputs(3, 0xFFFFFFFF);
        #endif
        ReadToCanMsg(0x0400,&msg);
        SendCanMsg(&msg);
    }
    if(val == 4) 
    {
        SystemState = SYS_STATE_ERROR;
        TableWrite(0x0400, 0x0004, 1); 
        #if DIGITAL_OUTPUT_BLOCKS>0
            ClearOutputs(0, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>1
            ClearOutputs(1, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>2
            ClearOutputs(2, 0xFFFFFFFF);
        #endif
        #if DIGITAL_OUTPUT_BLOCKS>3
            ClearOutputs(3, 0xFFFFFFFF);
        #endif
        ReadToCanMsg(0x0400,&msg);
        SendCanMsg(&msg);
    }
    //if(val == 5) StartBooloader();// iejimas i bootloaderi
    //....... prideti ko reikia
}

/**
  * @brief  Calibration function via CAN.
  * @param  bank: by dafault always 0 
  * @param  index: sensor number from 0.. 
  * @param  val: calibration point number (for hunidity sensor 0..8)
  * @retval None
  */
void SpecialIO_Output(unsigned short bank, unsigned short index,  RECORD_TYPE val){
  
  if(index<MAXTCHANNEL){//Temperature sensors
    CalibrationTypeDef TemperCalibrationValues;
    ReadCalibration(&TemperCalibrationValues,index);
    if(val==0){
      TemperCalibrationValues.cbCalibrValue0=GetTSensorADCValue(index);
    }else{
      TemperCalibrationValues.cbCalibrValue100=GetTSensorADCValue(index);
    }
    //save calibration values
    WriteCalibration(&TemperCalibrationValues,index);
    LoadCalibrationData();
  }else{//Hunidity sensors
    if(I2C_GetBoardsNr()>0){
      //select board
      if(index<(MAXTCHANNEL+MAXHCHANNEL)){
        SelectBoardNr(0);
      }else if(index>(MAXTCHANNEL+MAXHCHANNEL-1)){
        SelectBoardNr(1);
      }
      //save calibration value
      SaveHCalibrationPoint((index-MAXTCHANNEL)<MAXHCHANNEL ? index-MAXTCHANNEL : index-MAXTCHANNEL-MAXHCHANNEL,val);
      Delay(15);
    }
  }
}

