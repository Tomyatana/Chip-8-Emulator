#ifndef INPUT_C
#define INPUT_C

#include "definitions.h"

#define GET_KEY(x) ((key_states>>x)&1)
#define SET_KEY(x) key_states |= (1<<x)

u16 key_states = 0;

#endif
