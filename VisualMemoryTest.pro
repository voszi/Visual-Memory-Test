#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T14:49:01
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = VisualMemoryTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    BackGroundWidget.cpp \
    GraphicsWidget.cpp \
    PixmapLabel.cpp \
    PlayerNameDlg.cpp \
    TimerWidget.cpp \
    TopList.cpp \
    SoundEffects.cpp \
    CardDealer.cpp \
    FloatingPopUp.cpp \
    DbHandler.cpp \
    HelpDialog.cpp

HEADERS  += MainWindow.h \
    BackGroundWidget.h \
    GraphicsWidget.h \
    PixmapLabel.h \
    PlayerNameDlg.h \
    TimerWidget.h \
    TopList.h \
    SoundEffects.h \
    CardDealer.h \
    FloatingPopUp.h \
    DbHandler.h \
    PlayerResult.h \
    HelpDialog.h

FORMS    += MainWindow.ui \
    BackGroundWidget.ui \
    GraphicsWidget.ui \
    PlayerNameDlg.ui \
    TopList.ui \
    HelpDialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = VisualMemoryTest.rc

TRANSLATIONS += \
    VisualMemoryTest_hu.ts \
    VisualMemoryTest_en.ts \
    VisualMemoryTest_de.ts \
    VisualMemoryTest_es.ts \
    VisualMemoryTest_ru.ts \

OTHER_FILES += \
    VisualMemoryTest_de.ts \
    VisualMemoryTest_en.ts \
    VisualMemoryTest_es.ts \
    VisualMemoryTest_hu.ts \
    VisualMemoryTest_ru.ts
