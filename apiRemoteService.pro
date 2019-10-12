TEMPLATE = app
QT += core widgets network gui
#QT -= gui
CONFIG += c++14
#CONFIG -= app_bundle
#CONFIG -= qt


TEMPLATE = app

SOURCES += \
        main.cpp \
        tcpserver.cpp

#LIBS += -lpthread
#LIBS += -lX11


HEADERS += \
    main.h \
    tcpserver.h
