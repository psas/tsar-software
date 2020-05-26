// post_fire.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for post_fire.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef POST_FIRE_H
#define POST_FIRE_H

#include "states_def.h"
#include "utilities.h"
#include "messages.h"

uint32_t PostFire(enum StateName (*), enum StateName (*));

#endif // POST_FIRE_H
