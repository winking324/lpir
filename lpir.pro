#---------------------------------------------------------------------
# PART 1: General Setting
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
OBJECTS_DIR = .objs
QT += core gui macextras

#---------------------------------------------------------------------
# PART 2: target, library, flags
TARGET = bin/lpir
INCLUDEPATH += src
#INCLUDEPATH += /usr/local/include /usr/local/opt/opencv3/include
#LIBS += -L/usr/lib -L/usr/local/lib -L/usr/local/opt/opencv3/lib
#LIBS += -llog4cplus -lopencv_core -lopencv_highgui -lopencv_imgproc
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/lib -L/usr/local/lib
LIBS += -llog4cplus -ltesseract -llept -v
LIBS += -framework Carbon
DEFINES += QT_DEPRECATED_WARNINGS

#---------------------------------------------------------------------
# PART 3: Clean
QMAKE_CLEAN += $(TARGET) $(QMAKE_TARGET) bin/lpir

#---------------------------------------------------------------------
# PART 4: Input
SOURCES += \
    src/main.cpp \
    src/controller/keypress_simulater.cpp
SOURCES += \
    src/view/main_window.cpp \
    src/view/about_dialog.cpp \
    src/view/main_menubar.cpp \
    src/view/preferences_dialog.cpp \
    src/view/main_toolbar.cpp \
    src/view/log_dock.cpp \
    src/view/system_tray_menu.cpp
SOURCES += \
    src/controller/log_dock_appender.cpp \
    src/controller/screenshoter.cpp
HEADERS  += \
    src/commons/definations.h \
    src/controller/keypress_simulater.h
HEADERS  += \
    src/view/main_window.h \
    src/view/about_dialog.h \
    src/view/main_menubar.h \
    src/view/preferences_dialog.h \
    src/view/main_toolbar.h \
    src/view/log_dock.h \
    src/view/system_tray_menu.h
HEADERS  += \
    src/controller/log_dock_appender.h \
    src/controller/screenshoter.h
RESOURCES += \
    resource/resource.qrc
ICON = icon.icns
