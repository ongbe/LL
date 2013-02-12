TEMPLATE = app
TARGET = ll

HEADERS += ./src/lteheader.h \
   	   ./src/lltablewidget.h \
           ./src/lltablewidget_p.h

SOURCES += ./src/lteheader.cpp \
	   ./src/lltablewidget.cpp \
           ./test/testmain.cpp
