#pragma once
#include <QRect>
#include <QTimer>
#include <QPoint>
#include <QObject>
#include <cinttypes>
#include "commons/definations.h"
#include "model/price.h"
#include "controller/screenshoter.h"
#include "controller/keypress_simulater.h"


namespace lpir {


class EventHandler : public QObject {
  Q_OBJECT

 public:
  EventHandler(QObject *parent);

  void set_start_pos(const QPoint &pos);

  void set_end_pos(const QPoint &pos);

  void set_bid(uint32_t bid);

  bool IsNeedPaint();

  QRect SelectedRect();

  bool OnSelectEvent(uint32_t event_type);

  bool OnKeyPressEvent(uint32_t key_type, const QPoint &pos);

  bool OnKeyReleaseEvent(uint32_t key_type, const QPoint &pos);

  bool OnMouseMoveEvent(const QPoint &pos);

  void OnTimeShotted(const std::string &shotted_str);

  void OnPriceShotted(const std::string &shotted_str);

  void OnEnrollmentShotted(const std::string &shotted_str);

  void OnOk();

 signals:
  void OnRepaintEvent();

  void OnTimeUpdate(uint32_t ts);

 public slots:
  void OnShotTimer();

//  void OnFinished();

 private:
  void ResetSelectEvent();

  void LogPriceTrend();

  void RunBid();

 private:
  bool is_selecting_ = false;
  bool is_shotting_ = false;

  bool is_finish_ = false;
  bool is_bidden_ = false;
  bool is_price_input_ = false;
  bool is_verified_ = false;
  uint32_t current_event_type_ = SELECT_EVENT_NONE;

  uint32_t now_ts_ = 0;
  uint32_t enrollment_ = 0;
  uint32_t last_price_ = 0;
  uint32_t bid_ = 1000;
  uint32_t bidden_price_ = 0;

  PriceTrend price_trend_;

  QPoint start_pos_;
  QPoint end_pos_;
  QPoint price_input_pos_;
  QPoint fare_pos_;
  QPoint bid_pos_;
  QPoint ok_pos_;
  QPoint cancel_pos_;

  QTimer *shot_timer_;

  Screenshoter *time_shoter_;
  Screenshoter *price_shoter_;
  Screenshoter *enrollment_shoter_;
  KeypressSimulater key_simulater_;
};


}
