#pragma once


namespace lpir {


class KeypressSimulater
{
 public:
  KeypressSimulater();

  void TestKeypress(int x, int y);

 protected:
  void KeyPress(int key);

  void MouseMove(int x, int y);

  void MouseLeftClick(int x, int y);

  void MouseRightClick(int x, int y);
};


}
