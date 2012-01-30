#-------------------------------------------------
#
# Project created by QtCreator 2011-06-11T13:55:52
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS	= -O2

TARGET = pobr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    libs/qtimagefilterfactory.cpp \
    libs/qtimagefilter.cpp \
    libs/punchfilter.cpp \
    libs/convolutionfilter.cpp \
    imagecontroller.cpp

HEADERS  += mainwindow.h \
    libs/qtmatrix.h \
    libs/qtimagefilter.h \
    libs/punchfilter.h \
    libs/gaussfilter.h \
    libs/convolutionfilter.h \
    imagecontroller.h \
    libs/qtimagefilterfactory.h

FORMS    += mainwindow.ui
