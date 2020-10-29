#ifndef DDF_KEYSTROKE_H
#define DDF_KEYSTROKE_H

#include "error.h"
#include "input.h"

DDF_ERROR keystroke_global_init();
void keystroke_global_free();

DDF_ERROR keystroke_update (struct PadState state);

#endif /* DDF_KEYSTROKE_H */
