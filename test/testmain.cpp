#include <QApplication>

#include "test_lltablewidget.h"
#include "test_llipedit.h"

int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    Test_LLIpEdit win;
    win.show();
    return app.exec();
}
