#include "player.h"
#include "ui_player.h"
#include "playlistmodel.h"
#include "videowidget.h"
#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QComboBox>

using namespace std;
Player::Player(QWidget *parent): QMainWindow(parent), ui(new Ui::Player){
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    player->setAudioRole(QAudio::VideoRole);
    volumn = 0;
    player->setVolume(volumn);
    this->setState(player->state());
    qInfo() << "Supported audio roles:";
    for (QAudio::Role role : player->supportedAudioRoles())
        qInfo() << "    " << role;
    //playlist = new QMediaPlaylist();
    initPlayLists();
    player->setPlaylist(playlist);
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this,
            &Player::playlistPositionChanged);
    player->setVideoOutput(ui->videoWidget);
    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    ui->playlistView->setModel(playlistModel);
    ui->playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));
    connect(ui->oB, &QPushButton::clicked, this, &Player::add);
    connect(ui->aB, &QPushButton::clicked, this, &Player::open);
    connect(ui->rB, &QPushButton::clicked, this, &Player::remove);
    player->setNotifyInterval(20);
    ui->stopB->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->stopB->setEnabled(false);
    connect(ui->stopB, &QAbstractButton::clicked, player, &QMediaPlayer::stop);
    ui->preB->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(ui->preB, &QAbstractButton::clicked, playlist, &QMediaPlaylist::previous);
    ui->pauseB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(ui->pauseB, &QAbstractButton::clicked, this, &Player::playClicked);
    ui->nextB->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(ui->nextB, &QAbstractButton::clicked, playlist, &QMediaPlaylist::next);
    ui->sB->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    connect(ui->sB, &QAbstractButton::clicked, this, &Player::muteClicked);
    ui->volumn_slider->setRange(0, 100);
    connect(ui->volumn_slider, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);
    connect(player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this,
            &Player::metaDataChanged);
    connect(ui->playlistView, &QAbstractItemView::activated, this, &Player::jump);
    connect(player, &QMediaPlayer::stateChanged, this, &Player::setState);
    connect(player, &QMediaPlayer::mutedChanged, this, &Player::setMuted);
    ui->v_slider->setRange(0, player->duration());
    connect(ui->v_slider, &QSlider::sliderMoved, this, &Player::seek);
    connect(ui->classBox,SIGNAL(currentIndexChanged(int)), this, SLOT(ClassChanged(int)));
    metaDataChanged();
}

Player::~Player()
{
    delete ui;
}

void Player::playlistPositionChanged(int currentItem)
{
    ui->playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"),
                                                           Qt::CaseInsensitive);
}

void Player::addToPlaylist(const QList<QUrl> &urls)
{
    for (auto &url: urls) {

        if (isPlaylist(url)){
            playlist->load(url);

        }
        else{
            playlistVector->at(0)->addMedia(url);

            string filename = url.fileName().toStdString();

            int posf = filename.find_last_of('_');
            int pose = filename.find_last_of('.');
            string name(filename.substr(posf+1,pose-posf-1) );
            //获得后缀
            if (name == "basketball"){
                playlistVector->at(1)->addMedia(url);
            }
            else if (name == "running"){
                playlistVector->at(2)->addMedia(url);
            }
            else if (name == "esport"){
                playlistVector->at(3)->addMedia(url);
            }
            else if (name == "soccer"){
                playlistVector->at(4)->addMedia(url);
            }
            else{
                playlistVector->at(5)->addMedia(url);
            }
        }
    }
    QFile file("tmp.m3u");
    file.open(QIODevice::WriteOnly);
    playlistVector->at(indexPlaylist)->save(&file,"m3u");
    playlist->clear();
    file.close();
    file.open((QIODevice::ReadOnly));
    playlist->load(&file,"m3u");
    file.close();
    file.remove();
}

void Player::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations
        (QStandardPaths::MoviesLocation).value(0, QDir::homePath()));


    if (fileDialog.exec() == QDialog::Accepted){
        addToPlaylist(fileDialog.selectedUrls());
    }

}


void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

void Player::durationChanged(qint64 m_duration)
{
    duration = m_duration / 1000;
    ui->v_slider->setMaximum(duration);
}

void Player::positionChanged(qint64 progress)
{
    if (!ui->v_slider->isSliderDown())
        ui->v_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void Player::metaDataChanged()
{
    if (player->isMetaDataAvailable()) {

        if (ui->v_coverLabel) {
            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            ui->v_coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60, (duration / 60) % 60,
            duration % 60, (duration * 1000) % 1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->v_labelDuration->setText(tStr);
}

void Player::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void Player::remove()
{
    int index = playlist->currentIndex();
    QMediaContent m = playlist->currentMedia();

    for (int i=0;i<6;i++){
        int t = playlistVector->at(i)->mediaCount();
        for (int j=0;j<t;j++){
            if (m==playlistVector->at(i)->media(j)){
                playlistVector->at(i)->removeMedia(j);
            }
        }
    }
    playlist->removeMedia(index);
}

void Player::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            ui->stopB->setEnabled(false);
            ui->pauseB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            ui->stopB->setEnabled(true);
            ui->pauseB->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            ui->stopB->setEnabled(true);
            ui->pauseB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void Player::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        player->play();
        break;
    case QMediaPlayer::PlayingState:
        player->pause();
        break;
    }
}


void Player::onVolumeSliderValueChanged()
{
    emit changeVolume(volume());
}

int Player::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(ui->volumn_slider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void Player::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;

        ui->sB->setIcon(style()->standardIcon(muted
                ? QStyle::SP_MediaVolumeMuted
                : QStyle::SP_MediaVolume));

        if (muted ==true){
            volumn = ui->volumn_slider->value();
            ui->volumn_slider->setValue(0);
        }
        else{
            ui->volumn_slider->setValue(volumn);
        }
    }
}

void Player::muteClicked()
{
    player->setMuted(!playerMuted);
}

void Player::initPlayLists(){
    indexPlaylist = 0;
    playlist = new QMediaPlaylist();
    playlistVector = new QVector<QMediaPlaylist*>();
    for (int i=0;i<6;i++){
        QMediaPlaylist *tmp = new QMediaPlaylist();
        playlistVector->append(tmp);
    }
}

void Player::ClassChanged(int index){
    indexPlaylist = index;
    QFile file("tmp.m3u");
    file.open(QIODevice::WriteOnly);
    playlistVector->at(indexPlaylist)->save(&file,"m3u");
    playlist->clear();
    file.close();
    file.open((QIODevice::ReadOnly));
    playlist->load(&file,"m3u");
    file.close();
    file.remove();
}

void Player::setPath(std::string fpath){
    path = fpath;
}

void Player::add()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    QString Qpath = QString::fromStdString(path);
    if (path==""){
        fileDialog.setDirectory(QStandardPaths::standardLocations
           (QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    }
    else{
        fileDialog.setDirectory(Qpath);
    }

    if (path != ""){
        QList<QUrl> urls;
        QDir dir(Qpath);
        QDirIterator it(dir);
        while (it.hasNext()){
            QString f = it.next();
            if (f.contains(".wmv")){
                QUrl* url = new QUrl(QUrl::fromLocalFile(f));
                urls<<*url;
            }
        }
        addToPlaylist(urls);
    }
    ui->oB->setEnabled(false);
}
