#include "player.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player w;
    if (argc==2)
        w.setPath(argv[1]);  //set the path for the player
    w.show();
    return a.exec();
}
