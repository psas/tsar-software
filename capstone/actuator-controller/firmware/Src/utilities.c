// utilities.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This file contains methods for verification and
//  manipulation of state changes
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "utilities.h"

bool VerifyState(int state)
{
	bool success = false;
    if(state <= 0x1FF && state > 0)
    {
    	success = true;
		switch(state)
		{
			case SETUP_OPS:
				break;
			case LOX_FILL:
				break;
			case SITE_CLEAR_AND_START_UP:
				break;
			case PRESSURIZATION:
				break;
			case PRE_CHILL:
				break;
			case IGNITION:
				break;
			case LOX_INTRO:
				break;
			case BURN_INITIATED:
				break;
			case BURN_FEEDBACK:
				break;
			case BURN_TERMINATION_1:
				break;
			case BURN_TERMINATION_2:
				break;
			case BURN_TERMINATION_3:
				break;
			case PURGE:
				break;
			case POST_FIRE:
				break;
			case SAFE_APPROACH:
				break;
			case FAILURE:
				break;
			case GROUNDSTATION:
				break;
			case TEST:
				break;

			default:
				success = false;
				break;
		}
    }
	return success;
}

uint16_t StateConfiguration()
{
	uint16_t vc = 0;
	//PV1
	vc |= (uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin);

	//PV2
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 2);

	//PV3
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 3);

	//VV1
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 4);

	//VV2
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 5);

	//IV1
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 6);

	//IV2
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 7);

	//MV1
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 8);

	//MV2
	vc |= ((uint16_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 9);

	return vc;
}

bool ValveStateSetter(uint16_t vs)
{
	bool success = false;
	//PV1
	(vs & (uint16_t)PV1) == (uint16_t)PV1 \
			? HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_RESET);

	//PV2
	(vs & (uint16_t)PV2) == (uint16_t)PV2 \
			? HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_RESET);

	//PV3
	(vs & (uint16_t)PV3) == (uint16_t)PV3 \
			? HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_RESET);

	//VV1
	(vs & (uint16_t)VV1) == (uint16_t)VV1 \
			? HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_RESET);

	//VV2
	(vs & (uint16_t)VV2) == (uint16_t)VV2 \
			? HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_RESET);

	//IV1
	(vs & (uint16_t)IV1) == (uint16_t)IV1 \
			? HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_RESET);

	//IV2
	(vs & (uint16_t)IV2) == (uint16_t)IV2 \
			? HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_RESET);

	//MV1
	(vs & (uint16_t)MV1) == (uint16_t)MV1 \
			? HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_RESET);

	//MV2
	(vs & (uint16_t)MV2) == (uint16_t)MV2 \
			? HAL_GPIO_WritePin(GPIOD, GPIO2_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO2_Pin,GPIO_PIN_RESET);

	success = (vs == StateConfiguration() ? true : false);

	return success;
}
}
