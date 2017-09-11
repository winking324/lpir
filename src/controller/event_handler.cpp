#include "event_handler.h"
#include <log4cplus/log4cplus.h>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QString>
#include <fstream>


using namespace lpir;


// 11:00:00
const static uint32_t kEnrollmentStopTs = 39600;
// 11:30:00
const static uint32_t kTimerStopTs = 41400;
// 11:29:40
const static uint32_t kInputPriceTs = 41380;
// 11:29:45
const static uint32_t kFareAndBidTs = 41385;
// 11:29:55
const static uint32_t kStartOKTs = 41395;
// 11:29:57
const static uint32_t kForceOkTs = 41397;


EventHandler::EventHandler(QObject *parent)
  : QObject(parent) {
  shot_timer_ = new QTimer(this);

  time_shoter_ = new Screenshoter(this);
  price_shoter_ = new Screenshoter(this);
  enrollment_shoter_ = new Screenshoter(this);

  if (!time_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Cannot init time tesseract api");
  }
  if (!price_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Cannot init price tesseract api");
  }
  if (!enrollment_shoter_->InitTessApi()) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Cannot init enrollment tesseract api");
  }

  connect(shot_timer_, SIGNAL(timeout()), this, SLOT(OnShotTimer()));
  shot_timer_->start(300);
}

void EventHandler::set_start_pos(const QPoint &pos) {
  start_pos_ = pos;
}

void EventHandler::set_end_pos(const QPoint &pos) {
  end_pos_ = pos;
}

void EventHandler::set_bid(uint32_t bid) {
  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "update 50s bid: %u", bid);
  bid_ = bid;
}

bool EventHandler::IsNeedPaint() {
  return is_selecting_ && is_shotting_ &&
      current_event_type_ != SELECT_EVENT_NONE &&
      current_event_type_ < SELECT_EVENT_PRICE_INPUT;
}

QRect EventHandler::SelectedRect() {
  return QRect(start_pos_, end_pos_);
}

bool EventHandler::OnSelectEvent(uint32_t event_type) {
  if (event_type == SELECT_EVENT_NONE) return false;

  if (is_selecting_) {
    LOG4CPLUS_ERROR_FMT(LOGGER_NAME,
                        "cannot select event %u, current %u selecting",
                        event_type, current_event_type_);
    return false;
  }

  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "start select event: %u", event_type);
  is_selecting_ = true;
  is_shotting_ = false;
  current_event_type_ = event_type;
  return true;
}

bool EventHandler::OnKeyPressEvent(uint32_t key_type, const QPoint &pos) {
  if (!is_selecting_) return false;

  if (key_type == Qt::Key_Escape) {
    ResetSelectEvent();
    emit OnRepaintEvent();
    return false;
  }

  if (key_type == Qt::Key_Space) {
    if (is_shotting_ || current_event_type_ >= SELECT_EVENT_PRICE_INPUT) {
      return true;
    }

    is_shotting_ = true;
    start_pos_ = pos;
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "start screenshotting at: (%d, %d)",
                       start_pos_.x(), start_pos_.y());
    return true;
  }
  return false;
}

bool EventHandler::OnKeyReleaseEvent(uint32_t key_type, const QPoint &pos) {
  if (!is_selecting_ || current_event_type_ == SELECT_EVENT_NONE) return false;

  if (key_type == Qt::Key_Space) {
    end_pos_ = pos;

    Screenshoter *shooter;
    switch (current_event_type_) {
      case SELECT_EVENT_TIME:
        shooter = time_shoter_;
      break;
      case SELECT_EVENT_PRICE:
        shooter = price_shoter_;
      break;
      case SELECT_EVENT_ENROLLMENT:
        shooter = enrollment_shoter_;
      break;
      case SELECT_EVENT_PRICE_INPUT:
        price_input_pos_ = end_pos_;
      break;
      case SELECT_EVENT_FARE:
        fare_pos_ = end_pos_;
      break;
      case SELECT_EVENT_BID:
        bid_pos_ = end_pos_;
      break;
      case SELECT_EVENT_OK:
        ok_pos_ = end_pos_;
      break;
      case SELECT_EVENT_CANCEL:
        cancel_pos_ = end_pos_;
      break;
      default:
      break;
    }

    if (current_event_type_ < SELECT_EVENT_PRICE_INPUT) {
      if (!is_shotting_ || shooter == nullptr) return false;
      shooter->SetStartPos(start_pos_);
      shooter->SetEndPos(end_pos_);
    }
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "stop screenshotting at: (%d, %d)",
                       end_pos_.x(), end_pos_.y());
    ResetSelectEvent();
  }

  return true;
}

bool EventHandler::OnMouseMoveEvent(const QPoint &pos) {
  if (!is_selecting_ || !is_shotting_) return false;
  end_pos_ = pos;
  return true;
}

void EventHandler::OnTimeShotted(const std::string &shotted_str) {
  QString time_str = QString::fromStdString(shotted_str);
  QStringList time_splited = time_str.split(':');

  uint32_t ts = 0;
  uint32_t last_ts = now_ts_;
  bool cvt_ret[3] = {false};
  if (time_splited.size() == 3) {
    ts += time_splited[0].toUInt(&cvt_ret[0]) * 3600;
    ts += time_splited[1].toUInt(&cvt_ret[1]) * 60;
    ts += time_splited[2].toUInt(&cvt_ret[2]);
  }

  if (!(cvt_ret[0]&cvt_ret[1]&cvt_ret[2])) {
    LOG4CPLUS_WARN_FMT(LOGGER_NAME, "cannot parse time shotted str: %s, "
                                    "now ts: %u", shotted_str.c_str(), now_ts_);
    ++now_ts_;
  } else {
    now_ts_ = std::max(now_ts_, ts);
  }

  if (now_ts_ > kTimerStopTs) {
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "stop timer shotting, now ts: %u", now_ts_);
    LogPriceTrend();
    price_shoter_->ClearSelectedRect();
    time_shoter_->ClearSelectedRect();
  }

  if (now_ts_ != last_ts) {
    emit OnTimeUpdate(now_ts_);
  }
}

void EventHandler::OnPriceShotted(const std::string &shotted_str) {
  bool cvt_ret = false;
  uint32_t price = QString::fromStdString(shotted_str).toUInt(&cvt_ret);
  if (!cvt_ret) {
    LOG4CPLUS_WARN_FMT(LOGGER_NAME, "cannot parse price shotted str: %s",
                       shotted_str.c_str());
    return;
  }

  uint32_t new_price = std::max(last_price_, price);
  if (new_price == last_price_) {
    return;
  }

  last_price_ = new_price;
  price_trend_.push_back(Price(now_ts_, last_price_));
}

void EventHandler::OnEnrollmentShotted(const std::string &shotted_str) {
  bool cvt_ret = false;
  uint32_t enrollment = QString::fromStdString(shotted_str).toUInt(&cvt_ret);
  if (!cvt_ret) {
    LOG4CPLUS_WARN_FMT(LOGGER_NAME, "cannot parse enrollment shotted str: %s",
                       shotted_str.c_str());
    return;
  }

  enrollment_ = std::max(enrollment_, enrollment);

  // if time > 11:00, stop enrollment shot
  if (now_ts_ > kEnrollmentStopTs) {
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "stop enrollment shotting, "
                                    "now enrollment: %u, "
                                    "now ts: %u", enrollment_, now_ts_);
    enrollment_shoter_->ClearSelectedRect();
  }
}

void EventHandler::OnOk() {
  QElapsedTimer timer;
  key_simulater_.InputAt(std::to_string(100), price_input_pos_);
  timer.start();
  while (timer.elapsed() < 500) QCoreApplication::processEvents();
  key_simulater_.PressAt(fare_pos_);
  key_simulater_.PressAt(bid_pos_);
}

void EventHandler::OnShotTimer() {
  if (is_finish_) return;

  std::string enrollment_str = enrollment_shoter_->Shot();
  if (!enrollment_str.empty()) OnEnrollmentShotted(enrollment_str);

  std::string price_str = price_shoter_->Shot();
  if (!price_str.empty()) OnPriceShotted(price_str);

  std::string time_str = time_shoter_->Shot();
  if (!time_str.empty()) OnTimeShotted(time_str);

  RunBid();
}

void EventHandler::ResetSelectEvent() {
  is_selecting_ = false;
  is_shotting_ = false;
  current_event_type_ = SELECT_EVENT_NONE;
  start_pos_ = QPoint();
  end_pos_ = QPoint();
}

void EventHandler::LogPriceTrend() {
  std::ofstream ofs("/tmp/price.csv");
  for (const Price &p : price_trend_) {
    ofs << p.ts << "," << p.money << "\n";
  }
  ofs.close();
}

void EventHandler::RunBid() {
  if (now_ts_ < kInputPriceTs) return;

  if (!is_price_input_) {
    key_simulater_.InputAt(std::to_string(bid_), price_input_pos_);
    is_price_input_ = true;
    // cannot use nanosleep here, it will block all events
    QElapsedTimer wait_for_input_timer;
    wait_for_input_timer.start();
    while (wait_for_input_timer.elapsed() < 2000)
      QCoreApplication::processEvents();
    return;
  }

  if (now_ts_ < kFareAndBidTs) return;

  if (!is_bidden_) {
    key_simulater_.PressAt(fare_pos_);
    key_simulater_.PressAt(bid_pos_);
    bidden_price_ = last_price_ + bid_;
    is_bidden_ = true;
    return;
  }

  if (now_ts_ < kStartOKTs) {
    if (bidden_price_ <= last_price_) {
      key_simulater_.PressAt(cancel_pos_);
      key_simulater_.InputAt("500", price_input_pos_);
      bidden_price_ = last_price_ + 700;
      return;
    }
    return;
  }

  if (bidden_price_ < last_price_ + 300 || now_ts_ >= kForceOkTs) {
    key_simulater_.PressAt(ok_pos_);
    is_finish_ = true;
    return;
  }
}
