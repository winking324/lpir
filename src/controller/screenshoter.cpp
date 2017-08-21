#include "screenshoter.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <Carbon/Carbon.h>
#include <QtMacExtras/QtMac>
#include <iostream>


using namespace lpir;


Screenshoter::Screenshoter(QObject *parent)
  : QObject(parent) {
  shot_timer_ = new QTimer(parent);
  connect(shot_timer_, SIGNAL(timeout()), this, SLOT(Shot()));
}

Screenshoter::~Screenshoter() {
  tess_api_->End();
}

bool Screenshoter::InitTessApi() {
  tess_api_ = new tesseract::TessBaseAPI();
  if (tess_api_->Init(NULL, "eng")) {
    tess_api_->End();
    return false;
  }

  return true;
}

QRect Screenshoter::SelectedRect() {
  return selected_rect_;
}

void Screenshoter::SetStartPos(const QPoint &pos) {
  selected_rect_.setTopLeft(pos);
}

void Screenshoter::SetEndPos(const QPoint &pos) {
  selected_rect_.setBottomRight(pos);
}

void Screenshoter::StartShot() {
  shot_timer_->start(300);
}

void Screenshoter::Shot() {
  clock_t start = clock();
  CGRect cg_selected_rect;
  cg_selected_rect.origin.x    = selected_rect_.x();
  cg_selected_rect.origin.y    = selected_rect_.y();
  cg_selected_rect.size.width  = selected_rect_.width();
  cg_selected_rect.size.height = selected_rect_.height();

  CGImageRef cg_window_image = CGWindowListCreateImage(
        cg_selected_rect, kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID, kCGWindowImageBestResolution);

  shotted_pixmap_ = QtMac::fromCGImageRef(cg_window_image);
  CGImageRelease(cg_window_image);
  clock_t shot_end = clock();
  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "screenshot spend %f sec",
                     static_cast<double>(shot_end - start) / CLOCKS_PER_SEC);

  std::string shot_str = DigitalImageRecognition(shotted_pixmap_);
  shot_str.erase(std::remove(shot_str.begin(), shot_str.end(), '\n'), shot_str.end());
  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "image recognition result: %s, spend %f sec",
                     shot_str.c_str(),
                     static_cast<double>(clock() - shot_end) / CLOCKS_PER_SEC);
}

std::string Screenshoter::DigitalImageRecognition(const QPixmap &pixmap) {
  QImage image = pixmap.toImage();
  Pix *pix = QImageToPix(image);

  tess_api_->SetImage(pix);
  char *str = tess_api_->GetUTF8Text();
  std::string ir_str(str);
  delete []str;
  pixDestroy(&pix);
  tess_api_->Clear();
  return ir_str;
}

Pix *Screenshoter::QImageToPix(const QImage &img) {
  QImage q_img = img.rgbSwapped();
  int width = q_img.width();
  int height = q_img.height();
  int depth = q_img.depth();
  int wpl = q_img.bytesPerLine() / 4;

  Pix * pixs;
  pixs = pixCreate(width, height, depth);
  pixSetWpl(pixs, wpl);
  pixSetColormap(pixs, NULL);
  l_uint32 *datas = pixs->data;

  for (int y = 0; y < height; y++) {
    l_uint32 *lines = datas + y * wpl;
    QByteArray a((const char*)q_img.scanLine(y), q_img.bytesPerLine());
    for (int j = 0; j < a.size(); j++) {
      *((l_uint8 *)lines + j) = a[j];
    }
  }

  const qreal toDPM = 1.0 / 0.0254;
  int resolutionX = q_img.dotsPerMeterX() / toDPM;
  int resolutionY = q_img.dotsPerMeterY() / toDPM;

  if (resolutionX < 300) resolutionX = 300;
  if (resolutionY < 300) resolutionY = 300;
  pixSetResolution(pixs, resolutionX, resolutionY);

  return pixEndianByteSwapNew(pixs);
}

QImage Screenshoter::PixToQImage(Pix *pixImage) {
  int width = pixGetWidth(pixImage);
  int height = pixGetHeight(pixImage);
  int depth = pixGetDepth(pixImage);
  int bytesPerLine = pixGetWpl(pixImage) * 4;
  l_uint32 * datas = pixGetData(pixEndianByteSwapNew(pixImage));

  QImage::Format format;
  if (depth == 1)
    format = QImage::Format_Mono;
  else if (depth == 8)
    format = QImage::Format_Indexed8;
  else
    format = QImage::Format_RGB32;

  QImage result((uchar*)datas, width, height, bytesPerLine, format);

  // Set resolution
  l_int32 	xres, yres;
  pixGetResolution(pixImage, &xres, &yres);
  const qreal toDPM = 1.0 / 0.0254;
  result.setDotsPerMeterX(xres * toDPM);
  result.setDotsPerMeterY(yres * toDPM);

  // Handle palette
  QVector<QRgb> _bwCT;
  _bwCT.append(qRgb(255,255,255));
  _bwCT.append(qRgb(0,0,0));

  QVector<QRgb> _grayscaleCT(256);
  for (int i = 0; i < 256; i++)  {
    _grayscaleCT[i] = qRgb(i, i, i);
  }
  switch (depth) {
    case 1:
      result.setColorTable(_bwCT);
    break;
    case 8:
      result.setColorTable(_grayscaleCT);
    break;
    default:
      result.setColorTable(_grayscaleCT);
  }

  if (result.isNull()) {
    static QImage none(0,0,QImage::Format_Invalid);
    qDebug("Invalid format!!!\n");
    return none;
  }

  return result.rgbSwapped();
}


