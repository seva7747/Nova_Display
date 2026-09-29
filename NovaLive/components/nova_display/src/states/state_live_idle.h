#pragma once
#include "state_common.h"

// Shown while the session is live but nobody is talking (waiting for you
// to speak, or during the reply-window countdown).
const nova_state_screen_t *state_live_idle_ops(void);
