#include "keypress_simulater.h"
#include <ApplicationServices/ApplicationServices.h>


using namespace lpir;


enum KeyCode {
  KEY_ANSI_1 = 0x12,
  KEY_ANSI_2 = 0x13,
  KEY_ANSI_3 = 0x14,
  KEY_ANSI_4 = 0x15,
  KEY_ANSI_5 = 0x17,
  KEY_ANSI_6 = 0x16,
  KEY_ANSI_7 = 0x1A,
  KEY_ANSI_8 = 0x1C,
  KEY_ANSI_9 = 0x19,
  KEY_ANSI_0 = 0x1D,
};


KeypressSimulater::KeypressSimulater() {

}

void KeypressSimulater::TestKeypress(int x, int y) {
  MouseLeftClick(x, y);
  MouseLeftClick(x, y);
  KeyPress(KEY_ANSI_7);
  KeyPress(KEY_ANSI_0);
  KeyPress(KEY_ANSI_0);
}


void KeypressSimulater::KeyPress(int key) {
  // Create an HID hardware event source
  CGEventSourceRef press_src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
  // Create a new keyboard key press event
  CGEventRef press_evt = CGEventCreateKeyboardEvent(press_src, (CGKeyCode)key, true);
  // Post keyboard event and release
  CGEventPost (kCGHIDEventTap, press_evt);
  CFRelease(press_evt);
  CFRelease(press_src);

  // Create an HID hardware event source
  CGEventSourceRef release_src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
  // Create a new keyboard key release event
  CGEventRef release_evt = CGEventCreateKeyboardEvent(release_src, (CGKeyCode)key, false);
  // Post keyboard event and release
  CGEventPost (kCGHIDEventTap, release_evt);
  CFRelease(release_evt);
  CFRelease(release_src);
}

void KeypressSimulater::MouseMove(int x, int y) {
  CGEventRef evt = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, evt);
  CFRelease(evt);
}

void KeypressSimulater::MouseLeftClick(int x, int y) {
  MouseMove(x, y);
  CGEventRef down_evt = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventRef up_evt = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, down_evt);
  CGEventPost(kCGHIDEventTap, up_evt);
  CFRelease(up_evt);
  CFRelease(down_evt);
}

void KeypressSimulater::MouseRightClick(int x, int y) {
  MouseMove(x, y);
  CGEventRef down_evt = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, CGPointMake(x, y), kCGMouseButtonRight);
  CGEventRef up_evt = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, CGPointMake(x, y), kCGMouseButtonRight);
  CGEventPost(kCGHIDEventTap, down_evt);
  CGEventPost(kCGHIDEventTap, up_evt);
  CFRelease(up_evt);
  CFRelease(down_evt);
}
