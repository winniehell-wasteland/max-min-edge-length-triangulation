QT       += core svg
QT       -= gui

TARGET = mmlt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# CGAL library
LIBS += -lCGAL
QMAKE_CXXFLAGS += -frounding-math -std=c++11 -O3

include(cplex.pri)

SOURCES += \
    $$files(cgal/*.cpp) \
    $$files(containers/*.cpp) \
    $$files(cplex/*.cpp) \
    $$files(intersection/*.cpp) \
    $$files(utils/*.cpp) \
    $$files(*.cpp)

HEADERS += \
    $$files(cgal/*.h) \
    $$files(containers/*.h) \
    $$files(cplex/*.h) \
    $$files(intersection/*.h) \
    $$files(utils/*.h) \
    $$files(*.h)

CONFIG(debug, debug|release) {
    message("debug mode")
}
else {
    message("release mode")
    DEFINES += NDEBUG
}
