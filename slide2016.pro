#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T18:34:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slide2016
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11 -w
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
SOURCES += main.cpp\
        mainwindow.cpp \
    edge.cpp \
    vertex.cpp \
    face.cpp \
    mesh.cpp \
    marge.cpp \
    transformation.cpp \
    makeMesh.cpp \
    subdivision.cpp \
    offset.cpp \
    stl.cpp \
    polyline.cpp \
    zipper.cpp \
    makePolyline.cpp \
    selection.cpp

HEADERS  += mainwindow.h \
    edge.h \
    vertex.h \
    face.h \
    mesh.h \
    transformation.h \
    merge.h \
    makeMesh.h \
    subdivison.h \
    offset.h \
    stl.h \
    polyline.h \
    zipper.h \
    makePolyline.h \
    selection.h \
    parameters.h

FORMS    += mainwindow.ui

linux{
    LIBS += -lglut -lGLU
}

mac{
    INCLUDEPATH = /usr/local/include
    LIBS += -framework GLUT -framework OpenGL
}

win32{
    LIBS += -lopengl32 -lglu32 -lglut32
    INCLUDEPATH = $$PWD/../include
}
