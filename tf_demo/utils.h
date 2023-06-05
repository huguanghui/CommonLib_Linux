#ifndef TF_UTILS_H
#define TF_UTILS_H

#include <stdio.h>

#include "TinyFrame.h"

typedef unsigned char* pu8;

void dumpFrame(const uint8_t *buff, size_t len);

void dumpFrameInfo(TF_Msg *msg);

#endif // !TF_UTILS_H
