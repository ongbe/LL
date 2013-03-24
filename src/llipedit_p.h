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
public:
    explicit LLIpEditPrivate(LLIpEdit* parent);

public:
    QString ip() const;
    void setIp(const QString& strIp);

private:
    QString _ip;

    LLIpEdit* q_ptr;
    Q_DISABLE_COPY(LLIpEditPrivate)
};

#endif // LLIPEDIT_P_H
