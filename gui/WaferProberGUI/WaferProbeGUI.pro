QT += qml quick multimedia

CONFIG += c++11

# PKGCONFIG += opencv
HEADERS += \
    src/backend.h \
    src/FileIO.h

SOURCES += src/main.cpp \
    src/backend.cpp \
    src/FileIO.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += ../../src/libWaferProb/include
INCLUDEPATH += ../../src/libGalil/include
INCLUDEPATH += ../../src/libZaber/include

unix:!macx{
    LIBS += -L../../build/lib -lWaferProb -lgclibo -lgclib
}

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
    # LIBS += -L../../build/lib -lWaferProb -L/Applications/gclib/dylib -lgclib.0 -lgclibo.0
    LIBS += -L../../src/build/Debug/lib -lWaferProb -L/Applications/gclib/dylib -lgclib.0 -lgclibo.0
}

#INCLUDEPATH += /usr/local/include
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
