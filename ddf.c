#include <stdio.h>
#include <signal.h>
#include "input.h"
#include "error.h"
#include "keystroke.h"

unsigned char kill_signal_received = 0;
void handle_signal(int sig){
	kill_signal_received = 1;
}

static int init_global_systems(){
	signal(SIGINT, handle_signal);
	signal(SIGKILL, handle_signal);

	if(input_global_init() != DDF_OK){
		fprintf(stderr, "Couldn't initialize input system!\n");
		return 1;
	}

	if(keystroke_global_init() != DDF_OK){
		fprintf(stderr, "Couldn't initialize keystroke system!\n");
		return 1;
	}

	return 0;
}

static void free_global_systems(){
	input_global_free();
	keystroke_global_free();
}

static int loop(){
	DDF_ERROR err;
	struct PadState state = make_empty_pad_state();

	while(!kill_signal_received){
		// Read input
		input_read(&state, &err);
		if(err != DDF_OK){
			return err;
		}

		// Update keystrokes
		err = keystroke_update(state);
	}

	return 0;
}

int main(){
	if( init_global_systems() ){
		fprintf(stderr, "Unable to initialize global systems\n");
		return 1;
	}

	if( loop() ){
		fprintf(stderr, "Error while processing the game loop\n");
		return 1;
	}

	free_global_systems();
	
	return 0;
}


