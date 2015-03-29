QT       += opengl
QT       += core gui

TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    vecteur.cpp \
    Camera.cpp \
    API/CSommet.cpp \
    API/CSegment.cpp \
    API/CPointAnnexe.cpp \
    API/CPoint3f.cpp \
    API/CGraphe.cpp \
    API/CDonneesGraphe.cpp \
    API/CData2i.cpp \
    API/CArc.cpp \
    wagon.cpp \
    train.cpp \
    gcoll.cpp \
    sdlglutils.cpp

LIBS+=-lGLU -lGL
LIBS+=-lglut -lSDL -lSDL_image

HEADERS += \
    vecteur.h \
    Camera.h \
    API/CSommet.h \
    API/CSegment.h \
    API/CPointAnnexe.h \
    API/CPoint3f.h \
    API/consts.h \
    API/CGraphe.h \
    API/CDonneesGraphe.h \
    API/CData2i.h \
    API/CArc.h \
    wagon.h \
    train.h \
    gcoll.h \
    sdlglutils.h

FORMS +=
