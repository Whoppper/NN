<<<<<<< HEAD
unix {
    INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include
}

QT -= gui
=======
QT += widgets core gui xml
>>>>>>> upstream/main

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        connection.cpp \
        main.cpp \
        mainwindow.cpp \
        net.cpp \
        neuron.cpp \
        newnetdialog.cpp \
        trainingdialog.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    connection.h \
    mainwindow.h \
    net.h \
    neuron.h \
    newnetdialog.h \
    trainingdialog.h

FORMS +=

DISTFILES += \
    links.txt \
    trainingFiles/trainingDataXOR.txt


