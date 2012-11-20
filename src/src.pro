#-------------------------------------------------
#
# Project created by QtCreator 2012-10-26T15:53:13
#
#-------------------------------------------------

QT       += core svg
QT       -= gui

TARGET = mmt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# qxt library
CONFIG  += qxt
QXT     += core gui

# CGAL library
LIBS += -lCGAL
QMAKE_CXXFLAGS += -frounding-math

# JSON library
LIBS += -ljson

SOURCES += main.cpp \
    json_parser.cpp \
    controller.cpp \
    graph.cpp \
    intersection/intersection_graph.cpp \
    utils.cpp

HEADERS += \
    json_parser.h \
    controller.h \
    config.h \
    graphviz.h \
    graph.h \
    utils.h \
    intersection/arr_intersection_algorithm.h \
    intersection/sweep_intersection_algorithm.h \
    intersection/intersection_graph.h \
    intersection/sweep_line_visitor.h \
    intersection/naive_intersection_algorithm.h \
    parameters.h

CONFIG(debug, debug|release) {
    message("debug mode")
}
else {
    message("release mode")
    DEFINES += NDEBUG
}
