//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
    // 从第一个视频开始
}

// change the image and video for one button every one second
// 随机排序
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
        default:
            break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

// 打印当前播放视频的总时长
void ThePlayer::OnMetaDataAvailableChanged(bool available)
{
    if (available){
        qint64 time = this->metaData("Duration").toInt();
        if (time != 0){
            emit getTotalTime(time);
        }
    }
}

void ThePlayer::OnPositionChanged(qint64 position){
    emit UpGrateSlider(position);
}

void ThePlayer::changeState(int position){
    this->pause();
    this->setPosition(position);
}

void ThePlayer::replay(){
    this->play();
}
