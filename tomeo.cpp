/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include "the_frame.h"
#include "reader.h"


using namespace std;

// read in videos and thumbnails to this directory
// 将视频和缩略图读入此目录
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    int i=0;
    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";

            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        string path = f.toStdString();
                        int pos = path.find_last_of('/');
                        string name(path.substr(pos + 1) );
                        int time[6] = {18,15,11,9,8,9};
                        out . push_back(TheButtonInfo( url , ico  , name , time[i++]) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }
    QString emp = "empty.png";
    QImageReader *imageReader = new QImageReader(emp);
        QImage sprite = imageReader->read(); // read the thumbnail
            QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
            QUrl* url = new QUrl(QUrl::fromLocalFile( NULL )); // convert the file location to a generic url
            out . push_back(TheButtonInfo(url, ico, emp.toStdString() , 0)); // add to the output list
    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QGridLayout* videoLayout = new QGridLayout;
    QVideoWidget *videoWidget = new QVideoWidget;
    videoLayout->addWidget(videoWidget, 0, 0);
    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);


    TheFrame* frame = new TheFrame(player);

    player->connect(player, SIGNAL(getTotalTime(int)),frame,SLOT (getTotalTime(int)));
    player->connect(player, SIGNAL(UpGrateSlider(qint64)),frame,SLOT (UpGrateSlider(qint64)));
    frame->connect(frame, SIGNAL(changeState(int)),
                   player,SLOT (changeState(int)));
    frame->connect(frame, SIGNAL(replay()),
                   player,SLOT (replay()));

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QVBoxLayout *layout = new QVBoxLayout();
    buttonWidget->setLayout(layout);

    Reader* reader = new Reader();
    reader->setMaximumHeight(40);

    reader->connect(reader,SIGNAL(changeListByClass(int)),player,SLOT(changeListByClass(int)));
    reader->connect(reader,SIGNAL(changeListByTime(int)),player,SLOT(changeListByTime(int)));


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    QHBoxLayout *lay = new QHBoxLayout();
    QVBoxLayout *right = new QVBoxLayout();
    QVBoxLayout *top = new QVBoxLayout();
    QWidget left;

    left.setLayout(top);
    left.setMinimumSize(800, 680);
    left.setMinimumWidth(750);
    reader->setMinimumSize(300,50);
    window.setLayout(lay);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(1100, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);

    top->addWidget(frame);
    // showtime!
    lay->addWidget(&left);
    QWidget* r = new QWidget();
    r->setLayout(right);
    right->addWidget(reader);
    right->addWidget(buttonWidget);
    lay->addWidget(r);
    window.show();

    // wait for the app to terminate
    return app.exec();
}
