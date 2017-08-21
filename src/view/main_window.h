#pragma once
#include <QtGui>
#include <QPoint>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "system_tray_menu.h"
#include "controller/screenshoter.h"


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

  void StartTimeScreenshot();

  void StartPriceScreenshot();

  void StartEnrollmentScreenshot();

  void StartSelectPrice();

  void StartSelectFare();

  void StartSelectBid();

  void StartSelectOk();

 private:
  void InitLogger();

 private:
  bool is_shotting_;
  bool is_selecting_;
  uint32_t current_selecting_;
  QPoint start_pos_;

  QPoint select_pos_;

  MainMenuBar *menu_;
  MainToolBar *toolbar_;
  QSystemTrayIcon *tray_icon_;
  SystemTrayMenu  *tray_menu_;
  QVBoxLayout *layout_;

  Screenshoter *time_shoter_;
  Screenshoter *price_shoter_;
  Screenshoter *enrollment_shoter_;
};


}
