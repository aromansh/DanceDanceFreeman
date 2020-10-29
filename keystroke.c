#include "keystroke.h"
#include <xdo.h>
#include <string.h>
#include <stdio.h>

const char* WIN_NAME = "Half-Life";
xdo_t* xdo;
Window* window;


DDF_ERROR keystroke_global_init(){
	xdo = xdo_new(NULL);

	// Search window
	xdo_search_t search;
	memset(&search, 0, sizeof(xdo_search_t));
	search.searchmask |= SEARCH_NAME;
	search.require = SEARCH_ANY;
	search.max_depth = -1;
	search.winname = WIN_NAME;

	Window* windows = NULL;
	unsigned int numWindows;
	
	xdo_search_windows(xdo, &search, &windows, &numWindows);
	printf("Num windows found: %u\n", numWindows);

	if(numWindows != 1){
		return DDF_ERR_NO_WINDOW_FOUND;
	}

	window = windows;
	return DDF_OK;
}

void keystroke_global_free(){
	xdo_free(xdo);
}

DDF_ERROR keystroke_update (struct PadState state){
	int rc;
	char* key = NULL;
	if(state.up == Down){
		printf("w\n");
		//key = "w";
		xdo_send_keysequence_window_down(xdo, *window, "w", 12);
	}else if(state.down == Down){
		printf("s\n");
		//key = "s";
		xdo_send_keysequence_window_down(xdo, *window, "s", 12);
	}else if(state.left == Down){
		printf("a\n");
		//key = "a";
		xdo_send_keysequence_window_down(xdo, *window, "Left", 12);
	}else if(state.right == Down){
		printf("d\n");
		//key = "d";
		xdo_send_keysequence_window_down(xdo, *window, "Right", 12);
	}

	if(state.up == Up){
		printf("!w\n");
		//key = "w";
		xdo_send_keysequence_window_up(xdo, *window, "w", 12);
	}
	if(state.down == Up){
		printf("!s\n");
		//key = "s";
		xdo_send_keysequence_window_up(xdo, *window, "s", 12);
	}
	if(state.left == Up){
		printf("!a\n");
		//key = "a";
		xdo_send_keysequence_window_up(xdo, *window, "Left", 12);
	}
	if(state.right == Up){
		printf("!d\n");
		//key = "d";
		xdo_send_keysequence_window_up(xdo, *window, "Right", 12);
	}
	/*if(key != NULL){
		xdo_send_keysequence_window_down(xdo, *window, key, 50);
	}*/
	

	return DDF_OK;
}
