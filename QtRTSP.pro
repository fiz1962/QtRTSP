QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    rtspthread.cpp

HEADERS += \
    mainwindow.h \
    rtspthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
    message("ffmpeg only runs in Release mode under windows")

    INCLUDEPATH += C:\Qt\ffmpeg\include
    LIBS += -LC:\Qt\ffmpeg\lib -lavformat -lavcodec -lavutil -lswresample -lswscale
}

linux:!android {
    INCLUDEPATH += /usr/include/ffmpeg/
    LIBS += -L$$PWD/../libs -lavformat -lavcodec -lavutil -lswresample -lswscale
}

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    message("Building for droid")
    QT += androidextras
    DISTFILES += \
        android-sources/AndroidManifest.xml

    INCLUDEPATH += $$PWD/../ffmpeg-android-maker/output/include/arm64-v8a/
    DEPENDPATH += $$PWD/../ffmpeg-android-maker/output/include/arm64-v8a

    LIBS += -L$$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/ -lavformat -lavutil -lavcodec -lavdevice -lavfilter -lswresample -lswscale

    ANDROID_EXTRA_LIBS = $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libavcodec.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libavdevice.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libavfilter.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libavformat.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libavutil.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libswresample.so $$PWD/../ffmpeg-android-maker/output/lib/arm64-v8a/libswscale.so
}

ANDROID_ABIS = arm64-v8a

RESOURCES +=
