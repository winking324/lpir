#pragma once
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


namespace lpir {


class Screenshoter : public QObject
{
  Q_OBJECT

 public:
  Screenshoter(QObject *parent);

  virtual ~Screenshoter();

  bool InitTessApi();

  QRect SelectedRect();

  void SetStartPos(const QPoint &pos);

  void SetEndPos(const QPoint &pos);

  void ClearSelectedRect();

  std::string Shot();

 protected:
  std::string DigitalImageRecognition(const QPixmap &pixmap);

  Pix *QImageToPix(const QImage &img);

  QImage PixToQImage(Pix *pix);

 private:
  tesseract::TessBaseAPI *tess_api_;
  QPixmap shotted_pixmap_;
  QRect selected_rect_;
};

}

