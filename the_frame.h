#ifndef THEFRAME_H
#define THEFRAME_H
#include <iostream>
#include <QMediaPlayer>
#include<QWidget>
#include<QMovie>
#include<QLabel>
#include<QSpinBox>
#include<QToolButton>
#include<QSlider>
#include<QHBoxLayout>
#include<QGridLayout>
#include<QVBoxLayout>
#include<QIcon>
#include<QPixmap>
#include<QPushButton>
#include<QFileInfo>
#include<QFileDialog>
using namespace std;

class TheFrame : public QWidget
{
    Q_OBJECT
private:
    QLabel* label;
    QSlider* slider;
    QSpinBox *speedSpinBox;
    QLabel *speedLabel;
    QMediaPlayer* player;
    QPushButton* play_video;
    QPushButton* pause_video;
    QPushButton* stop_video;
    QPushButton* half_speed;
    QPushButton* normal_speed;
    QPushButton* fast_speed;
    QPushButton* double_speed;
    QLabel *volumeLabel;
    QSlider* volume_slider;
public:
    TheFrame(QMediaPlayer* p){
        player = p;

        // 进度条
        label = new QLabel("Current frame:");
        label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        slider = new QSlider(Qt::Horizontal);
        slider->setMinimum(0);
        slider->setTickInterval(1);

        // 播放速度
        speedLabel=new QLabel(tr("Speed:"));
        speedLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        half_speed = new QPushButton(QStringLiteral("x0.5"));
        normal_speed = new QPushButton(QStringLiteral("x1.0"));
        fast_speed = new QPushButton(QStringLiteral("x1.5"));
        double_speed = new QPushButton(QStringLiteral("x2.0"));

        play_video = new QPushButton(QStringLiteral("play"));
        pause_video = new QPushButton(QStringLiteral("pause"));
        stop_video = new QPushButton(QStringLiteral("stop"));
        play_video->setStyleSheet("QPushButton{\
                                 background-color:#A3F3D7;\
                                 font-size:20px}\
                                  QPushButton:hover{\
                                  background-color:red}");
        pause_video->setStyleSheet("QPushButton{\
                                  background-color:#FB6164;\
                                  font-size:20px}");
        stop_video->setStyleSheet("QPushButton{\
                                  background-color:#A3F3D7;\
                                 font-size:20px}");

        volumeLabel=new QLabel(tr("Volume:"));
        volumeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        volume_slider = new QSlider(Qt::Horizontal);
        volume_slider->setValue(0);

        QGridLayout* frameLayout = new QGridLayout;
        frameLayout->addWidget(label,1,0);
        frameLayout->addWidget(slider,1,1,1,4);
        frameLayout->addWidget(play_video,2,0);
        frameLayout->addWidget(pause_video,2,1);
        frameLayout->addWidget(stop_video,2,2);
        frameLayout->addWidget(volumeLabel,2,3);
        frameLayout->addWidget(volume_slider,2,4,1,1);

        connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(sliderChanged(int)));
        connect(slider,SIGNAL(sliderReleased()),this,SLOT(sliderReleased()));
        connect(play_video,SIGNAL(clicked()),this,SLOT(playVideo()));
        connect(pause_video,SIGNAL(clicked()),this,SLOT(pauseVideo()));
        connect(stop_video,SIGNAL(clicked()),this,SLOT(stopVideo()));
        connect(volume_slider,SIGNAL(sliderMoved(int)),this,SLOT(changeVolume(int)));

        this->setLayout(frameLayout);
    }

    QSlider* getSlider();

signals:
    void changeState(int);
    void replay();

public slots:
    void sliderChanged(int);
    void UpGrateSlider(qint64 position);
    void getTotalTime(int time);
    void sliderReleased();
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void changeVolume(int);
};

#endif // THEFRAME_H
