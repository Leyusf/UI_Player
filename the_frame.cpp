#include "the_frame.h"
void TheFrame::UpGrateSlider(qint64 position){
    slider->setValue(position);
}

void TheFrame::getTotalTime(int time){
    slider->setMaximum(time);
}

void TheFrame::sliderChanged(int position){
    emit changeState(position);
}

QSlider* TheFrame::getSlider(){
    return slider;
}

void TheFrame::sliderReleased(){
    emit replay();
}
void TheFrame::playVideo(){
    player->play();
}

void TheFrame::pauseVideo(){
    if(player == NULL)
        return;
    player->pause();
}

void TheFrame::stopVideo(){
    player->stop();
}

void TheFrame::changeVolume(int i){
    player->setVolume(i);
}
