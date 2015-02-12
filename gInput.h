#ifndef G_INPUT_H__
#define G_INPUT_H__

#include "gTools.h"
#include "gGears.h"
#include "FixedSizedArray.h"


const int KEY_COUNT = GLFW_KEY_LAST + 1;
const int MAX_KEY_MASK = 128;
const int MAX_KEY_LISTENER = 128;

enum{
	KEY_DOWN = 0x1,
	KEY_UP = 0x2,
	KEY_REPEAT = 0x4,

	KEY_ONE_TICK_STATE = 0x8,

	KEY_PRESSED = KEY_DOWN | KEY_ONE_TICK_STATE,
	KEY_RELEASED = KEY_UP | KEY_ONE_TICK_STATE,
	KEY_TYPED = KEY_DOWN | KEY_REPEAT | KEY_ONE_TICK_STATE,

	KEY_MASK_LEFT_SHIFT_DOWN = 0x10,
	KEY_MASK_RIGHT_SHIFT_DOWN = 0x20,

	KEY_MASK_LEFT_CONTROL_DOWN = 0x40,
	KEY_MASK_RIGHT_CONTROL_DOWN = 0x80,

	KEY_MASK_LEFT_ALT_DOWN = 0x100,
	KEY_MASK_RIGHT_ALT_DOWN = 0x200,

	KEY_MASK_LEFT_SUPER_DOWN = 0x400,
	KEY_MASK_RIGHT_SUPER_DOWN = 0x800,

	KEY_MASK_ALL = 0xFFFFFFFF,
};

class gInputListener{
	friend class gInput;
private:
	struct KeyMask{
		int key;
		int mask;
	};

	struct KeyMask keyMasks[MAX_KEY_MASK];
	int keyMaskCount;

protected:
	void addKeyMask(int key, int mask);
	bool doesAcceptKey(int key, int keyState);
	void addKeyMask(int key);
public:
	gInputListener(bool autoAdd = true);
	virtual ~gInputListener();
	virtual void onKey(int key, int keyState) {}
};

class gInput{
	friend class gGears;

private:
	char keyState[KEY_COUNT];
	float keyTime[KEY_COUNT];

	FixedSizedArray<int, KEY_COUNT> stateChangedKeys;

	int keyStateHeader;

	FixedSizedArray<gInputListener *, MAX_KEY_LISTENER> listeners;

	void informListeners();
	void clearOneTickStates();

	int getKeyState(int key){
		return keyState[key] | keyStateHeader;
	}

	int checkKeyState(int key, int state){
		return (getKeyState(key) | state) != 0;
	}

	void keyPress(float t, int key);
	void keyRelease(float t, int key);
	void keyType(float t, int key);
public:
	gInput();

	void addListener(gInputListener *listener){
		listeners.insert(listener);
	}
	void removeListener(gInputListener *listener){
		listeners.remove(listener);
	}
	
	//returns true if key is down
	bool isKeyDown(int key)
	{
		return (keyState[key] & KEY_DOWN) != 0;
	}
	//returns true if key is up
	bool isKeyUp(int key)
	{
		return (keyState[key] & KEY_UP) != 0;
	}
	//returns true if key is pressed in this fixed update
	bool isKeyPressed(int key)
	{
		return (keyState[key] & KEY_PRESSED) != 0;
	}
	//returns true if key is released in this fixed update
	bool isKeyReleased(int key)
	{
		return (keyState[key] & KEY_RELEASED) != 0;
	}
	//returns true if key is repeated in this fixed update
	bool isKeyTyped(int key)
	{
		return (keyState[key] & KEY_TYPED) != 0;
	}
	//returns the time stamp of last key press/release
	float getKeyTime(int key)
	{
		return keyTime[key];
	}
};

#endif