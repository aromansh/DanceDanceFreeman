#include "input.h"
#include "error.h"
#include "libevdev/libevdev.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct libevdev* dev = NULL;
const char file[] = "/dev/input/event11";
int rc=1;

DDF_ERROR input_global_init(){
	int fd = open(file, O_RDONLY);
	if(fd < 0) {
		return DDF_ERR_CANNOT_OPEN_DEVICE;
	}

	rc = libevdev_new_from_fd (fd, &dev);
	if(rc < 0){
		return DDF_ERR_CANNOT_INIT_LIBEVDEV;
	}

	return DDF_OK;
}

void input_global_free(){
	libevdev_free(dev);
}

/*
static struct make_empty_pad_state(){
	struct PadState state;
	state.up = Down;
	state.down = Down;
	state.left = Down;
	state.right = Down;
	state.x = Down;
	state.circle = Down;
	state.square = Down;
	state.triangle = Down;
	state.select = Down;
	state.start = Down;
	return state;
}
*/

struct PadState make_empty_pad_state(){
	struct PadState state;
	state.up = Up;
	state.down = Up;
	state.left = Up;
	state.right = Up;
	state.x = Up;
	state.square = Up;
	state.triangle = Up;
	state.circle = Up;
	state.start = Up;
	state.select = Up;
	return state;
}


static enum KeyState parse_evdev_value(int value){
	enum KeyState state;
	switch(value){
		case 0:
		default:
			state = Up;
			break;
		case 1:
		case 255:
			state = Down;
			break;
		case 2:
			state = Repeated;
			break;
	}
	return state;
}


void update_state_from_evdev_event(struct PadState* state, struct input_event* ev){
	enum KeyState* key = NULL;
	switch(ev->code){
		case BTN_THUMB:
			key = &state->right;
			break;
		case BTN_TOP2:
			key = &state->up;
			break;
		case BTN_TRIGGER:
			key = &state->down;
			break;
		case BTN_TOP:
			key = &state->left;
			break;
		case BTN_PINKIE:
			key = &state->circle;
			break;
		case BTN_BASE2:
			key = &state->square;
			break;
		case BTN_THUMB2:
			key = &state->triangle;
			break;
		case ABS_Y:
			key = &state->x;
			break;
		case BTN_BASE4:
			key = &state->select;
			break;
		case BTN_BASE3:
			key = &state->start;
			break;
	}
	if(key != NULL){
		*key = parse_evdev_value(ev->value);
	}
}


static void update_state(struct PadState* state){

	struct input_event ev;
	do {
		// Retrieve event
		rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &ev);

		// If some events were lost, keep polling until syncronizing
		if( rc == LIBEVDEV_READ_STATUS_SYNC){
			while( rc == LIBEVDEV_READ_STATUS_SYNC){
				rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_SYNC, &ev);
			}
		}
		// If successfully retrieve event, return the parsed event
		//else if(rc == LIBEVDEV_READ_STATUS_SUCCESS && ev.type == EV_KEY ){
			//return parse_evdev_event(&ev);
		/*}else{
			printf("Empty state\n");
			return make_empty_pad_state();
		}*/
	} while(rc != LIBEVDEV_READ_STATUS_SUCCESS || (ev.type != EV_KEY && ev.type != EV_ABS));

	update_state_from_evdev_event(state, &ev);
}


void print_state(struct PadState* state){
	printf("Up %d, Down %d, Left %d, Right %d, X %d, Circle %d, Square %d, Triangle %d, Select %d, Start %d\n",
		(int)state->up,
		(int)state->down,
		(int)state->left,
		(int)state->right,
		(int)state->x,
		(int)state->circle,
		(int)state->square,
		(int)state->triangle,
		(int)state->select,
		(int)state->start
	);
}

void input_read(struct PadState* const state, DDF_ERROR* const err){
	update_state(state);
	print_state(state);
	*err = DDF_OK;
}

