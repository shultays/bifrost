#ifndef G_INPUT_H__
#define G_INPUT_H__

#include "gTools.h"
#include "gGears.h"
#include "FixedSizedArray.h"


const int KEY_COUNT = GLFW_KEY_LAST + 1 + (GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1) /*mouse*/;
const int MAX_KEY_MASK = 128;
const int MAX_KEY_LISTENER = 128;

#define MOUSE_BUTTON_1 (GLFW_KEY_LAST + 1)
#define MOUSE_BUTTON_LEFT MOUSE_BUTTON_1
#define MOUSE_BUTTON_2 (GLFW_KEY_LAST + 2)
#define MOUSE_BUTTON_RIGHT MOUSE_BUTTON_2
#define MOUSE_BUTTON_3 (GLFW_KEY_LAST + 3)
#define MOUSE_BUTTON_MID MOUSE_BUTTON_3
#define MOUSE_BUTTON_4 (GLFW_KEY_LAST + 4)
#define MOUSE_BUTTON_5 (GLFW_KEY_LAST + 5)
#define MOUSE_BUTTON_6 (GLFW_KEY_LAST + 6)
#define MOUSE_BUTTON_7 (GLFW_KEY_LAST + 7)
#define MOUSE_BUTTON_8 (GLFW_KEY_LAST + 8)

#define MOUSE_START (MOUSE_BUTTON_1)
#define MOUSE_COUNT (MOUSE_BUTTON_8 - MOUSE_BUTTON_1 + 1)

enum {
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

class gInputListener {
	friend class gInput;
private:
	struct KeyMask {
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

class gInput {
	friend class gGears;

private:
	char keyState[KEY_COUNT];
	float keyTime[KEY_COUNT];

	Vec2 mouseActionPos[MOUSE_COUNT];
	Vec2 mousePos;
	Vec2 mousePosAtUpdate;

	Vec2 scrollSinceUpdate;

	FixedSizedArray<int, KEY_COUNT> stateChangedKeys;

	int keyStateHeader;

	FixedSizedArray<gInputListener *, MAX_KEY_LISTENER> listeners;

	void informListeners();
	void clearOneTickStates();

	int getKeyState(int key) {
		return keyState[key] | keyStateHeader;
	}

	int checkKeyState(int key, int state) {
		return (getKeyState(key) | state) != 0;
	}

	void keyPress(float t, int key);
	void keyRelease(float t, int key);
	void keyType(float t, int key);

	void mouseMove(const Vec2& pos);

	void scroll(const Vec2& delta) {
		scrollSinceUpdate += delta;
	}

	bool mouseInited;
public:
	gInput();

	void addListener(gInputListener *listener) {
		listeners.insert(listener);
	}
	void removeListener(gInputListener *listener) {
		listeners.remove(listener);
	}

	//returns true if key is down
	bool isKeyDown(int key) const {
		return (keyState[key] & KEY_DOWN) == KEY_DOWN;
	}
	//returns true if key is up
	bool isKeyUp(int key) const {
		return (keyState[key] & KEY_UP) == KEY_UP;
	}
	//returns true if key is pressed in this fixed update
	bool isKeyPressed(int key) const {
		return (keyState[key] & KEY_PRESSED) == KEY_PRESSED;
	}
	//returns true if key is released in this fixed update
	bool isKeyReleased(int key) const {
		return (keyState[key] & KEY_RELEASED) == KEY_RELEASED;
	}
	//returns true if key is repeated in this fixed update
	bool isKeyTyped(int key) const {
		return (keyState[key] & KEY_TYPED) == KEY_TYPED;
	}
	//returns the time stamp of last key press/release
	float getKeyTime(int key) const {
		return keyTime[key];
	}

	Vec2 getMousePos() const {
		return mousePos;
	}
	Vec2 getMouseDelta() const {
		return mousePos - mousePosAtUpdate;
	}

	Vec2 getMouseKeyActionPos(int key) const {
		int i = key - MOUSE_START;
		if (i >= MOUSE_COUNT || i < 0) {
			return Vec2::zero();
		}
		return mouseActionPos[i];
	}

	Vec2 getScrollDelta() const {
		return scrollSinceUpdate;
	}

	bool isScrolled() const {
		return scrollSinceUpdate.isNonZero();
	}
};

#endif