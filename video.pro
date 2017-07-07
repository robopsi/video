#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T16:20:46
#
#-------------------------------------------------

QT       += core gui sql network multimedia multimediawidgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videoPlayer
TEMPLATE = app

CONFIG += link_pkgconfig
GST_VERSION = 1.0

PKGCONFIG += \
    gstreamer-$$GST_VERSION \
    gstreamer-base-$$GST_VERSION \
    gstreamer-audio-$$GST_VERSION \
    gstreamer-video-$$GST_VERSION \
    gstreamer-pbutils-$$GST_VERSION


INCLUDEPATH +=$$PWD base
include(base/base.pri)

INCLUDEPATH +=$$PWD video
include(video/video.pri)

INCLUDEPATH +=$$PWD bottomWidget
include(bottomWidget/bottomWidget.pri)

INCLUDEPATH +=$$PWD player
include(player/player.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    global_value.cpp \
    inotifythread.cpp \
    ueventthread.cpp

HEADERS  += mainwindow.h \
    global_value.h \
    inotifythread.h \
    ueventthread.h

RESOURCES += \
    res_main.qrc \
    res_video.qrc \
    qml.qrc \
    res_music.qrc
