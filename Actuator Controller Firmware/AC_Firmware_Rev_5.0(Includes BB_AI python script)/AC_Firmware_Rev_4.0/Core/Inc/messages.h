// messages.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for messages.c
//
	/*
	 * Description of the solenoid valve names.

	PV-F01:  Fuel Pressure : SOV1
	PV-L01:  Lox Pressure  : SOV2
	PV-P01:  Fuel Purge N2 : SOV3
	SOV-F01: Fuel Vent     : SOV4
	PV-F02:  Fuel Main     : SOV5
	PV-L03:   Lox Main     : SOV6
	PV-LF01: Lox Fill      : SOV7
	PV-L02:   Lox Vent     : SOV8
	PV-P02:   Lox Purge    : SOV9
	IG:   Igniter          : IG
	 */
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#ifndef __MESSAGES_H
#define __MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utilities.h"
#include <init.h>
#include "states_def.h"
// Definition of the time to procede to Shutdown after being in a certain state outside a nominal value range.
static uint32_t  time_to_shutoff1 =  100;
static uint32_t  time_to_shutoff2 =   100;


uint32_t ProcessMessages(struct StateVars (*));
uint32_t OnTickStatusMessage(struct StateVars (*));
uint32_t SendStatusMessage(struct StateVars (*));
void Get_State_Disagree_Error_Msg(char (*),enum StateName , enum StateName );
void Get_Invalid_State_Error_Msg(char (*), enum StateName , enum StateName );
void Get_Valve_State_Status_Msg(char (*), struct StateVars (*) , uint32_t );
void UART_SendMessage(UART_HandleTypeDef *huart, char* message);
void UART_SendMessage_VI(UART_HandleTypeDef *huart, char* message);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

//void handleMarionette(char marionette[]);
uint32_t splitValues(char *marionette_data, char separated[31][6]);


void NominalValueCheck(struct StateVars *ctrl, char marionette[]);
uint32_t  hexadecimalTouint32(char* hexVal);
uint32_t ProcessCommands(struct StateVars * ctrl);
uint32_t getWords(char *base, char target[10][20]);


uint32_t status, cmd_flag, command, empty, nominal, retransition, inv_flag, hexTuint, one_time_flag, time_1 ;
char buffer [256], Rx_indx, Rx_data[2], Rx_Buffer[200], Tx_Buffer[100], Marionette[30], marionette_data[120],  marionette_key[4] ;
volatile uint32_t RxTxFlags;
char arr[31][6]; 						//to hold the separated sensor values.
char notice[100]; 						//for the output message.

uint32_t sensor1, sensor2, sensor3,sensor4, sensor5, sensor6, sensor7, sensor8, sensor9, sensor10;
//
//variable to check if we have a data packet or a user command
// This variable will contain the amount of spaces that separate all the available sensor.
static uint32_t MAR_DATA_CHECK = 31;
//Data Packet Structure
// 3F99,PT-F01,PT-F02,PT-F03,TT-F01,PT-F04,TT-F02,PT-M02,PT-L01,PT-L02,PT-L03,TT-L01,PT-L04,TT-L02,TT-L03,TT-L04,TT-L05,PTM01,TP-1,TP-2,\
TP-3,TP-4, PV-F01(SOV1), SOV-F01(SOV4), PV-F02(SOV5), PV-P01(SOV3), PV-P02(SOV9), PV-L01(SOV2), PV-L02(SOV8), PV-L03(SOV6), PV-LF01(SOV7)\r



static uint32_t power_off     =  6835;    //hex: 1AB3    //off
static uint32_t shutdown      =  11086;   //hex: 2B4E
static uint32_t valve_check   =  19954;   //hex: 4DF2    //valve_check
static uint32_t lox_fill      =  24090;   //hex: 5E1A    //lox_fill
static uint32_t lox_chill     =  28555;   //hex: 6F8B    //lox_chill
static uint32_t lox_pre_fill  =  31250;   //hex: 7A12    //lox_prefill
static uint32_t pressurizing  =  35619;   //hex: 8B23    //pressurizing
static uint32_t safety        =  40005;   //hex: 9C45    //safety
static uint32_t ignition      =  43879;   //hex: AB67    //ignition
static uint32_t state_status  =  58866;   //hex: E5F2    //state status
static uint32_t valve_status  =  57322;   //hex: DFEA    //Valve status
static uint32_t resetac       =  46808;   //hex: B6D8    //reset the AC software

//Commands for the valve_check state (individual valves)
static uint32_t cycle_valves   =  57914;   //hex: E23A    //command to cycle valves one by one automatically
static uint32_t IGON           =  18759;   //hex: 4947    // Igniter on
static uint32_t IGOFF          =  13477;   //hex: 34A5   // Igniter off
static uint32_t pvf01on        =  24398;   //hex: 5F4E
static uint32_t pvf01off       =  36138;   //hex: 8D2A
static uint32_t pvl01on        =  15850;   //hex: 3DEA
static uint32_t pvl01off       =  27528;   //hex: 6B88
static uint32_t pvp01on        =  61867;   //hex: F1AB
static uint32_t pvp01off       =  49725;   //hex: C23D
static uint32_t sovf01on       =  46061;   //hex: B3ED
static uint32_t sovf01off      =  36426;   //hex: 8E4A
static uint32_t pvf02on        =  10983;   //hex: 2AE7
static uint32_t pvf02off       =  19052;   //hex: 4A6C
static uint32_t pvl03on        =  26536;   //hex: 67A8
static uint32_t pvl03off       =  16098;   //hex: 3EE2
static uint32_t pvlf01on       =  21926;   //hex: 55A6
static uint32_t pvlf01off      =  50615;   //hex: C5B7
static uint32_t pvl02on        =  46072;   //hex: B3F8
static uint32_t pvl02off       =  35071;   //hex: 88FF
static uint32_t pvp02on        =  47533;   //hex: B9AD
static uint32_t pvp02off       =  56639;   //hex: DD3F
static uint32_t marionette_cmd=  16281;   //hex: 3F99

#endif
