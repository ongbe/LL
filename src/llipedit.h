#ifndef LLIPEDIT_H
#define LLIPEDIT_H

#include <QWidget>

class LLIpEditPrivate;
class LLIpEdit : public QWidget
{
    Q_DECLARE_PRIVATE(LLIpEdit)
    Q_OBJECT
public:
    explicit LLIpEdit(QWidget *parent = 0);

public:
    QString ip();
    void setIp(const QString& strIp);

private:
    LLIpEditPrivate* d_ptr;
    Q_DISABLE_COPY(LLIpEdit)

};

#endif // LLIPEDIT_H
