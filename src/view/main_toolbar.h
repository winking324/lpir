#pragma once
#include <QSize>
#include <QPoint>
#include <QToolBar>


namespace lpir {


class MainToolBar : public QToolBar
{
  Q_OBJECT

 public:
  MainToolBar(QWidget *parent = nullptr);

  ~MainToolBar();

 signals:
  void StartTimeScreenshot();

  void StartPriceScreenshot();

  void StartEnrollmentScreenshot();

  void StartSelectPrice();

  void StartSelectFare();

  void StartSelectBid();

  void StartSelectOk();

 private slots:
  void OnTimeScreenshot();

  void OnPriceScreenshot();

  void OnEnrollmentScreenshot();

  void OnSelectPrice();

  void OnSelectFare();

  void OnSelectBid();

  void OnSelectOk();

 private:

};


}

