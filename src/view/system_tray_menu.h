#pragma once
#include <QMenu>
#include <QAction>


namespace lpir {


class SystemTrayMenu : public QMenu
{
  Q_OBJECT

 public:
  SystemTrayMenu(QWidget *parent = nullptr);

  ~SystemTrayMenu();

 public slots:
  void OnExit();

  void OnOptions();

 private:
  QAction *exit_action_;
  QAction *options_action_;
};


}
