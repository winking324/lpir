#pragma once
#include <QPoint>


namespace lpir {


class KeypressSimulater
{
 public:
  KeypressSimulater() = default;

  void InputAt(const std::string &input, const QPoint &pos);

  void PressAt(const QPoint &pos);

 protected:
  void KeyPress(int key);

  void MouseMove(int x, int y);

  void MouseLeftClick(int x, int y);

  void MouseRightClick(int x, int y);
};


}
