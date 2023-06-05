/** 
 * ***************************************************************** 
 * @file    TF_Config.h
 * @author  Home(zhhuang@andartechs.com.cn) 
 * @version 1.0.0 
 * @date    2022-02-07
 * @brief    
 *  
 *                 Copyright (c) 2022, Andar Technologies Inc. 
 *                           www.andartechs.com 
 *  
 * ***************************************************************** 
 */
#ifndef __TF_CONFIG_H
#define __TF_CONFIG_H

#include <stdint.h>
#include <stdio.h>

typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

#define GUI 1

#define TF_ID_BYTES 2
#define TF_LEN_BYTES 2
#define TF_TYPE_BYTES 2
#define TF_CKSUM_TYPE TF_CKSUM_XOR
#define TF_USE_SOF_BYTE 1
#define TF_SOF_BYTE 0x01
typedef uint16_t TF_TICKS;
typedef uint8_t TF_COUNT;
#define TF_MAX_PAYLOAD_RX 128
#define TF_SENDBUF_LEN 2048
#define TF_MAX_ID_LST 10
#define TF_MAX_TYPE_LST 10
#define TF_MAX_GEN_LST 5
#define TF_PARSER_TIMEOUT_TICKS 10

#define TF_Error(format, ...) printf("[TF] " format "\n", ##__VA_ARGS__)

#endif // TF_CONFIG_H
