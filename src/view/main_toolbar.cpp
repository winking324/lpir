#include "main_toolbar.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <QHBoxLayout>
#include <QToolButton>
#include <QValidator>
#include <QIcon>
#include <QMenu>


using namespace lpir;


MainToolBar::MainToolBar(QWidget *parent/* = nullptr*/)
  : QToolBar(parent) {
  setMovable(false);

  QIcon check_icon(":/uncheck.png");
  QWidget *tool_bar = new QWidget(this);
  QHBoxLayout *tool_bar_layout = new QHBoxLayout(tool_bar);
  tool_bar_layout->setMargin(0);
//  tool_bar_layout->setSpacing(0);

  QToolButton *info_btn = new QToolButton(tool_bar);
  QMenu *info_menu = new QMenu(tool_bar);
  select_time_ = new QAction(check_icon, "Select Timer", tool_bar);
  select_price_ = new QAction(check_icon, "Select Price", tool_bar);
  select_enrollment_ = new QAction(check_icon, "Select Enrollment", tool_bar);
  info_menu->insertAction(nullptr, select_enrollment_);
  info_menu->insertAction(select_enrollment_, select_price_);
  info_menu->insertAction(select_price_, select_time_);
  info_btn->setMenu(info_menu);
  info_btn->setIcon(QIcon(":/select_rect.png"));
  info_btn->setIconSize(QSize(24, 24));
  info_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  info_btn->setPopupMode(QToolButton::InstantPopup);
  tool_bar_layout->addWidget(info_btn);

  QToolButton *input_btn = new QToolButton(tool_bar);
  QMenu *input_menu = new QMenu(tool_bar);
  select_price_input_ = new QAction(check_icon, "Select Price Input", tool_bar);
  select_fare_ = new QAction(check_icon, "Select Fare", tool_bar);
  select_bid_ = new QAction(check_icon, "Select Bid", tool_bar);
  select_ok_ = new QAction(check_icon, "Select OK", tool_bar);
  select_cancle_ = new QAction(check_icon, "Select Cancel", tool_bar);
  input_menu->insertAction(nullptr, select_cancle_);
  input_menu->insertAction(select_cancle_, select_ok_);
  input_menu->insertAction(select_ok_, select_bid_);
  input_menu->insertAction(select_bid_, select_fare_);
  input_menu->insertAction(select_fare_, select_price_input_);
  input_btn->setMenu(input_menu);
  input_btn->setIcon(QIcon(":/select_point.png"));
  input_btn->setIconSize(QSize(24, 24));
  input_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  input_btn->setPopupMode(QToolButton::InstantPopup);
  tool_bar_layout->addWidget(input_btn);

  time_label_ = new QLabel("00:00:00", tool_bar);
  tool_bar_layout->addWidget(new QLabel("System Time:", tool_bar));
  tool_bar_layout->addWidget(time_label_);

  bid_edit_ = new QLineEdit(tool_bar);
  bid_edit_->setText("1000");
  bid_edit_->setDisabled(true);
  bid_edit_->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), tool_bar));
  tool_bar_layout->addWidget(new QLabel("45s Bid:", tool_bar));
  tool_bar_layout->addWidget(bid_edit_);

  lock_bid_ = new QPushButton("Unlock", tool_bar);
  tool_bar_layout->addWidget(lock_bid_);

  QPushButton *ok_btn = new QPushButton("OK", tool_bar);
  tool_bar_layout->addWidget(ok_btn);

  tool_bar->setLayout(tool_bar_layout);
  addWidget(tool_bar);

  connect(select_time_, SIGNAL(triggered()), this, SLOT(OnTimeScreenshot()));
  connect(select_price_, SIGNAL(triggered()), this, SLOT(OnPriceScreenshot()));
  connect(select_enrollment_, SIGNAL(triggered()), this, SLOT(OnEnrollmentScreenshot()));

  connect(select_price_input_, SIGNAL(triggered()), this, SLOT(OnSelectPrice()));
  connect(select_fare_, SIGNAL(triggered()), this, SLOT(OnSelectFare()));
  connect(select_bid_, SIGNAL(triggered()), this, SLOT(OnSelectBid()));
  connect(select_ok_, SIGNAL(triggered()), this, SLOT(OnSelectOk()));
  connect(select_cancle_, SIGNAL(triggered()), this, SLOT(OnSelectCancel()));
  connect(bid_edit_, SIGNAL(textChanged(QString)), this, SIGNAL(OnBidEvent(QString)));
  connect(lock_bid_, SIGNAL(clicked()), this, SLOT(OnLockEvent()));
  connect(ok_btn, SIGNAL(clicked()), this, SIGNAL(OnOk()));
}

MainToolBar::~MainToolBar() {

}

void MainToolBar::ChangeButtonState(uint32_t event_type, bool status) {
  QIcon icon(":/check.png");
  if (!status) {
    icon = QIcon(":/uncheck.png");
  }

  switch (event_type) {
    case SELECT_EVENT_TIME:
      select_time_->setIcon(icon);
    break;
    case SELECT_EVENT_PRICE:
      select_price_->setIcon(icon);
    break;
    case SELECT_EVENT_ENROLLMENT:
      select_enrollment_->setIcon(icon);
    break;
    case SELECT_EVENT_PRICE_INPUT:
      select_price_input_->setIcon(icon);
    break;
    case SELECT_EVENT_FARE:
      select_fare_->setIcon(icon);
    break;
    case SELECT_EVENT_BID:
      select_bid_->setIcon(icon);
    break;
    case SELECT_EVENT_OK:
      select_ok_->setIcon(icon);
    break;
    case SELECT_EVENT_CANCEL:
      select_cancle_->setIcon(icon);
    break;
    default:
    break;
  }
}

void MainToolBar::UpdateTime(uint32_t ts) {
  QString ts_str = QString("%1:%2:%3").arg(ts/3600).arg(ts%3600/60).arg(ts%60);
  time_label_->setText(ts_str);
}

void MainToolBar::OnTimeScreenshot() {
  ChangeButtonState(SELECT_EVENT_TIME, true);
  emit OnSelectEvent(SELECT_EVENT_TIME);
}

void MainToolBar::OnPriceScreenshot() {
  ChangeButtonState(SELECT_EVENT_PRICE, true);
  emit OnSelectEvent(SELECT_EVENT_PRICE);
}

void MainToolBar::OnEnrollmentScreenshot() {
  ChangeButtonState(SELECT_EVENT_ENROLLMENT, true);
  emit OnSelectEvent(SELECT_EVENT_ENROLLMENT);
}

void MainToolBar::OnSelectPrice() {
  ChangeButtonState(SELECT_EVENT_PRICE_INPUT, true);
  emit OnSelectEvent(SELECT_EVENT_PRICE_INPUT);
}

void MainToolBar::OnSelectFare() {
  ChangeButtonState(SELECT_EVENT_FARE, true);
  emit OnSelectEvent(SELECT_EVENT_FARE);
}

void MainToolBar::OnSelectBid() {
  ChangeButtonState(SELECT_EVENT_BID, true);
  emit OnSelectEvent(SELECT_EVENT_BID);
}

void MainToolBar::OnSelectOk() {
  ChangeButtonState(SELECT_EVENT_OK, true);
  emit OnSelectEvent(SELECT_EVENT_OK);
}

void MainToolBar::OnSelectCancel() {
  ChangeButtonState(SELECT_EVENT_CANCEL, true);
  emit OnSelectEvent(SELECT_EVENT_CANCEL);
}

void MainToolBar::OnLockEvent() {
  if (bid_edit_->isEnabled()) {
    bid_edit_->setDisabled(true);
    lock_bid_->setText("Unlock");
  } else {
    bid_edit_->setDisabled(false);
    lock_bid_->setText("Lock");
  }
}
