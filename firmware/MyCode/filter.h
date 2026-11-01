/* filter.h */
#ifndef __FILTER_H
#define __FILTER_H

#include "main.h"

#define FILTER_WINDOW_SIZE  5

typedef struct {
    uint16_t buf[FILTER_WINDOW_SIZE];
    uint8_t  index;
    uint8_t  count;
    uint32_t sum;
} Filter_t;

void Filter_Init(Filter_t *f);
uint16_t Filter_Push(Filter_t *f, uint16_t new_value);

#endif

