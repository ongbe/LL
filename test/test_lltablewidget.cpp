#include "test_lltablewidget.h"
#include "../src/lltablewidget.h"

#include <QVBoxLayout>

Test_LLTableWidget::Test_LLTableWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    LLTableWidget* table = new LLTableWidget;
    layout->addWidget(table);
    setLayout(layout);

    const QStringList labels = QStringList() << "Monday" << "Tuesday"
                                             << "Wednesday" << "Thursday"
                                             << "Friday" << "Saturday" << "Sunday";
    table->setLabels(labels);
    table->setAutoAdjust(true);
    table->setEnableRowColors(true);

    for(int i = 1; i <= 1000000; i++)
    {
        LLTableWidgetRow* row = new LLTableWidgetRow;
        row->setBackgroundColor(QColor(rand()%255,rand()%255,rand()%255));
        for(int j = 0; j < labels.size(); j++)
        {
            LLTableWidgetItem* item = new LLTableWidgetItem;
            item->setText(tr("Row:%1,Column:%2").arg(i).arg(j));
            row->addItem(item);
        }
        table->addRow(row);
    }
}
