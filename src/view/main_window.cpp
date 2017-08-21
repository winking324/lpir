#include "log_dock.h"
#include "main_window.h"
#include "main_menubar.h"
#include "main_toolbar.h"
#include "controller/keypress_simulater.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <QIcon>
#include <QPainter>


using namespace lpir;


enum CurrentSelectingType {
  SELECT_NONE         = 0,
  SELECT_TIME         = 1,
  SELECT_PRICE        = 2,
  SELECT_ENROLLMENT   = 3,
  SELECT_INPUT        = 4,
};


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {
  is_shotting_ = false;
  is_selecting_ = false;
  current_selecting_ = SELECT_NONE;

  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground);

  setWindowState(Qt::WindowMaximized);

  tray_icon_ = new QSystemTrayIcon(this);
  tray_menu_ = new SystemTrayMenu(this);
  tray_icon_->setIcon(QIcon(":/icon.png"));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->show();
  menu_ = new MainMenuBar(this);

  toolbar_ = new MainToolBar(this);
  addToolBar(toolbar_);

  InitLogger();
  time_shoter_ = new Screenshoter(this);
  price_shoter_ = new Screenshoter(this);
  enrollment_shoter_ = new Screenshoter(this);

  if (!time_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_FMT(LOGGER_NAME, "Cannot init time tesseract api");
  }
  if (!price_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_FMT(LOGGER_NAME, "Cannot init price tesseract api");
  }
  if (!enrollment_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_FMT(LOGGER_NAME, "Cannot init enrollment tesseract api");
  }

  connect(toolbar_, SIGNAL(StartTimeScreenshot()), this, SLOT(StartTimeScreenshot()));
  connect(toolbar_, SIGNAL(StartPriceScreenshot()), this, SLOT(StartPriceScreenshot()));
  connect(toolbar_, SIGNAL(StartEnrollmentScreenshot()), this, SLOT(StartEnrollmentScreenshot()));

  connect(toolbar_, SIGNAL(StartSelectPrice()), this, SLOT(StartSelectPrice()));
  connect(toolbar_, SIGNAL(StartSelectFare()), this, SLOT(StartSelectFare()));
  connect(toolbar_, SIGNAL(StartSelectBid()), this, SLOT(StartSelectBid()));
  connect(toolbar_, SIGNAL(StartSelectOk()), this, SLOT(StartSelectOk()));
}


MainWindow::~MainWindow() {

}

void MainWindow::UpdatePreferences() {
}

void MainWindow::StartTimeScreenshot() {
  LOG4CPLUS_INFO_STR(LOGGER_NAME, "start time screenshotting");
  is_shotting_ = true;
  is_selecting_ = false;
  current_selecting_ = SELECT_TIME;
  setCursor(Qt::CrossCursor);
}

void MainWindow::StartPriceScreenshot() {
  LOG4CPLUS_INFO_STR(LOGGER_NAME, "start price screenshotting");
  is_shotting_ = true;
  is_selecting_ = false;
  current_selecting_ = SELECT_PRICE;
  setCursor(Qt::CrossCursor);
}

void MainWindow::StartEnrollmentScreenshot() {
  LOG4CPLUS_INFO_STR(LOGGER_NAME, "start enrollment screenshotting");
  is_shotting_ = true;
  is_selecting_ = false;
  current_selecting_ = SELECT_ENROLLMENT;
  setCursor(Qt::CrossCursor);
}

void MainWindow::StartSelectPrice() {
  is_shotting_ = false;
  is_selecting_ = false;
  current_selecting_ = SELECT_INPUT;
}

void MainWindow::StartSelectFare() {
  is_shotting_ = false;
  is_selecting_ = false;
  current_selecting_ = SELECT_INPUT;
}

void MainWindow::StartSelectBid() {
  is_shotting_ = false;
  is_selecting_ = false;
  current_selecting_ = SELECT_INPUT;
}

void MainWindow::StartSelectOk() {
  is_shotting_ = false;
  is_selecting_ = false;
  current_selecting_ = SELECT_INPUT;
}

void MainWindow::paintEvent(QPaintEvent *) {
  if (is_shotting_ && is_selecting_) {
    QPainter window_painter(this);
    window_painter.setBrush(Qt::transparent);

    // Need to modify top pos, map from global to parent
    QRect draw_rect = time_shoter_->SelectedRect();
    draw_rect.setTopLeft(mapFromGlobal(draw_rect.topLeft()));
    window_painter.drawRect(draw_rect);
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (!is_shotting_ && current_selecting_ == SELECT_NONE) return;

  if (event->key() == Qt::Key_Escape) {
    is_shotting_ = false;
    is_selecting_ = false;
    repaint();
  }

  if (event->key() == Qt::Key_Space) {
    setMouseTracking(true);
    if (current_selecting_ == SELECT_INPUT) return;

    if (is_selecting_) return;
    is_selecting_ = true;

    start_pos_ = cursor().pos();

    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "start screenshotting at: (%d, %d)",
                       start_pos_.x(), start_pos_.y());
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  setMouseTracking(false);
  QPoint p = cursor().pos();
  if (current_selecting_ == SELECT_INPUT) {
    select_pos_ = cursor().pos();
    KeypressSimulater s;
    s.TestKeypress(select_pos_.x(), select_pos_.y());
    return;
  }

  if (!is_shotting_) return;
  if (!is_selecting_) return;

  if (event->key() == Qt::Key_Space) {
    is_shotting_ = false;
    is_selecting_ = false;
    unsetCursor();
    repaint();

    switch (current_selecting_) {
      case SELECT_NONE:
      break;
      case SELECT_TIME:
        time_shoter_->SetStartPos(start_pos_);
        time_shoter_->SetEndPos(p);
        time_shoter_->StartShot();
      break;
      case SELECT_PRICE:
        price_shoter_->SetStartPos(start_pos_);
        price_shoter_->SetEndPos(p);
        price_shoter_->StartShot();
      break;
      case SELECT_ENROLLMENT:
        enrollment_shoter_->SetStartPos(start_pos_);
        enrollment_shoter_->SetEndPos(p);
        enrollment_shoter_->StartShot();
      break;
      default:
      break;
    }
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "stop screenshotting at: (%d, %d)",
                       p.x(), p.y());
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent *) {
  if (!is_shotting_) return;

  if (is_selecting_) {
    repaint();
  }
}

void MainWindow::InitLogger() {
  using namespace log4cplus;
  SharedAppenderPtr log_appender(new FileAppender(LOGGER_FILE_PATH));
  std::unique_ptr<Layout> log_layout(new PatternLayout(LOGGER_FORMAT));
  log_appender->setLayout(std::move(log_layout));
  Logger::getInstance(LOGGER_NAME).addAppender(log_appender);
  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "lpir start running...");
}




