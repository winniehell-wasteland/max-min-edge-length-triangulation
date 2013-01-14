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

# CGAL library
LIBS += -lCGAL
QMAKE_CXXFLAGS += -frounding-math

include(cplex.pri)

SOURCES += main.cpp \
    json_parser.cpp \
    controller.cpp \
    graph.cpp \
    intersection/intersection_graph.cpp \
    containers/segment_container.cpp \
    mmt_utils.cpp \
    logger.cpp \
    containers/convex_hull.cpp \
    cplex/sat_problem.cpp \
    orders.cpp \
    segment.cpp

HEADERS += \
    json_parser.h \
    controller.h \
    config.h \
    graphviz.h \
    graph.h \
    intersection/arr_intersection_algorithm.h \
    intersection/sweep_intersection_algorithm.h \
    intersection/intersection_graph.h \
    intersection/sweep_line_visitor.h \
    intersection/naive_intersection_algorithm.h \
    parameters.h \
    containers/segment_container.h \
    mmt_utils.h \
    logger.h \
    containers/convex_hull.h \
    cplex/sat_problem.h \
    intersection/types.h \
    containers/types.h \
    segment.h \
    orders.h \
    index_types.h \
    containers/sorted_vector.h \
    assertions.h

CONFIG(debug, debug|release) {
    message("debug mode")
}
else {
    message("release mode")
    DEFINES += NDEBUG
}

OTHER_FILES += \
    cplex.pri
