#include "videowidget.h"

#include <QKeyEvent>
#include <QMouseEvent>

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    setAttribute(Qt::WA_OpaquePaintEvent);
}

// listen for keyboard clicks
void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        setGeometry(9, 9, 781, 491);  //revert to the original size of the video
        event->accept();
    }
    else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        setFullScreen(!isFullScreen());
        event->accept();
    }
    else {
        QVideoWidget::keyPressEvent(event);
    }
}

// listen for the double-click event and set the video to full screen
void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreen(true);
    event->accept();
}

// listen for mouse clicks
void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}

