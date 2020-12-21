//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <iostream>
#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <string>


using namespace std;

class ThePlayer : public QMediaPlayer {

    Q_OBJECT

private:
    vector<TheButtonInfo>* infos; // 视频信息列表
    vector<TheButton*>* buttons;  //按钮列表
    QTimer* mTimer;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        // 设定声音大小

        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this,
                 SLOT (playStateChanged(QMediaPlayer::State)) );

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();

        //connect( mTimer, SIGNAL (timeout()), SLOT ( shuffle() ) );

        setNotifyInterval(20);
        connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(OnPositionChanged(qint64)));

        connect(this, SIGNAL(metaDataAvailableChanged(bool)),
                this, SLOT(OnMetaDataAvailableChanged(bool)));
        // ...running shuffle method
        // 每1秒发出一次timeout信号，执行shuffle函数(刷新播放列表)
    }

    // all buttons have been setup, store pointers here
    // 将视频列表保存
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);


private slots:

    // change the image and video for one button every one second
    void shuffle();

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);

    // 获取视频长度
    void OnMetaDataAvailableChanged(bool available);

    void OnPositionChanged(qint64 position);

    void changeState(int);

    void replay();

    void changeListByClass(int);

    void changeListByTime(int index);

signals:
    void UpGrateSlider(qint64 position);

    void getTotalTime(int time);
};

#endif //CW2_THE_PLAYER_H
