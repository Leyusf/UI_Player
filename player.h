#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QLabel>
#include <string>
#include "playlistmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();
    void setPath(std::string fpath);


private:
    std::string path="";
    int volumn;
    int indexPlaylist;
    QMediaPlayer::State playerState = QMediaPlayer::StoppedState;
    bool playerMuted = false;
    Ui::Player *ui;
    QMediaPlayer* player;
    QMediaPlaylist *playlist = nullptr;
    //All
    QVector<QMediaPlaylist*>* playlistVector;
    QVector<int>* timeList;
    PlaylistModel *playlistModel = nullptr;
    qint64 duration;
    QSlider *slider = nullptr;
    QLabel *coverLabel = nullptr;
    void setMuted(bool muted);
    void addToPlaylist(const QList<QUrl> &urls);
    void updateDurationInfo(qint64 currentInfo);
    int volume() const;
    void initPlayLists();
signals:
    void changeMuting(bool muting);
    void changeVolume(int volume);

private slots:
    void ClassChanged(int index);
    void setState(QMediaPlayer::State state);
    void muteClicked();
    void playClicked();
    void onVolumeSliderValueChanged();
    void playlistPositionChanged(int currentItem);
    void open();
    void add();
    void remove();
    void jump(const QModelIndex &index);
    void metaDataChanged();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void seek(int seconds);
};
#endif // PLAYER_H
