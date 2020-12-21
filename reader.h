#ifndef READER_H
#define READER_H

#include <QWidget>
#include <iostream>

namespace Ui {
class Reader;
}

class Reader : public QWidget
{
    Q_OBJECT

public:
    explicit Reader(QWidget *parent = nullptr);
    ~Reader();

signals:
    void changeListByClass(int);
    //根据分类
    void changeListByTime(int);
    //根据时间

public slots:
    void ComboBoxChanged(int);
    void CheckBoxChanged(int);

private:
    Ui::Reader *ui;
};

#endif // READER_H
