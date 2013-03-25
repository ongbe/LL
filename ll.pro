TEMPLATE = app
TARGET = ll

HEADERS += ./src/lteheader.h \
   	   ./src/lltablewidget.h \
           ./src/lltablewidget_p.h \
    src/llipedit.h \
    src/llipedit_p.h \
    test/test_llipedit.h

SOURCES += ./src/lteheader.cpp \
	   ./src/lltablewidget.cpp \
           ./test/testmain.cpp \
    src/llipedit.cpp \
    test/test_llipedit.cpp

HEADERS += ./test/test_lltablewidget.h

SOURCES += ./test/test_lltablewidget.cpp



