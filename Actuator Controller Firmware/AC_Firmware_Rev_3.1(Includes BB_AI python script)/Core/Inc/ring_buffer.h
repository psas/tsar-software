
#ifndef __RINGBUFFER_H__											 // header guard so the file can't be included more than once
#define __RINGBUFFER_H__

#include <stdio.h>												     // include for printf
#include <stdlib.h>													 // include standard library
#include <stdbool.h>
/*
// declare variables in struct
typedef struct Ringbuffer
{
	char* buff;      													  // ring buffer
	uint32_t  bufferLength;                                                        // ring buffer size
	uint32_t  writeIndex;                                                            // the index with the newest information
	uint32_t  readIndex;                                                            // the index where the new information will be saved
} ringbuffer;*/

//100 rows available for data storage
static uint32_t SIZE_OF_BUFFER = 120;
void ringBuffer(char data[]);
void ringBufferRead();
uint32_t initOnceFlag;
uint32_t writeIndex, readIndex, bufferLength, circular;
char circularBuffer[120][100];
char circular_buffer_readFlag;
uint32_t init_time, later_time;

#endif
