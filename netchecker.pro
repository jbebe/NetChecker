QT += widgets network

HEADERS     = \  
    include/NetCheckerWindow.hpp \
    include/Diagnostic.hpp \
    include/Translation.hpp
SOURCES     = \ 
    main.cpp \
    include/NetCheckerWindow.cpp

# install
target.path = bin
INSTALLS += target

RESOURCES += \
    resource.qrc
