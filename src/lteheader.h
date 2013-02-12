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

class LteTableWidgetRow;
class LteTableWidgetItem;
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

class LteTableWidgetReal : public QWidget
{
    Q_OBJECT

    enum ExportFormat
    {
        EF_Text,
        EF_Xml,
        EF_Html
    };

public:
    explicit LteTableWidgetReal(QWidget* parent = 0);
    virtual ~LteTableWidgetReal();

public:
    void exportContent();

    void clear();

    void addRow(LteTableWidgetRow* row);
    int rowCount() const;

    void setIconVisible(bool visible);
    bool isIconVisible() const;

    void setFont(const QFont &font);

    void setAutoPilot(bool bAuto);
    bool isAutoPilot() const;

    bool find(const QString& strPattern,Qt::CaseSensitivity cs);

    void setGridVisible(bool visible);
    bool isGridVisible() const;

    void setAutoAdjust(bool bAuto);
    bool isAutoAdjust() const;

    const LteTableWidgetRow* currentRow() const;

    void setItemCountLimit(int limit);
    int itemCountLimit() const;

    void setAlternatingRowColors(bool enable);

    void expandAll();
    void collapseAll();

    void setEnableItemBgColors(bool enable);
    bool enabledItemBgColors() const;

public Q_SLOTS:

    void columnWidthChanged(ContainerType<qreal> allColumnWidth);
    static QColor getRandColor();
    void receiveKeyPressEvent(QKeyEvent* e);

protected:
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

private Q_SLOTS:
    void setCurrentYPos(int y);
    void rowHeightChanged(qreal dHeight);
    void doUpdateGeometry();
    void setViewportX(int startX,int endX);

private:
    void drawBackground(QPainter* painter);
    void drawExpandBox(QPainter* painter,LteTableWidgetItem* item,qreal columnStartXPos,qreal columnEndXPos,qreal initY);
    void drawItemPressedBackground(QPainter* painter);
    void drawItemPressedFrame(QPainter* painter);
    void drawItemSeparationLine(QPainter* painter, qreal initY);

#ifdef USE_ICON
    bool drawIcon(QPainter* painter,LteTableWidgetItem* item,qreal columnXPos,qreal columnEndXPos,qreal initY);
#endif
    void drawRows(QPainter* painter);
    qreal drawOneRow(QPainter* painter, LteTableWidgetRow* row, qreal initY);
    qreal drawItem(QPainter* painter, LteTableWidgetItem* item, qreal columnXPos,
                   qreal& rowYPos, qreal columnWidth, qreal columnEndXPos, const QColor& bgColor);
    void drawItemBgColor(QPainter* painter, qreal initY, qreal startX, qreal endX, const QColor& bgColor);
    qreal drawChildren(QPainter* painter, LteTableWidgetItem* parent,
                       const QList<LteTableWidgetItem*> children, qreal columnXPos,
                       qreal& rowYPos, qreal columnWidth);
    void drawItemBackground(QPainter* painter);
    void drawPlainBackground(QPainter* painter);
    void drawVerticalSeparator(QPainter* painter);
    void drawHighlightText(QPainter* painter, LteTableWidgetItem* item, const QRectF& rect, const QColor& bgColor);
    void drawHighlightBlock(QPainter* painter, const QString& strText, const QRectF& rect);
    void drawDottedText(QPainter* painter, LteTableWidgetItem* item, const QRectF& rect, const QColor& bgColor);
    void drawRowBackground(QPainter* painter, LteTableWidgetRow* row, qreal initY);
    qreal getColumnStartXPos(int columnIndex);
    qreal getColumnWidth(int columnIndex);
    QString getDottedString(const QString& originalStr,qreal width);

    QColor getReversedColor(const QColor& clr);

private:

    typedef QPair<QRectF,QString> RectTip;
    typedef QPair<QRectF,LteTableWidgetItem*> RectItem;

    void initVars();
    void initSettings();
    void initConnections();
    void initUpdateTimer();
    void wheelEventHandler(QWheelEvent* e);
    void mouseMoveEventHandler(QMouseEvent* e);
    void mousePressEventHandler(QMouseEvent* e);
    void resizeEventHandler(QResizeEvent* e);
    void keyPressEventHandler(QKeyEvent *e);

    void addRectItem(const QRectF& rect,LteTableWidgetItem* item);
    void clearRectItem();

    void addRectTip(const QRectF& rect,const QString& strTip);
    void clearRectTip();
    void notifyGeoChange();

    void scrollDown();
    void scrollUp();

    void autoAdjust();
    void incrementByRowHeight(qreal& initY);
    inline void increment(qreal& initValue,qreal incre);

    inline void updateAndNotifyGeoChange();
    void reset();
    void calcBeginIndex();
    void judgeScrollingDown(int y);
    void setCurrentAndPressedYPos(int y);
    void adjustItemCount();
    inline void doAutoPilot();

    void _exportAsText();
    void _exportAsXml();
    void _exportAsHtml();
    void getExportFormat();

    ContainerType<LteTableWidgetRow*> _rows;
    ContainerType<qreal> _allColumnWidth;
    ContainerType<RectTip> _rectTips;
    ContainerType<RectItem> _rectItems;

    qreal _totalHeight;
    qreal _currentYPos;
    qreal _pressedYPos;
    qreal _dCurrentPressY;

    bool _isIconVisible;
    bool _isAutoPilot;
    bool _isFindOn;
    bool _isGridVisible;
    bool _isAutoAdjust;
    bool _isScrollingDown;
    bool _enableAlternativeRowColors;
    bool _enableItemBgColors;

    int _startIndex;
    int _breakIndex;
    int _oldY;
    int _itemCountLimit;
    int _viewportStartX;
    int _viewportEndX;

    QString _findPattern;

    QFont _textFont;

    QTimer* _geometryUpdateTimer;

    Qt::CaseSensitivity _findCaseSensitivity;

    QString _strExportFile;
    ExportFormat _exportFormat;

    friend class LteTableWidgetPrivate;
    Q_DISABLE_COPY(LteTableWidgetReal)
Q_SIGNALS:
    void currentYPosChanged(int y);
    void totalHeightChanged(int totalHeight);
};

#endif // LTEHEADER_H
