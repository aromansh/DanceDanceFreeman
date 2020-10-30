#ifndef DDF_INPUT_H
#define DDF_INPUT_H

#include "error.h"

enum KeyState {Up = 0, Down = 1, Repeated = 2};

struct PadState {
	enum KeyState up;
	enum KeyState down;
	enum KeyState left;
	enum KeyState right;
	enum KeyState x;
	enum KeyState circle;
	enum KeyState square;
	enum KeyState triangle;
	enum KeyState select;
	enum KeyState start;
};

DDF_ERROR input_global_init();

struct PadState make_empty_pad_state();

void input_global_free();

void input_read(struct PadState* const state, DDF_ERROR* const err);

#endif /* DDF_INPUT_H */
