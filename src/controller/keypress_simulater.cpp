#include "keypress_simulater.h"
#include <ApplicationServices/ApplicationServices.h>
#include <string>


using namespace lpir;


const static int kKeyTable[10] = {
  // 0,    1,    2,    3,    4,
  0x1D, 0x12, 0x13, 0x14, 0x15,
  // 5,    6,    7,    8,    9
  0x17, 0x16, 0x1A, 0x1C, 0x19,
};

void KeypressSimulater::InputAt(const std::string &input, const QPoint &pos) {
  PressAt(pos);

  for (auto i : input) {
    int key = i - '0';
    if (key < 0 || key > 9) continue;
    KeyPress(kKeyTable[key]);
  }
}

void KeypressSimulater::PressAt(const QPoint &pos) {
  MouseMove(pos.x(), pos.y());
  MouseLeftClick(pos.x(), pos.y());
  MouseLeftClick(pos.x(), pos.y());
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
  CGEventRef down_evt = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventRef up_evt = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, down_evt);
  CGEventPost(kCGHIDEventTap, up_evt);
  CFRelease(up_evt);
  CFRelease(down_evt);
}

void KeypressSimulater::MouseRightClick(int x, int y) {
  CGEventRef down_evt = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, CGPointMake(x, y), kCGMouseButtonRight);
  CGEventRef up_evt = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, CGPointMake(x, y), kCGMouseButtonRight);
  CGEventPost(kCGHIDEventTap, down_evt);
  CGEventPost(kCGHIDEventTap, up_evt);
  CFRelease(up_evt);
  CFRelease(down_evt);
}
