#include "llipedit.h"
#include "llipedit_p.h"

#include <QVBoxLayout>

/**********************************/
/*!       LLIpEditPrivate        !*/
/**********************************/
LLIpEditPrivate::LLIpEditPrivate(LLIpEdit *parent):QWidget(parent)
{

}

/*!
 *
 */
void LLIpEditPrivate::setIp(const QString &strIp)
{
    _ip = strIp;
}

QString LLIpEditPrivate::ip() const
{
    return _ip;
}

/**********************************/
/*!            LLIpEdit          !*/
/**********************************/
LLIpEdit::LLIpEdit(QWidget *parent) :
    QWidget(parent)
{
    d_ptr = new LLIpEditPrivate(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(d_ptr);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
}
