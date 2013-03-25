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
        const int MaxHeight = 30;
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
    setFixedHeight(LL::Constant::MaxHeight);
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

bool LLIpEditPrivate::inputIp(const QString& strIp)
{
    if(strIp.split('.').size() != Count)
    {
        return false;
    }

    const QStringList kItemList = strIp.split('.');
    for(int i = 0; i < kItemList.size(); ++i)
    {
        bool ok = false;
        int value = kItemList.at(i).toInt(&ok);
        if(!ok || (0 <= value && value <= 255))
        {
            return false;
        }
    }
    return true;
}

bool LLIpEditPrivate::outputIp()
{
    _ip.clear();
    for(int i = 0; i < Count; ++i)
    {
        if(_edits[i]->text().isEmpty())
        {
            return false;
        }
        _ip += _edits[i]->text() + ".";
    }
    _ip = _ip.left(_ip.length() - 1);
    return true;
}


/*!
 *
 */
void LLIpEditPrivate::setIp(const QString &strIp)
{
    if(inputIp(strIp))
    {
        _ip = strIp;
    }
}

QString LLIpEditPrivate::ip()
{
    if(outputIp())
    {
        return _ip;
    }
    else
    {
        return QString();
    }
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

QString LLIpEdit::ip()
{
    Q_D(LLIpEdit);
    return d->ip();
}
void LLIpEdit::setIp(const QString& strIp)
{
    Q_D(LLIpEdit);
    d->setIp(strIp);
}
