TEMPLATE = app
TARGET = novanav
QT += widgets webenginewidgets
QT += core gui declarative
CONFIG(debug, debug|release) {
    TARGET = ${TARGET}d
    CONFIG += console
}

# C++ config macOS
# CONFIG += c++17
CONFIG += c++17 qmltypes qt

SOURCES += novanav.cpp

# /usr/local/Cellar/qt/6.6.2_1/bin/qmake novanav.pro

INCLUDEPATH += /usr/local/opt/qt/include
INCLUDEPATH += /usr/local/opt/qt/include/QtWebEngineWidgets

# biblioteca de Qt 6 WebEngineWidgets

#LIBS += -L/usr/local/opt/qt/lib -lQt5WebEngineWidgets

LIBS += -L/usr/local/opt/qt/lib -lQt6WebEngineWidgets

#QT += quick widgets core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
    "${workspaceFolder}/**" \
    "/usr/local/Cellar/qt/6.6.2_1/lib/QtWidgets.framework/Versions/A/Headers" \
    "/usr/local/Cellar/qt/6.6.2_1/lib/QtGui.framework/Versions/A/Headers"

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0
    QMAKE_CFLAGS += -mmacosx-version-min=$$QMAKE_MACOSX_DEPLOYMENT_TARGET
    QMAKE_CXXFLAGS += -mmacosx-version-min=$$QMAKE_MACOSX_DEPLOYMENT_TARGET
}

DEFINES -= QT_NO_DEBUG
DEFINES += QT_GUI_LIB QT_CORE_LIB

macx:CONFIG(release, debug|release): LIBS += -L/usr/local/opt/qt/lib -lQt6WebEngineWidgets

# Rutas de inclusión del compilador
QMAKE_CXXFLAGS += -I$$[QT_INSTALL_HEADERS]
INCLUDEPATH += $$[QT_INSTALL_HEADERS]

# Agregar las rutas de inclusión y defines de VSCode
INCLUDEPATH += \
    "${workspaceFolder}/**" \
    "/usr/local/Cellar/qt/6.6.2_1/lib/QtWidgets.framework/Versions/A/Headers" \
    "/usr/local/Cellar/qt/6.6.2_1/lib/QtGui.framework/Versions/A/Headers"

