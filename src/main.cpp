#include <QApplication>
#include "view/main_window.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icon.png"));

  lpir::MainWindow w;
  w.show();
  return a.exec();
}
