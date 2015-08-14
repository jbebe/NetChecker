QT += widgets network svg

win32:RC_ICONS += res/app.ico
macx:ICON = res/app.hqx

HEADERS     = \  
    include/NetCheckerWindow.hpp \
    include/Diagnostic.hpp \
	include/Globals.hpp \
    include/Translation.hpp \
    include/ProcessHandler.hpp \
    include/Privileges.hpp
SOURCES     = \ 
    main.cpp \
    src/Diagnostic.cpp \
    src/Globals.cpp \
    src/NetCheckerWindow.cpp \
    src/Privileges.cpp \
	src/ProcessHandler.cpp

# install
target.path = bin
INSTALLS += target

RESOURCES += \
    resource.qrc

CONFIG += c++11

QMAKE_CFLAGS_WARN_ON
QMAKE_CXXFLAGS_WARN_ON
