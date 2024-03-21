TEMPLATE = app
TARGET = novanav
QT += widgets webenginewidgets

# C++ config macOS
CONFIG += c++17

SOURCES += novanav.cpp

# /usr/local/Cellar/qt/6.6.2_1/bin/qmake novanav.pro

INCLUDEPATH += /usr/local/opt/qt/include
INCLUDEPATH += /usr/local/opt/qt/include/QtWebEngineWidgets

# biblioteca de Qt 6 WebEngineWidgets

#LIBS += -L/usr/local/opt/qt/lib -lQt5WebEngineWidgets

LIBS += -L/usr/local/opt/qt/lib -lQt6WebEngineWidget

