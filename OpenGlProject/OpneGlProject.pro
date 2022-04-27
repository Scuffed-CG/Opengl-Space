QT       += core gui opengl 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cg1_4dline.cpp \
    cg1_4dvector.cpp \
    main.cpp \
    mainview.cpp \
    mainwindow.cpp

HEADERS += \
    Util.h \
    cg1_4dline.h \
    cg1_4dvector.h \
    mainview.h \
    mainwindow.h


LIBS += -lopengl32 -lglu32 \
    -LC:\Qt\5.15.2\mingw81_64\lib\libQt5OpenGL.a \
    -LC:\Qt\5.15.2\mingw81_64\lib\libQt5OpenGLExtensions.a


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
