#ifndef LLTABLEWIDGET_H
#define LLTABLEWIDGET_H

#include <QWidget>
#include <QObject>

class LLTableWidgetSpirit;
class LLTableWidgetItem;
class LLTableWidgetRow;
class LLTableWidgetPrivate;
class LLTableWidget : public QWidget
{
    Q_DECLARE_PRIVATE(LLTableWidget)
    Q_OBJECT
public:
    explicit LLTableWidget(QWidget *parent = 0);

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

private:
    LLTableWidgetPrivate* d_ptr;
    Q_DISABLE_COPY(LLTableWidget)
    friend class LLTableWidgetSpirit;

Q_SIGNALS:
    void rowClicked(LLTableWidgetRow* row);
};

class LLTableWidgetRow
{
public:
    explicit LLTableWidgetRow();

public:
    void addItem(LLTableWidgetItem* item);
    void clear();
    int count() const;
    QList<LLTableWidgetItem*> items();

    void setBackgroundColor(const QColor& clr);
    QColor backgroundColor() const;

private:
    class PrivateData;
    QScopedPointer<PrivateData> d_data;
};

class LLTableWidgetItem
{
public:
    explicit LLTableWidgetItem(LLTableWidgetRow* parent = 0);

public:

    void setText(const QString& strText);
    QString text() const;

    void setData(const QVariant& var);
    QVariant data() const;

private:
    class PrivateData;
    QScopedPointer<PrivateData> d_data;
};

#endif // LLTABLEWIDGET_H
