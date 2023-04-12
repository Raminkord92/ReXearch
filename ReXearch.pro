QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32: RC_ICONS = programIcon.ico
TARGET = ReXearch

SOURCES += \
    Headers/Views/axlistview.cpp \
    Sources/DBManager.cpp \
    Sources/arxivequerygenerator.cpp \
    Sources/axdialog.cpp \
    Sources/Models/axqstringlistmodel.cpp \
    Sources/Models/axtablemodel.cpp \
    Sources/Views/axtableview.cpp \
    Sources/clickablelabel.cpp \
    Sources/Delegates/clickablelinkdelegate.cpp \
    Sources/dlgpaperdetails.cpp \
    Sources/listviewdelegate.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \

HEADERS += \
    Headers/DBManager.h \
    Headers/Common.h \
    Headers/Views/axlistview.h \
    Headers/arxivequerygenerator.h \
    Headers/axdialog.h \
    Headers/Models/axqstringlistmodel.h \
    Headers/Models/axtablemodel.h \
    Headers/Views/axtableview.h \
    Headers/clickablelabel.h \
    Headers/Delegates/clickablelinkdelegate.h \
    Headers/dlgpaperdetails.h \
    Headers/mainwindow.h \
    Headers/Delegates/listviewdelegate.h \
    ThirdParties/Libs/pugixml/headers/pugixml.hpp
    ThirdParties/Libs/pugixml/headers/pugiconfig.hpp

FORMS += \
    AdvanceOptions.ui \
    axdialog.ui \
    dlgpaperdetails.ui \
    mainwindow.ui

CONFIG(debug, debug|release) {
    LIBS += -LThirdParties/Libs/pugixml/debug -lpugixmld
} else {
    LIBS += -LThirdParties/Libs/pugixml/release -lpugixml
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lpugixml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lpugixmld
else:unix: LIBS += -L$$PWD/./ -lpugixml

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

RESOURCES += \
    resources.qrc
