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
    $$files(*.h) \
    cgal/segment.h

CONFIG(debug, debug|release) {
    message("debug mode")
}
else {
    message("release mode")
    DEFINES += NDEBUG
}

OTHER_FILES += \
    cplex.pri
