#ifndef LLIPEDIT_P_H
#define LLIPEDIT_P_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class LLIpEdit;
class LLIpEditPrivate : public QWidget
{
    Q_OBJECT
    enum { First, Second, Third, Fourth, Count };
public:
    explicit LLIpEditPrivate(LLIpEdit* parent);

public:
    QString ip() const;
    void setIp(const QString& strIp);

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawBg(QPainter* painter);
    QLabel* createLabel();
    void initEdits();
private:
    QString _ip;
    QLineEdit* _edits[Count];

    LLIpEdit* q_ptr;
    Q_DISABLE_COPY(LLIpEditPrivate)
};

#endif // LLIPEDIT_P_H
