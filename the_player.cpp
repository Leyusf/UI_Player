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

void ThePlayer::changeListByClass(int index){
    string list[4] = {"b","s","t","d"};
    TheButtonInfo* emp;
    if (index!=0){
        for (int i=0;i<int(infos->size());i++){
             TheButtonInfo* info = &infos->at(i);
             string filename = info->name;
             int posf = filename.find_last_of('_');
             int pose = filename.find_last_of('.');
             string name(filename.substr(posf+1,pose-posf-1) );
             //获得后缀
             if (name==list[index-1]){
                 buttons -> at( updateCount++ % buttons->size() ) -> init( info );
             }
             if (filename=="empty.png"){
                 emp = info;
             }
        }
    }
    else{
        for (int i=0;i<int(infos->size());i++){
             TheButtonInfo* info = &infos->at(i);
             string filename = info->name;
             //获得后缀
             if (filename!="empty.png"){
                 buttons -> at( updateCount++ % buttons->size() ) -> init( info );
             }
             if (filename=="empty.png"){
                 emp = info;
             }

        }
    }

    while (updateCount<4) {
        buttons -> at( updateCount++ % buttons->size() ) -> init( emp );
    }
    updateCount = 0;
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

void ThePlayer::changeListByTime(int index){
    cout<<index<<endl;
    int time[infos->size()];
    TheButtonInfo* emp;
    if (index==2){
        for (int i=0;i<int(infos->size());i++){
             TheButtonInfo* info = &infos->at(i);
             time[i] = info->time;
             string filename = info->name;
             int posf = filename.find_last_of('_');
             int pose = filename.find_last_of('.');
             string name(filename.substr(posf+1,pose-posf-1) );
             if (filename=="empty.png"){
                 emp = info;
             }
        }

        sort(time,time+infos->size());
        for(int i:time)
           cout<<"T: "<<i<<endl;
        int j=infos->size()-1,c=0;
        while (c<4){
            for (int i=infos->size()-1;i>=0;i--){
                 TheButtonInfo* info = &infos->at(i);
                 cout<<"IT: "<<info->time<<"T: "<<time[j]<<endl;
                 if (time[j]==info->time){
                     cout<<"T"<<time[j]<<endl;
                     buttons -> at( updateCount++ % buttons->size() ) -> init( info );
                     j--;
                     break;
                 }
            }
            cout<<"C: "<<c<<endl;
            c++;
        }
    }
    else{
        for (int i=0;i<int(infos->size());i++){
             TheButtonInfo* info = &infos->at(i);
             string filename = info->name;
             //获得后缀
             if (filename!="empty.png"){
                 buttons -> at( updateCount++ % buttons->size() ) -> init( info );
             }
             if (filename=="empty.png"){
                 emp = info;
             }

        }
    }

    while (updateCount<4) {
        buttons -> at( updateCount++ % buttons->size() ) -> init( emp );
    }
    updateCount = 0;
}
