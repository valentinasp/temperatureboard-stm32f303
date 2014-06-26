/*
* File Name          : meniu.c
* Author             : vpa
* Version            : V0.0.1
* Date               : 18/2/2013
* Description        : Menu functons
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "menu_core.h"
#include "stdio.h"
#include "calibration.h"
#include "delay.h"
#include "i2c.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const SCMD ChannelMenu[] = {
   "1",cmd_channal1,
   "2",cmd_channal2,
   "3",cmd_channal3,
   "4",cmd_channal4,
   "5",cmd_channal4,
   "6",cmd_channal4,
   NULL,NULL
};

SCMD BoardMenu[] = {
   "1",cmd_board1,
   "2",cmd_board2,
   "3",cmd_board3,
   "4",cmd_board4,
   NULL,NULL
};

const SCMD CalibMenu[] = {
   "TERM",      cmd_termcalibration,
   "OTHER",     cmd_calibrationboards,
   "BACK",      cmd_back,
   NULL,NULL
};
/* Local constants */
static const char intro[] =
   "\r\n"
   "+-----------------------------------------------------------------------+\r\n"
   "|                       Main Menu                                       |\n\r";
static const char help[] = 
   "+ command ------------------+ function ---------------------------------+\r\n"
   "|                           |                                           |\n\r"
   "| INFO                      | Display version and sertification         |\r\n"
   "| ERASE                     | Erase calibration values                  |\r\n"
   "| RUNCAL                    | Run calibration application               |\r\n"
   "| SETID                     | Selectiong board using board ID number    |\r\n"
   "| ?                         | Print commands inro (help)                |\r\n"
   "| EXIT                      | Exit from main meniu                      |\r\n"
   "+---------------------------+-------------------------------------------+\r\n";

static const char select_calibration[] = 
"\r\n"
   "+-----------------------------------------------------------------------+\r\n"
   "|                       Select Calibration                              |\n\r"
   "+ command ------------------+ function ---------------------------------+\r\n"
   "|                           |                                           |\n\r"
   "| TERM                      | Calibration on board temperature sensors  |\r\n"
   "| OTHER                     | Calibration of other boards (via I2C)     |\r\n"
   "| BACK                      | Return to main menu                       |\r\n"
   "+---------------------------+-------------------------------------------+\r\n";

static const char board_intro[] =
   "\r\n"
   "+-----------------------------------------------------------------------+\r\n"
   "|                       Select Board                                    |\n\r"
   "+ command (NR) ---+ board I2C address --+-- bord indetification --------+\r\n"
   "|                 |                     |                               |\n\r";

static const char board_tag[] =
   "+-----------------+---------------------+-------------------------------+\r\n";

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void clear_screen(void);
void table(const char *title, const char *mode);//color test

static void clear_screen(void){
  printf("\x1b[2J");//terminal clear screen command
}
bool cmd_bootinfo(void){
  printf("\n\rVersion: %d.0",1);
  return false;
}

bool cmd_run_calibration(void){
  clear_screen();
  printf(select_calibration);
  Menu(CalibMenu);      
  return true;
}

bool cmd_calibrationboards(void){
  clear_screen();
  printf(board_intro);
  I2C_PrintBoardsList();
  printf(board_tag);
  BoardMenu[I2C_GetBoardsNr()].m_Function=NULL;
  Menu(BoardMenu);
  return false;
}

bool cmd_board1(void){
  clear_screen();
  printf("Calibration b1\r\n");
  Delay(3000);
  return false;
}

bool cmd_board2(void){
  clear_screen();
  printf("Calibration b2\r\n");
  Delay(3000);
  return false;
}
bool cmd_board3(void){
  clear_screen();
  printf("Calibration b3\r\n");
  Delay(3000);
  return false;
}
bool cmd_board4(void){
  clear_screen();
  printf("Calibration b4\r\n");
  Delay(3000);
  return false;
}

bool cmd_back(void){
  cmd_help();
  return false;
}

bool cmd_help(void){
  clear_screen();
  printf(intro);/* display example info        */
  printf(help);
  return true;
}

bool cmd_exit(void){
  clear_screen();
  printf("Please restart device...");
  return false;
}

bool cmd_erase_calibration(void){
  clear_screen();
  if(EraseCalibration()){
    printf("Erase calibration data is done..");
  }else{
    printf("Error erasing calibration data!!!");
  }
  return true;
}

bool cmd_termcalibration(void){
//================ Color test ========================
  //table("normal ( ESC[22m or ESC[m )", "22;");
  //table("bold ( ESC[1m )", "1;");
  //table("faint ( ESC[2m ), not well supported", "2;");
  //table("italic ( ESC[3m ), not well supported", "3;");
  //table("underline ( ESC[4m ), support varies", "4;");
  //table("blink ( ESC[5m )", "5;");
  //table("inverted ( ESC[7m )", "7;");
  //printf("\n\r \033[7m Inverted Text \r\n \033[m Returned to normal text \r\n ");
//================ End color test =====================
  
  printf("\r\nCalibration process is start.\r\nPlease select a channel (1..6) \r\n");
  Menu(ChannelMenu);      
  
  return true;
}

bool cmd_setboard_id(void){
  
  return true;
}

void table(const char *title, const char *mode)
{
	int f, b;
	printf("\n\033[1m%s\033[m\n bg\t fg\n", title);
	for (b = 40; b <= 107; b++) {
		if (b == 48) b = 100;
		printf("%3d\t\033[%s%dm", b, mode, b);
		for (f = 30; f <= 97; f++) {
			if (f == 38) f = 90;
			printf("\033[%dm%3d ", f, f);
		}
		puts("\033[m");
	}
}

bool cmd_channal1(void){
  clear_screen();
//  CalibrationProcess(Channel1);
  LoadCalibrationData();
  clear_screen();
  printf("Calibration done...Saving value...\r\n");
  Delay(3000);
  cmd_help();
  return false;
}
bool cmd_channal2(void){
  clear_screen();
  //CalibrationProcess(Channel2);
  LoadCalibrationData();
  clear_screen();
  printf("Calibration done...Saving value...\r\n");
  Delay(3000);
  cmd_help();
  return false;
}
bool cmd_channal3(void){
  clear_screen();
  //CalibrationProcess(Channel3);
  LoadCalibrationData();
  clear_screen();
  printf("Calibration done...Saving value...\r\n");
  Delay(3000);
  cmd_help();
  return false;
}

bool cmd_channal4(void){
  clear_screen();
  //CalibrationProcess(Channel4);
  LoadCalibrationData();
  clear_screen();
  printf("Calibration done...Saving value...\r\n");
  Delay(3000);
  cmd_help();
  return false;
}
