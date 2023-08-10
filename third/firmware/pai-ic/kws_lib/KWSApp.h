/**********************************************************
* API for KWS model library
* Copyright (C) 2022 AiZip Inc. All rights reserved.
**********************************************************/

#ifndef __AZPKWS_H__
#define __AZPKWS_H__


#include <stdint.h>


#define NUM_WAKEWORD    1


#ifdef __cplusplus
extern "C" {
#endif

typedef enum __KWS_STATUS
{
	errKWS_NoError = 0,
	errKWS_NotReady,
	errKWS_InvalidParam,
	errKWS_InvalidLicense,
	errKWS_BufferOverflow
}
KWS_STATUS;


/**********************************************************
* Initialize KWS model
* iobuf: the pointer to the RAM buffer allocated for KWS algorithm
* iosize: size of the allocated buffer
* dbuf: reserved, should be 0
* dsize: reserved, should be 0
**********************************************************/
KWS_STATUS kws_init(int8_t * iobuf, int iosize, int8_t * dbuf, int dsize);

/**********************************************************
* Get the KWS model version number
**********************************************************/
uint32_t kws_get_version(void);

/**********************************************************
* Get the audio signal sampling specs
* rate: pointer to receive the audio sampling rate
* buffsize: pointer to receive the number of element in audio buffer
**********************************************************/
KWS_STATUS kws_get_specs(int * rate, int * buffsize);

/**********************************************************
* Load audio input data into KWS model
* audio: audio data buffer
* size: number of elements (16-bit) in the audio buffer
**********************************************************/
KWS_STATUS kws_load_input(int16_t * audio, int size);

/**********************************************************
* Run KWS model to get keywords classification
* top: pointer to receive top class index
* prob: pointer to receive top class probability
**********************************************************/
KWS_STATUS kws_classify(uint8_t * top, uint8_t * prob);

/**********************************************************
* Release KWS model memory
**********************************************************/
void kws_free(void);


#ifdef __cplusplus
}
#endif

#endif	// __AZPKWS_H__
