#include "system_tray_menu.h"
#include <QApplication>


using namespace lpir;


SystemTrayMenu::SystemTrayMenu(QWidget *parent)
  : QMenu(parent) {

  options_action_ = new QAction("Options", this);
  exit_action_ = new QAction("Exit", this);

  addAction(options_action_);
  addAction(exit_action_);

  connect(options_action_, SIGNAL(triggered()), this, SLOT(OnOptions()));
  connect(exit_action_, SIGNAL(triggered()), this, SLOT(OnExit()));
}

SystemTrayMenu::~SystemTrayMenu() {

}

void SystemTrayMenu::OnExit() {
  QApplication::setQuitOnLastWindowClosed(true);
  parentWidget()->setAttribute(Qt::WA_QuitOnClose);
  parentWidget()->close();
}

void SystemTrayMenu::OnOptions() {
//  FMNConfigDlg *m_configDlg = new FMNConfigDlg(this);
//  m_configDlg->exec();
}



