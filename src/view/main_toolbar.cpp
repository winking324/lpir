#include "main_toolbar.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <QHBoxLayout>
#include <QToolButton>
#include <QIcon>


using namespace lpir;


MainToolBar::MainToolBar(QWidget *parent/* = nullptr*/)
  : QToolBar(parent) {
  setMovable(false);

  QWidget *tool_bar = new QWidget(this);
  QHBoxLayout *tool_bar_layout = new QHBoxLayout(tool_bar);

  QToolButton *time_btn = new QToolButton(tool_bar);
  time_btn->setIcon(QIcon(":/time.png"));

  QToolButton *price_btn = new QToolButton(tool_bar);
  price_btn->setIcon(QIcon(":/price.png"));

  QToolButton *enrollment_btn = new QToolButton(tool_bar);
  enrollment_btn->setIcon(QIcon(":/enrollment.png"));

  QToolButton *select_price_btn = new QToolButton(tool_bar);
  select_price_btn->setIcon(QIcon(":/select.png"));

  QToolButton *select_fare_btn = new QToolButton(tool_bar);
  select_fare_btn->setIcon(QIcon(":/select.png"));

  QToolButton *select_bid_btn = new QToolButton(tool_bar);
  select_bid_btn->setIcon(QIcon(":/select.png"));

  QToolButton *select_ok_btn = new QToolButton(tool_bar);
  select_ok_btn->setIcon(QIcon(":/select.png"));

  tool_bar_layout->addWidget(time_btn);
  tool_bar_layout->addWidget(price_btn);
  tool_bar_layout->addWidget(enrollment_btn);
  tool_bar_layout->addWidget(select_price_btn);
  tool_bar_layout->addWidget(select_fare_btn);
  tool_bar_layout->addWidget(select_bid_btn);
  tool_bar_layout->addWidget(select_ok_btn);
  tool_bar->setLayout(tool_bar_layout);
  addWidget(tool_bar);

  connect(time_btn, SIGNAL(released()), this, SLOT(OnTimeScreenshot()));
  connect(price_btn, SIGNAL(released()), this, SLOT(OnPriceScreenshot()));
  connect(enrollment_btn, SIGNAL(released()), this, SLOT(OnEnrollmentScreenshot()));

  connect(select_price_btn, SIGNAL(released()), this, SLOT(OnSelectPrice()));
  connect(select_fare_btn, SIGNAL(released()), this, SLOT(OnSelectFare()));
  connect(select_bid_btn, SIGNAL(released()), this, SLOT(OnSelectBid()));
  connect(select_ok_btn, SIGNAL(released()), this, SLOT(OnSelectOk()));
}

MainToolBar::~MainToolBar() {

}

void MainToolBar::OnTimeScreenshot() {
  emit StartTimeScreenshot();
}

void MainToolBar::OnPriceScreenshot() {
  emit StartPriceScreenshot();
}

void MainToolBar::OnEnrollmentScreenshot() {
  emit StartEnrollmentScreenshot();
}

void MainToolBar::OnSelectPrice() {
  emit StartSelectPrice();
}

void MainToolBar::OnSelectFare() {
  emit StartSelectFare();
}

void MainToolBar::OnSelectBid() {
  emit StartSelectBid();
}

void MainToolBar::OnSelectOk() {
  emit StartSelectOk();
}
