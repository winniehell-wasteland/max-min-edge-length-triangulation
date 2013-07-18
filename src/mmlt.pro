QT       += core svg
QT       -= gui

TARGET = mmlt

CONFIG   += console
CONFIG   -= app_bundle

# we come to that later
#CONFIG   -= qt

TEMPLATE = app

QMAKE_CXXFLAGS += -frounding-math -std=c++11 -O3

include(meta.pri)
include(boost.pri)
include(cgal.pri)
include(cplex.pri)
include(json.pri)

SOURCES += \
    $$files(cgal/*.cpp) \
    $$files(containers/*.cpp) \
    $$files(cplex/*.cpp) \
    $$files(intersection/*.cpp) \
    $$files(sat/*.cpp) \
    $$files(utils/*.cpp) \
    $$files(mmlt.cpp)

HEADERS += \
    $$files(cgal/*.h) \
    $$files(containers/*.h) \
    $$files(cplex/*.h) \
    $$files(intersection/*.h) \
    $$files(sat/*.h) \
    $$files(utils/*.h) \
    $$files(*.h)
