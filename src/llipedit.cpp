#include "llipedit.h"
#include "llipedit_p.h"

#include <QVBoxLayout>
#include <QValidator>

/**********************************/
/*!       LLIpEditPrivate        !*/
/**********************************/
namespace LL
{
    namespace Constant
    {
        const int RoundRadius = 4;
        const int MaxWidth = 140;
    }

    namespace Palette
    {
        const QColor Background = Qt::white;
        const QColor Border = QColor(176,176,176);
    }
}

LLIpEditPrivate::LLIpEditPrivate(LLIpEdit *parent):QWidget(parent)
{
    initEdits();

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->addWidget(_edits[First]);
    mainLayout->addWidget(createLabel());
    mainLayout->addWidget(_edits[Second]);
    mainLayout->addWidget(createLabel());
    mainLayout->addWidget(_edits[Third]);
    mainLayout->addWidget(createLabel());
    mainLayout->addWidget(_edits[Fourth]);
    setLayout(mainLayout);

    setFixedWidth(LL::Constant::MaxWidth);
}

/*!
 * reimpl
 */
void LLIpEditPrivate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawBg(&painter);
}

void LLIpEditPrivate::drawBg(QPainter *painter)
{
    painter->save();
    painter->setRenderHints(QPainter::HighQualityAntialiasing);
    painter->setPen(LL::Palette::Border);
    painter->setBrush(LL::Palette::Background);
    painter->drawRoundedRect(rect(), LL::Constant::RoundRadius, LL::Constant::RoundRadius);
    painter->restore();
}

QLabel* LLIpEditPrivate::createLabel()
{
    QLabel* label = new QLabel(this);
    label->setText(".");
    label->setAttribute(Qt::WA_TranslucentBackground);
    return label;
}

void LLIpEditPrivate::initEdits()
{
    for(int i = 0; i < Count; ++i)
    {
        QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        QRegExpValidator *pValidator = new QRegExpValidator(regExp, this);
        _edits[i] = new QLineEdit(this);
        _edits[i]->setFixedWidth(30);
        _edits[i]->setMaxLength(3);
        _edits[i]->setValidator(pValidator);
        _edits[i]->setFrame(false);
//        _edits[i]->setStyleSheet("border: 1px solid rgb(167,167,167);border-radius: 3px;");
    }
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

QString LLIpEdit::ip() const
{
    Q_D(const LLIpEdit);
    return d->ip();
}
void LLIpEdit::setIp(const QString& strIp)
{
    Q_D(LLIpEdit);
    d->setIp(strIp);
}
