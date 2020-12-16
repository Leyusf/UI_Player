#include "reader.h"
#include "ui_reader.h"

Reader::Reader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reader)
{
    ui->setupUi(this);
}

Reader::~Reader()
{
    delete ui;
}

void Reader::ComboBoxChanged(int index){
    emit changeListByClass(index);
}

void Reader::CheckBoxChanged(int index){
    emit changeListByTime(index);
}
