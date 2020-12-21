//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H


#include <QPushButton>
#include <QUrl>
#include <string>
#include <iostream>

using namespace std;
class TheButtonInfo {
public:
    QUrl* url; // video file to play
    QIcon* icon; // icon to display
    std::string name;
    int time;

    TheButtonInfo ( QUrl* url, QIcon* icon, std::string lname, int ltime) : url (url), icon (icon) {
        name = std::string(lname);
        time = ltime;
    }
};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;
    TheButton(QWidget *parent) :  QPushButton(parent) {
         setIconSize(QSize(200,110));
         connect(this, SIGNAL(released()), this, SLOT (clicked() ));
         // if QPushButton clicked...then run clicked() below

    }


    void init(TheButtonInfo* i);
    // 将视频信息(视频路径和缩略图)与按钮相关联

private slots:
    void clicked();
    // 点击动作

signals:
    void jumpTo(TheButtonInfo*);
    // 跳转至被点击按钮关联的视频

};

#endif //CW2_THE_BUTTON_H
