QT       += core svg
QT       -= gui

TARGET = generator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -frounding-math -std=c++11 -O3

include(meta.pri)
include(boost.pri)
include(cgal.pri)
include(json.pri)

SOURCES += \
    $$files(cgal/*.cpp) \
    $$files(containers/*.cpp) \
    $$files(utils/*.cpp) \
    $$files(generator.cpp)

SOURCES -= $$files(utils/controller.cpp)

HEADERS += \
    $$files(cgal/*.h) \
    $$files(containers/*.h) \
    $$files(utils/*.h) \
    $$files(*.h)

SOURCES -= $$files(utils/controller.h)
