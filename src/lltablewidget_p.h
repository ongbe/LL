#ifndef LLTABLEWIDGET_P_H
#define LLTABLEWIDGET_P_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>

#include "lteheader.h"

class LLTableWidgetItem;
class LLTableWidgetRow;
class LLTableWidget;
class LLTableWidgetSpirit;
class LLTableWidgetPrivate : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(LLTableWidget)
public:
    explicit LLTableWidgetPrivate(LLTableWidget* parent = 0);
    ~LLTableWidgetPrivate();

public:

    void setLabels(const QStringList& labels);
    QStringList labels() const;

    void addRow(LLTableWidgetRow* row);
    int rowCount() const;
    void clear();
    void clearContents();

    void setEnableRowColors(bool enable);
    bool isEnabledRowColors() const;

    void setAutoAdjust(bool adjust);
    bool isAutoAdjust() const;

protected:
    QSize sizeHint() const
    {
        return QSize(300,300);
    }

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:

    void initWidgets();
    void initSettings();
    void initConnections();

private Q_SLOTS:
    void changeHeight(int totalHeight);

private:

    LLTableWidget* q_ptr;
    LteHeader* _header;
    LLTableWidgetSpirit* _spirit;
    QScrollBar* _bar;

    friend class LLTableWidgetSpirit;
};

class LLTableWidgetSpirit : public QWidget
{
    Q_OBJECT
public:
    explicit LLTableWidgetSpirit(LLTableWidgetPrivate* parent = 0);
    ~LLTableWidgetSpirit();

public:
    void addRow(LLTableWidgetRow* row);
    int rowCount() const;
    void clear();

    void setEnableRowColors(bool enable);
    bool isEnabledRowColors() const;

public Q_SLOTS:
    void setCurrentYPos(int yPos);
    int currentYPos() const;

    void setColumnWidth(QVector<qreal> columnWidth);

protected:
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *);

private:
    void drawVerticalLines(QPainter* painter);
    void drawRows(QPainter* painter);
    void drawOneRow(QPainter* painter, LLTableWidgetRow* row, int initY);
    void drawRowBg(QPainter* painter, LLTableWidgetRow* row, int initY);
    void drawPressedRowBg(QPainter* painter, LLTableWidgetRow* row, int initY);
    void drawItem(QPainter* painter, LLTableWidgetItem* item, qreal initX, qreal colWidth, int initY);

private:
    void notifyHeightChanged();

    void scrollUp();
    void scrollDown();

    void enlargeRowHeight();
    void shrinkRowHeight();

    void fire_clickItem();

private:
    LLTableWidgetPrivate* _parent;
    QList<LLTableWidgetRow*> _rows;

    bool _isEnabledRowColors;
    bool _isCtrlPressed;

    QVector<qreal> _columnWidth;

    int _currentIndex;
    int _currentYPos;
    int _pressedYPos;

    qreal _totalHeight;
    qreal _rowHeight;

    friend class LLTableWidgetPrivate;

Q_SIGNALS:
    void heightChanged(int totalHeight);
    void setBarValue(int value);
};

#endif // LLTABLEWIDGET_P_H
