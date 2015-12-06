#pragma once

#include <stdint.h>

void ak4558_init();

// Number of stereo samples
#define AUDIO_BUFFER_SIZE 32

extern int32_t *audio_in_buffer;
extern int32_t *audio_out_buffer;
extern volatile int audio_buffers_fresh;
