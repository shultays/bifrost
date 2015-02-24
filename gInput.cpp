#include "gInput.h"
#include "gGlobals.h"

gInputListener::~gInputListener() {
	input.removeListener(this);
}

gInputListener::gInputListener(bool autoAdd) {
	keyMaskCount = 0;
	if (autoAdd) {
		input.addListener(this);
	}
}

void gInputListener::addKeyMask(int key, int mask) {
	keyMasks[keyMaskCount].key = key;
	keyMasks[keyMaskCount].mask = mask;
	keyMaskCount++;
}

bool gInputListener::doesAcceptKey(int key, int keyState) {
	if (keyMaskCount == 0) return true;
	for (int i = 0; i < keyMaskCount; i++) {
		if ((keyMasks[i].key == KEY_MASK_ALL || keyMasks[i].key == key) && (keyMasks[i].mask == KEY_MASK_ALL || (keyState & keyMasks[i].mask) == keyMasks[i].mask)) {
			return true;
		}
	}
	return false;
}

void gInputListener::addKeyMask(int key) {
	addKeyMask(key, KEY_MASK_ALL);
}


void gInput::informListeners() {
	for (int j = 0; j < stateChangedKeys.size(); j++) {
		int key = stateChangedKeys[j];
		int keyState = getKeyState(key);
		for (int i = 0; i < listeners.size(); i++) {
			if (listeners[i]->doesAcceptKey(key, keyState)) {
				listeners[i]->onKey(key, keyState);
			}
		}
	}
}

void gInput::clearOneTickStates() {
	mousePosAtUpdate = mousePos;
	scrollSinceUpdate = Vec2::zero();
	for (int i = 0; i < KEY_COUNT; i++) {
		keyState[i] &= ~KEY_ONE_TICK_STATE;
	}
	stateChangedKeys.clear();
}



void gInput::keyPress(float t, int key) {
	keyState[key] = KEY_PRESSED;
	keyTime[key] = t;
	stateChangedKeys.insert(key);
	switch (key) {
		case GLFW_KEY_LEFT_CONTROL:
			keyStateHeader |= KEY_MASK_LEFT_CONTROL_DOWN;
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			keyStateHeader |= KEY_MASK_RIGHT_CONTROL_DOWN;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			keyStateHeader |= KEY_MASK_LEFT_SHIFT_DOWN;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			keyStateHeader |= KEY_MASK_RIGHT_SHIFT_DOWN;
			break;
		case GLFW_KEY_LEFT_ALT:
			keyStateHeader |= KEY_MASK_LEFT_ALT_DOWN;
			break;
		case GLFW_KEY_RIGHT_ALT:
			keyStateHeader |= KEY_MASK_RIGHT_ALT_DOWN;
			break;
		case GLFW_KEY_LEFT_SUPER:
			keyStateHeader |= KEY_MASK_LEFT_SUPER_DOWN;
			break;
		case GLFW_KEY_RIGHT_SUPER:
			keyStateHeader |= KEY_MASK_RIGHT_SUPER_DOWN;
			break;
		default:
			break;
	}

	if (key >= MOUSE_START && key < MOUSE_START + MOUSE_COUNT) {
		mouseActionPos[key - MOUSE_START] = mousePos;
	}
}
void gInput::keyRelease(float t, int key) {
	keyState[key] = KEY_RELEASED;
	keyTime[key] = t;
	stateChangedKeys.insert(key);
	switch (key) {
		case GLFW_KEY_LEFT_CONTROL:
			keyStateHeader &= ~KEY_MASK_LEFT_CONTROL_DOWN;
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			keyStateHeader &= ~KEY_MASK_RIGHT_CONTROL_DOWN;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			keyStateHeader &= ~KEY_MASK_LEFT_SHIFT_DOWN;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			keyStateHeader &= ~KEY_MASK_RIGHT_SHIFT_DOWN;
			break;
		case GLFW_KEY_LEFT_ALT:
			keyStateHeader &= ~KEY_MASK_LEFT_ALT_DOWN;
			break;
		case GLFW_KEY_RIGHT_ALT:
			keyStateHeader &= ~KEY_MASK_RIGHT_ALT_DOWN;
			break;
		case GLFW_KEY_LEFT_SUPER:
			keyStateHeader &= ~KEY_MASK_LEFT_SUPER_DOWN;
			break;
		case GLFW_KEY_RIGHT_SUPER:
			keyStateHeader &= ~KEY_MASK_RIGHT_SUPER_DOWN;
			break;
		default:
			break;
	}

	if (key >= MOUSE_START && key < MOUSE_START + MOUSE_COUNT) {
		mouseActionPos[key - MOUSE_START] = mousePos;
	}
}
void gInput::keyType(float t, int key) {
	keyState[key] = KEY_TYPED;
	keyTime[key] = t;
	stateChangedKeys.insert(key);
}

gInput::gInput() {
	for (int i = 0; i < KEY_COUNT; i++) {
		keyState[i] = KEY_UP;
		keyTime[i] = 0.0f;
	}
	mouseInited = false;
	for (int i = 0; i < MOUSE_COUNT; i++) {
		mousePos = Vec2::zero();
	}
	mousePos = Vec2::zero();
	mousePosAtUpdate = Vec2::zero();
	scrollSinceUpdate = Vec2::zero();

	keyStateHeader = 0;
	listeners.clear();
}


void gInput::mouseMove(const Vec2& pos) {
	mousePos = pos;
	if (!mouseInited) {
		mouseInited = true;
		for (int i = 0; i < MOUSE_COUNT; i++) {
			mousePos = mousePos;
		}
		mousePos = mousePos;
		mousePosAtUpdate = mousePos;
	}
}