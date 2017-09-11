#pragma once
#include <map>
#include <cinttypes>
#include <QtGui>
#include <QPoint>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "system_tray_menu.h"
#include "controller/event_handler.h"


namespace lpir {


class LogDock;
class LogDockAppender;
class MainMenuBar;
class MainToolBar;


class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0);

  ~MainWindow();

 protected:
  virtual void paintEvent(QPaintEvent *event) override;

  virtual void keyPressEvent(QKeyEvent *event) override;

  virtual void keyReleaseEvent(QKeyEvent *event) override;

  virtual void mouseMoveEvent(QMouseEvent *event) override;

 public slots:
  void UpdatePreferences();

  void OnSelectEvent(uint32_t event_type);

  void OnBidEvent(QString bid_str);

  void OnOk();

  void OnTimeUpdate(uint32_t ts);

 private:
  void InitLogger();

 private:
  MainMenuBar *menu_;
  MainToolBar *toolbar_;
  QSystemTrayIcon *tray_icon_;
  SystemTrayMenu  *tray_menu_;
  QVBoxLayout *layout_;
  EventHandler *event_handler_;
};


}
