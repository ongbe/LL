#ifndef LTEHEADER_H
#define LTEHEADER_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#undef DEFINE_VAR
#define DEFINE_VAR(TYPE, NAME, VALUE) const TYPE NAME = VALUE;

template <typename ForwardIterator>
void LteDeleteAll(ForwardIterator begin,ForwardIterator end)
{
    while(begin != end)
    {
        delete *begin;
        *begin = NULL;
        ++begin;
    }
}

template <typename Container>
void LteDeleteAll(Container& container)
{
    LteDeleteAll(container.begin(),container.end());
    container.clear();
}

namespace Def
{
    namespace Color
    {
        DEFINE_VAR(QColor,Header_StartBackground,QColor(255,255,255))
        DEFINE_VAR(QColor,Header_MiddleBackground,QColor(241,241,241))
        DEFINE_VAR(QColor,Header_StopBackground,QColor(225,225,225))

        DEFINE_VAR(QColor,Header_NormalTitle,Qt::black)
        DEFINE_VAR(QColor,Header_HoverTitle,Qt::black)
        DEFINE_VAR(QColor,Header_Separator,QColor(132,132,132))

        DEFINE_VAR(QColor,Header_HoverStartBackground,QColor(69,151,212))
        DEFINE_VAR(QColor,Header_HoverStopBackground,QColor(92,191,212))

        DEFINE_VAR(QColor,Header_PressedStartBackground,QColor(61,197,255))
        DEFINE_VAR(QColor,Header_PressedStopBackground,QColor(56,182,235))

        DEFINE_VAR(QColor,Real_StartBackground,QColor(201,201,201))
        DEFINE_VAR(QColor, Real_StopBackground,QColor(189,189,189))

        DEFINE_VAR(QColor,Real_ExpandBox_Start,QColor(241,241,241))
        DEFINE_VAR(QColor,Real_ExpandBox_Stop,QColor(211,211,211))
        DEFINE_VAR(QColor,Real_ExpandBox_Frame,QColor(131,131,131))
        DEFINE_VAR(QColor,Real_ExpandBox_Handle,QColor(65,65,65))

        DEFINE_VAR(QColor,Real_FoundTextColor,QColor(243,179,101))

//        DEFINE_VAR(QColor Real_ItemPressed_StartBackground , QColor(99,222,255))
//        DEFINE_VAR(QColor Real_ItemPressed_StopBackground , QColor(99,222,255))

        DEFINE_VAR(QColor,Real_ItemPressed_StartBackground,QColor(55,195,255).light())
        DEFINE_VAR(QColor,Real_ItemPressed_StopBackground,QColor(55,195,255))

        DEFINE_VAR(QColor,Real_ItemPressed_Frame,QColor(245,213,51))


        DEFINE_VAR(QColor,Real_Item_StartLightBackground , QColor(250,250,250))
        DEFINE_VAR(QColor,Real_Item_StopLightBackground , QColor(250,250,250))

        DEFINE_VAR(QColor,Real_Item_StartDarkBackground , QColor(221,221,221))
        DEFINE_VAR(QColor,Real_Item_StopDarkBackground , QColor(221,221,221))

        DEFINE_VAR(QColor,Real_Separator,Header_Separator)
        DEFINE_VAR(QColor,Real_Found_BrushColor,QColor(245,197,85))
        DEFINE_VAR(QColor,Real_Found_PenColor,QColor(245,197,85))
        DEFINE_VAR(QColor,Real_PenColor,QColor(145,145,145))

        DEFINE_VAR(QColor,Real_ItemPressed_Pen,Qt::black)
    }

    namespace Constant
    {
        DEFINE_VAR(int,Header_Height,22)
        DEFINE_VAR(qreal,Header_HoverOpacity,0.3)
        DEFINE_VAR(qreal,Real_FoundOpacity,0.6)
        DEFINE_VAR(int,Header_ExtraSpace,4)

        DEFINE_VAR(int,Real_RowHeight,18)
        DEFINE_VAR(QSize,Real_IconSize,QSize(Real_RowHeight - 3,Real_RowHeight - 3))
        DEFINE_VAR(int,Real_ExpandBoxExtraSpace,3)
        DEFINE_VAR(QSize,Real_ExpandBoxSize,QSize(10,10))
        DEFINE_VAR(int,Real_BoxTextSpace,7)
        DEFINE_VAR(int,Real_TextPointSize,11)
        DEFINE_VAR(int,Real_IconTextSpace,3)
        DEFINE_VAR(QString,Real_ThreeDot,"...")

        DEFINE_VAR(int,Real_GeoUpdateInterval,1000)

        DEFINE_VAR(int,Real_MaxRowCountForMouseMoveUpdate,5000)
        DEFINE_VAR(int,DefaultItemCountLimit,100000)
    }

    namespace Type
    {
        typedef int Real;
        #define ContainerType QVector
    }
}

class LteHeader : public QWidget
{
    struct HeaderItem
    {
        explicit HeaderItem(const QString& text,qreal width,bool pressed):_text(text),_width(width),_pressed(pressed),_hover(false){}
        explicit HeaderItem(const QString& text):_text(text),_width(0),_pressed(false),_hover(false){}
        QString _text;
        int _index;
        qreal _width;
        qreal _startXPos;
        qreal _separatorXPos;
        bool _pressed;
        bool _hover;
        QRectF _dragRect;
        QRectF _thisRect;
    };

Q_OBJECT
public:
    explicit LteHeader(QWidget* parent = 0);
    virtual ~LteHeader(){ clear(); }

public:
    void setLabels(const QStringList& labels);
    QStringList labels() const;

    void clear();

    void setAutoAdjust(bool adjust);
    bool isAutoAdjust() const;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
    void resizeEvent(QResizeEvent *);

    QSize sizeHint() const
    {
        return QSize(400,Def::Constant::Header_Height);
    }


private:
    void drawBackground(QPainter* painter);
    void drawItems(QPainter* painter);
    void drawSeparators(QPainter* painter);
    void drawHoverItem(QPainter* painter,HeaderItem* item,qreal& initX);
    void drawPressedItem(QPainter* painter,HeaderItem* item,qreal& initX);
    void drawNormalItem(QPainter* painter,HeaderItem* item,qreal& initX);

private:
    void initVars();
    void initSettings();
    void createItems();
    void clearHover();
    void mouseMoveEventHandler(QMouseEvent*);
    void mousePressEventHandler(QMouseEvent*);
    void mouseReleaseEventHandler(QMouseEvent *);
    qreal resetLatterItems(int startIndex); // returns the total width
    void updateColumnWidth();
    bool isDraggingItem(QMouseEvent* e);

    void setItemCount(int count);
    int itemCount() const;
    int getTotalWidth();



private:
    QStringList _labels;
    ContainerType<HeaderItem*> _headerItems;

    int _width;
    int _itemCount;

    bool _mousePressed;
    bool _startDragging;
    bool _isAutoAdjust;

    qreal _selectedXPos;
    qreal _maxWidth;

    HeaderItem* _selectedItem;
    QPointF _pressedPoint;

Q_SIGNALS:
    void columnWidthChanged(QVector<qreal> allColumnWidth);

    friend class LteTableWidgetPrivate;

};
#endif // LTEHEADER_H
