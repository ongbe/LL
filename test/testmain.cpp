#include <QApplication>

#include "test_lltablewidget.h"

int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    Test_LLTableWidget win;
    win.show();
    return app.exec();
}
