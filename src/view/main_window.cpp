#include "log_dock.h"
#include "main_window.h"
#include "main_menubar.h"
#include "main_toolbar.h"
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
  event_handler_ = new EventHandler(this);

  connect(toolbar_, SIGNAL(OnSelectEvent(uint32_t)), this, SLOT(OnSelectEvent(uint32_t)));
  connect(toolbar_, SIGNAL(OnBidEvent(QString)), this, SLOT(OnBidEvent(QString)));
  connect(toolbar_, SIGNAL(OnOk()), this, SLOT(OnOk()));
  connect(event_handler_, SIGNAL(OnRepaintEvent()), this, SLOT(repaint()));
  connect(event_handler_, SIGNAL(OnTimeUpdate(uint32_t)), this, SLOT(OnTimeUpdate(uint32_t)));
}


MainWindow::~MainWindow() {

}

void MainWindow::UpdatePreferences() {
}

void MainWindow::OnSelectEvent(uint32_t event_type) {
  if (event_handler_->OnSelectEvent(event_type)) {
    if (event_type < SELECT_EVENT_PRICE_INPUT) {
      setCursor(Qt::CrossCursor);
    }
  }
}

void MainWindow::OnBidEvent(QString bid_str) {
  bool cvt_ret = false;
  uint32_t bid = bid_str.toUInt(&cvt_ret);
  if (!cvt_ret) return;

  event_handler_->set_bid(bid);
}

void MainWindow::OnOk() {
  event_handler_->OnOk();
}

void MainWindow::OnTimeUpdate(uint32_t ts) {
  toolbar_->UpdateTime(ts);
}

void MainWindow::paintEvent(QPaintEvent *) {
  if (event_handler_->IsNeedPaint()) {
    QPainter window_painter(this);
    window_painter.setBrush(Qt::transparent);

    // Need to modify top pos, map from global to parent
    QRect draw_rect = event_handler_->SelectedRect();
    draw_rect.setTopLeft(mapFromGlobal(draw_rect.topLeft()));
    window_painter.drawRect(draw_rect);
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event_handler_->OnKeyPressEvent(event->key(), cursor().pos())) {
    setMouseTracking(true);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  setMouseTracking(false);
  if (event_handler_->OnKeyReleaseEvent(event->key(), cursor().pos())) {
    unsetCursor();
    repaint();
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent *) {
  event_handler_->OnMouseMoveEvent(cursor().pos());
}

void MainWindow::InitLogger() {
  using namespace log4cplus;
  SharedAppenderPtr log_appender(new FileAppender(LOGGER_FILE_PATH));
  std::unique_ptr<Layout> log_layout(new PatternLayout(LOGGER_FORMAT));
  log_appender->setLayout(std::move(log_layout));
  Logger::getInstance(LOGGER_NAME).addAppender(log_appender);
  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "lpir start running...");
}




