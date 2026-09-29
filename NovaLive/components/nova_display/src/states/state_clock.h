#pragma once
#include "state_common.h"

// The rest screen: minimalist HH:MM, shown from boot until the wake word,
// and again whenever there's no active conversation.
const nova_state_screen_t *state_clock_ops(void);
