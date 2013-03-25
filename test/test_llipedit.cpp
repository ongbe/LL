#include "test_llipedit.h"
#include "../src/llipedit.h"

#include <QVBoxLayout>

Test_LLIpEdit::Test_LLIpEdit(QWidget *parent) :
    QWidget(parent)
{
    LLIpEdit* edit = new LLIpEdit(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(edit);
    setLayout(mainLayout);
}
