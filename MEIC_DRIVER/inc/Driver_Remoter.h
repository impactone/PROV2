#ifndef __DRIVER_REMOTER_H
#define __DRIVER_REMOTER_H
#include "includes.h"
#include "BSP_UART.h"
#define  ST_Status    0x02
#define  RM_Status 0x01
#define  KM_Status  0x03
//#define  Lift_Status  0x11
//#define  Front_Lift   0x31
//#define  Behind_Lift  0x21
//#define  FrontAssitant_Status 0x23
//#define  MiddleAssitant_Status 0x33
#define  Standard_Speed 1000
#define  Lift_Speed  500
typedef struct
{
	uint16_t ch0;
	uint16_t ch1;
	uint16_t ch2;
	uint16_t ch3;
	uint8_t  left_switch;
	uint8_t  right_switch;	
	struct 
	{

		int16_t x; //!< Byte 6-7
		int16_t y; //!< Byte 8-9
		int16_t z; //!< Byte 10-11
		uint8_t l; //!< Byte 12
		uint8_t r; //!< Byte 13
	}mouse;
	struct
	{
	  uint8_t  Key_W; //!< Bit 0
    uint8_t  Key_S; //!< Bit 1
    uint8_t  Key_A; //!< Bit 2
    uint8_t  Key_D; //!< Bit 3
    uint8_t  Key_Q; //!< Bit 4
		uint8_t  Key_E; //!< Bit 5
		uint8_t  Shift; //!< Bit 6
		uint8_t  Ctrl;  //!< Bit 7
		uint8_t  Key_R;
		uint8_t  Key_F;
		uint8_t  Key_G;
		uint8_t  Key_Z;
		uint8_t  Key_X;
		uint8_t  Key_C;
		uint8_t  Key_V;
		uint8_t  Key_B;
		
	} Key;
  uint16_t key;
	uint16_t res;	
	
}DBUS_Type;

typedef enum
{
	CH0,
	CH1,
	CH2,
	CH3,
	LEFT_SWITCH,
	RIGHT_SWTICH,
	MOUSE_X,
	MOUSE_Y,
	MOUSE_Z,
	MOUSE_L,
	MOUSE_R,
	KEY,
	RES
}Receive_Type;


void DBUS_Init(void);
void DBUS_Decode(void);
int16_t GetRemoterMsg(Receive_Type code);
void Limits16_t(int16_t *t);
void Limits32_t(int32_t *t);
void Limit_Input(float *t,int16_t max);
void Remoter_SetState(uint8_t set);
uint8_t Judge(uint8_t x,uint8_t y);
uint8_t  Presskey_Judge(uint8_t key,uint8_t* loose);
#endif

