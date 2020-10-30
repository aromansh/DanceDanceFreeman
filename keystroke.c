#include "keystroke.h"
#include <xdo.h>
#include <string.h>
#include <stdio.h>

#define DDF_DEFAULT_DELAY 12

/* Modifiers to index mappings */
#define MOD_NONE			0
#define MOD_SELECT			1
#define MOD_START 			2
#define NUM_MODIFIERS		3

/* Dance pad buttons to index mappings */
#define DP_UP				0
#define DP_DOWN				1
#define DP_LEFT				2
#define DP_RIGHT			3
#define DP_CIRCLE			4
#define DP_SQUARE			5
#define DP_X				6
#define DP_TRIANGLE			7
#define NUM_DP_BUTTONS 		8

/* Actions to keyboard key mappings */
#define NOTHING					""
#define FORWARD					"w"
#define BACKWARD				"s"
#define STRIFE_LEFT				"a"
#define STRIFE_RIGHT			"d"
#define LOOK_UP					"Page_Up"
#define LOOK_DOWN				"Page_Down"
#define TURN_LEFT				"Left"
#define TURN_RIGHT				"Right"
#define CENTER_VIEW				"End"
#define USE						"e"
#define CROUCH					"Control_L"
#define JUMP					"space"
#define WEAPON_NEXT				"bracketright"
#define WEAPON_PREVIOUS			"bracketleft"
#define RELOAD					"r"
#define FLASHLIGHT				"f"
#define FAST_SAVE				"F6"
#define FAST_LOAD				"F7"
#define PREVIOUS_WEAPON			"q"
#define SHOOT_PRIMARY			"Return"
#define SHOOT_SECONDARY			"BackSpace" /* Requires modifying default mapping */

/* Dance pad buttons to actions mappings */
char* actions[NUM_MODIFIERS][NUM_DP_BUTTONS];

const char* WIN_NAME = "Half-Life";
xdo_t* xdo;
Window* window;

void map_buttons_to_actions(){
	actions[MOD_NONE][DP_UP] = FORWARD;
	actions[MOD_NONE][DP_DOWN] = BACKWARD;
	actions[MOD_NONE][DP_LEFT] = TURN_LEFT;
	actions[MOD_NONE][DP_RIGHT] = TURN_RIGHT;
	actions[MOD_NONE][DP_CIRCLE] = SHOOT_PRIMARY;
	actions[MOD_NONE][DP_SQUARE] = USE;
	actions[MOD_NONE][DP_X] = JUMP;
	actions[MOD_NONE][DP_TRIANGLE] = CROUCH;

	actions[MOD_SELECT][DP_UP] = LOOK_UP;
	actions[MOD_SELECT][DP_DOWN] = LOOK_DOWN;
	actions[MOD_SELECT][DP_LEFT] = CENTER_VIEW;
	actions[MOD_SELECT][DP_RIGHT] = SHOOT_SECONDARY;
	actions[MOD_SELECT][DP_CIRCLE] = SHOOT_PRIMARY;
	actions[MOD_SELECT][DP_SQUARE] = RELOAD;
	actions[MOD_SELECT][DP_X] = FLASHLIGHT;
	actions[MOD_SELECT][DP_TRIANGLE] = NOTHING;

	actions[MOD_START][DP_UP] = WEAPON_NEXT;
	actions[MOD_START][DP_DOWN] = NOTHING;
	actions[MOD_START][DP_LEFT] = NOTHING;
	actions[MOD_START][DP_RIGHT] = SHOOT_PRIMARY;
	actions[MOD_START][DP_CIRCLE] = NOTHING;
	actions[MOD_START][DP_SQUARE] = /*FAST_LOAD*/ NOTHING;
	actions[MOD_START][DP_X] = WEAPON_PREVIOUS;
	actions[MOD_START][DP_TRIANGLE] = /*FAST_SAVE*/ NOTHING;
}


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
	map_buttons_to_actions();

	return DDF_OK;
}

void keystroke_global_free(){
	xdo_free(xdo);
}

/*
 * Press or release all the buttons, according to it's state and the func given
 */
void update_actions(struct PadState padState, enum KeyState targetKeyState,
						   int (*keyFunc)(const xdo_t*, Window, const char*, useconds_t)){
	// Retrieve modifier	
	int modifierIndex;
	if(padState.select == Down){
		modifierIndex = MOD_SELECT;
	}else if(padState.start == Down){
		modifierIndex = MOD_START;
	}else{
		modifierIndex = MOD_NONE;
	}

	// Press or release all buttons
	// Arrows
	if(padState.up == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_UP], DDF_DEFAULT_DELAY);
	}
	if(padState.down == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_DOWN], DDF_DEFAULT_DELAY);
	}
	if(padState.left == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_LEFT], DDF_DEFAULT_DELAY);
	}
	if(padState.right == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_RIGHT], DDF_DEFAULT_DELAY);
	}

	// Symbols
	if(padState.circle == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_CIRCLE], DDF_DEFAULT_DELAY);
	}
	if(padState.square == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_SQUARE], DDF_DEFAULT_DELAY);
	}
	if(padState.x == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_X], DDF_DEFAULT_DELAY);
	}
	if(padState.triangle == targetKeyState){
		keyFunc(xdo, *window, actions[modifierIndex][DP_TRIANGLE], DDF_DEFAULT_DELAY);
	}
}

DDF_ERROR keystroke_update (struct PadState state){
	//int rc;
	//char* key = NULL;

	// Define a function
	//int (*keyFunc)(const xdo_t*, Window, const char* useconds_t) = NULL;

	// Press and release all the single actions
	update_actions(state, Down, &xdo_send_keysequence_window_down);
	update_actions(state, Up, &xdo_send_keysequence_window_up);

/*
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
	if(key != NULL){
		xdo_send_keysequence_window_down(xdo, *window, key, 50);
	}
	*/

	return DDF_OK;
}
