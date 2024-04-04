TEMPLATE = app
TARGET = novanav
QT += widgets webenginewidgets


CONFIG += c++17

CONFIG(debug, debug|release) {
    TARGET = $$TARGET-debug
    CONFIG += console
}

SOURCES += novanav.cpp

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0
    QMAKE_CFLAGS += -mmacosx-version-min=$$QMAKE_MACOSX_DEPLOYMENT_TARGET
    QMAKE_CXXFLAGS += -mmacosx-version-min=$$QMAKE_MACOSX_DEPLOYMENT_TARGET
}

DEFINES -= QT_NO_DEBUG
DEFINES += QT_GUI_LIB QT_CORE_LIB

