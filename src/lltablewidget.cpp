#include "lltablewidget.h"
#include "lltablewidget_p.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QDebug>

#undef D_VAR
#undef D_INT
#undef D_COLOR
#define D_VAR(TYPE,NAME,VALUE) const TYPE NAME = VALUE;
#define D_INT(NAME,VALUE) D_VAR(int,NAME,VALUE)
#define D_COLOR(NAME,VALUE) D_VAR(QColor,NAME,VALUE)

namespace LL
{
    namespace Constant
    {
        D_INT(RowHeight,16)
        D_INT(MaxRowHeight,40)
        D_INT(DeltaRowHeight,3)
        D_INT(WheelDelta,20)
    }

    namespace Palette
    {
        D_COLOR(VerticalLine,QColor(177,177,177))
    }
}

class LLTableWidgetRow::PrivateData
{
public:
    explicit PrivateData():bgColor(Qt::white){}
    void addItem(LLTableWidgetItem* item)
    {
        items.push_back(item);
    }

    void clear()
    {
        qDeleteAll(items);
    }

    int count() const
    {
        return items.size();
    }

    QColor bgColor;
    QList<LLTableWidgetItem*> items;

};

class LLTableWidgetItem::PrivateData
{
public:
    void setText(const QString& strText)
    {
        _text = strText;
    }

    QString text() const
    {
        return _text;
    }

    void setData(const QVariant& var)
    {
        _data = var;
    }

    QVariant data() const
    {
        return _data;
    }

    QString _text;
    QVariant _data;
};

/*!
 * impl of LLTableWidgetRow
 */
LLTableWidgetRow::LLTableWidgetRow()
{
    d_data.reset(new PrivateData);
}

void LLTableWidgetRow::addItem(LLTableWidgetItem *item)
{
    d_data->addItem(item);
}

void LLTableWidgetRow::clear()
{
    d_data->clear();
}

int LLTableWidgetRow::count() const
{
    return d_data->count();
}

QList<LLTableWidgetItem*> LLTableWidgetRow::items()
{
    return d_data->items;
}

void LLTableWidgetRow::setBackgroundColor(const QColor &clr)
{
    d_data->bgColor = clr;
}

QColor LLTableWidgetRow::backgroundColor() const
{
    return d_data->bgColor;
}

/*!
 * impl of LLTableWidgetItem
 */
LLTableWidgetItem::LLTableWidgetItem(LLTableWidgetRow *parent)
{
    d_data.reset(new PrivateData);
}

void LLTableWidgetItem::setText(const QString &strText)
{
    d_data->setText(strText);
}

QString LLTableWidgetItem::text() const
{
    return d_data->text();
}

void LLTableWidgetItem::setData(const QVariant &var)
{
    d_data->setData(var);
}

QVariant LLTableWidgetItem::data() const
{
    return d_data->data();
}

/***********************************************/
/*!               LLTableWidgetSpiri          !*/
/***********************************************/
LLTableWidgetSpirit::LLTableWidgetSpirit(LLTableWidgetPrivate *parent):QWidget(parent),_parent(parent)
{
    _totalHeight = 0;
    _currentYPos = 0;
    _currentIndex = -1;
    _pressedYPos = 0;
    _isEnabledRowColors = true;
    _isCtrlPressed = false;
    _rowHeight = LL::Constant::RowHeight;

    setFocusPolicy(Qt::StrongFocus);
}

LLTableWidgetSpirit::~LLTableWidgetSpirit()
{
    clear();
}

/*!
 * private utility functions
 */
void LLTableWidgetSpirit::notifyHeightChanged()
{
    int totalHeight = _totalHeight - height();
    if(totalHeight >= 0)
    {
        emit heightChanged(_totalHeight - height());
    }
}

void LLTableWidgetSpirit::scrollUp()
{
    _currentYPos -= LL::Constant::WheelDelta;
    if(_currentYPos < 0)
    {
        _currentYPos = 0;
    }

    emit setBarValue(_currentYPos);
    update();
}

void LLTableWidgetSpirit::scrollDown()
{
    _currentYPos += LL::Constant::WheelDelta;
    if(_currentYPos >= (_totalHeight - height()))
    {
        _currentYPos = _totalHeight - height();
    }

    emit setBarValue(_currentYPos);
    update();
}

void LLTableWidgetSpirit::enlargeRowHeight()
{
    _rowHeight += LL::Constant::DeltaRowHeight;
    if(_rowHeight > LL::Constant::MaxRowHeight)
    {
        _rowHeight = LL::Constant::MaxRowHeight;
    }
    update();
}

void LLTableWidgetSpirit::shrinkRowHeight()
{
    _rowHeight -= LL::Constant::DeltaRowHeight;
    if(_rowHeight < LL::Constant::RowHeight)
    {
        _rowHeight = LL::Constant::RowHeight;
    }
    update();
}

void LLTableWidgetSpirit::fire_clickItem()
{
    if(_pressedYPos >= 0 && _pressedYPos < _totalHeight)
    {
        int itemIndex = _pressedYPos/_rowHeight;
        if(itemIndex >= 0 && itemIndex < _rows.size())
        {
            LLTableWidgetRow* row = _rows[itemIndex];
            emit _parent->q_ptr->rowClicked(row);
        }
    }
}

/*!
 * reimpl
 */
void LLTableWidgetSpirit::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        _pressedYPos = _currentYPos + e->pos().y();
        fire_clickItem();
        update();
    }
}

void LLTableWidgetSpirit::resizeEvent(QResizeEvent *)
{
    notifyHeightChanged();
}

void LLTableWidgetSpirit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawRows(&painter);
    drawVerticalLines(&painter);
}

void LLTableWidgetSpirit::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() & Qt::ControlModifier)
    {
        _isCtrlPressed = true;
    }

    switch(e->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        scrollUp();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        scrollDown();
        break;
    }
}

void LLTableWidgetSpirit::keyReleaseEvent(QKeyEvent *)
{
    _isCtrlPressed = false;
}

void LLTableWidgetSpirit::wheelEvent(QWheelEvent *e)
{
    if(_isCtrlPressed)
    {
        if(e->delta() > 0)
        {
            enlargeRowHeight();
        }
        else
        {
            shrinkRowHeight();
        }
    }
    else
    {
        if(e->delta() < 0)
        {
            scrollDown();
        }
        else
        {
            scrollUp();
        }
    }
}

/*!
 * painting functions
 */
void LLTableWidgetSpirit::drawVerticalLines(QPainter *painter)
{
    painter->save();
    painter->setPen(LL::Palette::VerticalLine);

    qreal initX = 0;
    for(int col = 0; col < _columnWidth.size(); ++col)
    {
        initX += _columnWidth.at(col);
        QPointF topPoint(initX,0);
        QPointF bottomPoint(initX,height());
        painter->drawLine(topPoint,bottomPoint);
    }

    painter->restore();
}

void LLTableWidgetSpirit::drawRows(QPainter *painter)
{
    painter->save();

    int initY = 0;
    for(int rowIndex = 0; rowIndex < _rows.size(); rowIndex++)
    {
        initY = rowIndex * _rowHeight;

        if((_currentYPos - _rowHeight) > initY)
        {
            continue;
        }
        else if(initY > (_currentYPos + height()))
        {
            break;
        }

        LLTableWidgetRow* row = _rows[rowIndex];
        drawOneRow(painter,row,(initY - _currentYPos));
    }

    painter->restore();
}

void LLTableWidgetSpirit::drawOneRow(QPainter *painter, LLTableWidgetRow *row, int initY)
{
    if(_columnWidth.isEmpty())
    {
        return;
    }

    painter->save();

    drawRowBg(painter,row,initY);
    drawPressedRowBg(painter,row,initY);

    qreal initX = 0;
    QList<LLTableWidgetItem*> item = row->items();
    int columnCount = _parent->_header->labels().size();
    for(int colIndex = 0; colIndex < columnCount; colIndex++)
    {
        drawItem(painter, item.at(colIndex), initX, _columnWidth.at(colIndex), initY);
        initX += _columnWidth.at(colIndex);
    }

    painter->restore();
}

void LLTableWidgetSpirit::drawRowBg(QPainter *painter, LLTableWidgetRow *row, int initY)
{
    painter->save();

    QRect rowRect(QPoint(0,initY),QPoint(width(),initY + _rowHeight));

    if(!_isEnabledRowColors)
    {
        painter->setPen(QColor(199,199,199));
        painter->setBrush(Qt::white);
    }
    else
    {
        painter->setPen(Qt::white);
        painter->setBrush(row->backgroundColor());
    }
    painter->drawRect(rowRect);

    painter->restore();
}

void LLTableWidgetSpirit::drawPressedRowBg(QPainter *painter, LLTableWidgetRow *row, int initY)
{
    int logicalY = _pressedYPos - _currentYPos;
    if(logicalY >= initY && logicalY <= (initY + _rowHeight))
    {
        QRect rowRect(QPoint(0, initY),QPoint(width(), initY + _rowHeight));
        painter->save();

        if(_isEnabledRowColors)
        {
            painter->setPen(QColor(221,221,221));
            painter->setBrush(QColor(221,221,221));
        }
        else
        {
            painter->setPen(Qt::blue);
            painter->setBrush(Qt::blue);
        }
        painter->setOpacity(0.6);
        painter->drawRect(rowRect);
        painter->restore();
    }
}

void LLTableWidgetSpirit::drawItem(QPainter *painter, LLTableWidgetItem *item, qreal initX, qreal colWidth, int initY)
{
    painter->save();
    painter->setPen(Qt::black);
    QPoint topLeft(initX, initY);
    QPoint bottomRight(initX + colWidth, initY + _rowHeight);
    QRect itemRect(topLeft,bottomRight);

    QFont textFont;
    textFont.setPointSize(_rowHeight/2);
    textFont.setFamily("Times New Roman");
    painter->setFont(textFont);
    painter->drawText(itemRect,item->text(),Qt::AlignVCenter|Qt::AlignHCenter);
    painter->restore();
}

/*!
 * public functions
 */

void LLTableWidgetSpirit::addRow(LLTableWidgetRow* row)
{
    _rows.push_back(row);
    _totalHeight = _rows.size() * _rowHeight;
    update();
    notifyHeightChanged();
}

int LLTableWidgetSpirit::rowCount() const
{
    return _rows.size();
}
void LLTableWidgetSpirit::clear()
{
    qDeleteAll(_rows);
    _rows.clear();
}

void LLTableWidgetSpirit::setEnableRowColors(bool enable)
{
    _isEnabledRowColors = enable;
    update();
}

bool LLTableWidgetSpirit::isEnabledRowColors() const
{
    return _isEnabledRowColors;
}

/*!
 * public slots
 */
void LLTableWidgetSpirit::setCurrentYPos(int yPos)
{
    _currentYPos = yPos;
    update();
}

int LLTableWidgetSpirit::currentYPos() const
{
    return _currentYPos;
}

void LLTableWidgetSpirit::setColumnWidth(QVector<qreal> columnWidth)
{
    _columnWidth = columnWidth;
    update();
}

/***********************************************/
/*!               LLTableWidget               !*/
/***********************************************/

LLTableWidget::LLTableWidget(QWidget *parent) :
    QWidget(parent)
{
    d_ptr = new LLTableWidgetPrivate(this);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(d_ptr);
    setLayout(layout);
}

/*!
 * public functions
 */
void LLTableWidget::setLabels(const QStringList& labels)
{
    Q_D(LLTableWidget);
    d->setLabels(labels);
}

QStringList LLTableWidget::labels() const
{
    Q_D(const LLTableWidget);
    return d->labels();
}

void LLTableWidget::addRow(LLTableWidgetRow* row)
{
    Q_D(LLTableWidget);
    d->addRow(row);
}

int LLTableWidget::rowCount() const
{
    Q_D(const LLTableWidget);
    return d->rowCount();
}
void LLTableWidget::clear()
{
    Q_D(LLTableWidget);
    d->clear();
}

void LLTableWidget::clearContents()
{
    Q_D(LLTableWidget);
    d->clearContents();
}

void LLTableWidget::setEnableRowColors(bool enable)
{
    Q_D(LLTableWidget);
    d->setEnableRowColors(enable);
}

bool LLTableWidget::isEnabledRowColors() const
{
    Q_D(const LLTableWidget);
    return d->isEnabledRowColors();
}
void LLTableWidget::setAutoAdjust(bool adjust)
{
    Q_D(LLTableWidget);
    d->setAutoAdjust(adjust);
}

bool LLTableWidget::isAutoAdjust() const
{
    Q_D(const LLTableWidget);
    return d->isAutoAdjust();
}


/***********************************************/
/*!              LLTableWidgetPrirvate        !*/
/***********************************************/
LLTableWidgetPrivate::LLTableWidgetPrivate(LLTableWidget *parent):QWidget(parent),q_ptr(parent)
{
    initWidgets();
    initSettings();
    initConnections();
}

LLTableWidgetPrivate::~LLTableWidgetPrivate()
{
    clear();
}

/*!
 * private utility functions
 */
void LLTableWidgetPrivate::initWidgets()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    QScrollArea* area = new QScrollArea(this);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setFrameShape(QFrame::NoFrame);
    area->setWidgetResizable(true);
    mainLayout->addWidget(area);

    QWidget* areaWidget = new QWidget(this);
    QVBoxLayout* areaWidgetLayout = new QVBoxLayout;
    areaWidgetLayout->setContentsMargins(0,0,0,0);
    areaWidgetLayout->setSpacing(0);
    _header = new LteHeader(this);
    areaWidgetLayout->addWidget(_header);
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(0,0,0,0);
    bottomLayout->setSpacing(0);
    _spirit = new LLTableWidgetSpirit(this);
    _bar = new QScrollBar(Qt::Vertical,this);
    bottomLayout->addWidget(_spirit);
    bottomLayout->addWidget(_bar);
    areaWidgetLayout->addLayout(bottomLayout);
    areaWidget->setLayout(areaWidgetLayout);
    area->setWidget(areaWidget);
    setLayout(mainLayout);
}

void LLTableWidgetPrivate::initSettings()
{
}

void LLTableWidgetPrivate::initConnections()
{
    connect(_spirit,SIGNAL(setBarValue(int)),_bar,SLOT(setValue(int)));
    connect(_bar,SIGNAL(valueChanged(int)),_spirit,SLOT(setCurrentYPos(int)));
    connect(_spirit,SIGNAL(heightChanged(int)),this,SLOT(changeHeight(int)));
    connect(_header,SIGNAL(columnWidthChanged(QVector<qreal>)),_spirit,SLOT(setColumnWidth(QVector<qreal>)));
}

/*!
 * private slots
 */
void LLTableWidgetPrivate::changeHeight(int totalHeight)
{
    _bar->setMaximum(totalHeight);
}

/*!
 * public functions
 */
void LLTableWidgetPrivate::addRow(LLTableWidgetRow* row)
{
    _spirit->addRow(row);
}

int LLTableWidgetPrivate::rowCount() const
{
    return _spirit->rowCount();
}

void LLTableWidgetPrivate::clear()
{
    _header->clear();
    _spirit->clear();
}

void LLTableWidgetPrivate::clearContents()
{
    _spirit->clear();
}

void LLTableWidgetPrivate::setEnableRowColors(bool enable)
{
    _spirit->setEnableRowColors(enable);
}

bool LLTableWidgetPrivate::isEnabledRowColors() const
{
    return _spirit->isEnabledRowColors();
}

void LLTableWidgetPrivate::setAutoAdjust(bool adjust)
{
    _header->setAutoAdjust(adjust);
}

bool LLTableWidgetPrivate::isAutoAdjust() const
{
    return _header->isAutoAdjust();
}

void LLTableWidgetPrivate::setLabels(const QStringList &labels)
{
    _header->setLabels(labels);
}

QStringList LLTableWidgetPrivate::labels() const
{
    return _header->labels();
}

/*!
 * reimpl
 */
void LLTableWidgetPrivate::keyPressEvent(QKeyEvent *e)
{
    _spirit->keyPressEvent(e);
}

void LLTableWidgetPrivate::keyReleaseEvent(QKeyEvent *e)
{
    _spirit->keyReleaseEvent(e);
}
