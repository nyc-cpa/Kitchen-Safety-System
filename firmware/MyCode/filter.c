#include "filter.h"

void Filter_Init(Filter_t *f)
{
    f->index = 0;
    f->count = 0;
    f->sum   = 0;
    for (uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) {
        f->buf[i] = 0;
    }
}

uint16_t Filter_Push(Filter_t *f, uint16_t new_value)
{
    f->sum -= f->buf[f->index];
    f->buf[f->index] = new_value;
    f->sum += new_value;
    f->index = (f->index + 1) % FILTER_WINDOW_SIZE;
    if (f->count < FILTER_WINDOW_SIZE) {
        f->count++;
    }
    return (uint16_t)(f->sum / f->count);
}
