#pragma once
#include <QList>
#include <QLabel>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QPushButton>
#include "commons/definations.h"


namespace lpir {


class MainToolBar : public QToolBar
{
  Q_OBJECT

 public:
  MainToolBar(QWidget *parent = nullptr);

  ~MainToolBar();

  void ChangeButtonState(uint32_t event_type, bool status);

  void UpdateTime(uint32_t ts);

 signals:
  void OnSelectEvent(uint32_t event_type);

  void OnBidEvent(QString bid_str);

  void OnOk();

 private slots:
  void OnTimeScreenshot();

  void OnPriceScreenshot();

  void OnEnrollmentScreenshot();

  void OnSelectPrice();

  void OnSelectFare();

  void OnSelectBid();

  void OnSelectOk();

  void OnSelectCancel();

  void OnLockEvent();

 private:
  QAction *select_time_;
  QAction *select_price_;
  QAction *select_enrollment_;

  QAction *select_price_input_;
  QAction *select_fare_;
  QAction *select_bid_;
  QAction *select_ok_;
  QAction *select_cancle_;

  QLabel *time_label_;
  QLineEdit *bid_edit_;
  QPushButton *lock_bid_;
};


}

