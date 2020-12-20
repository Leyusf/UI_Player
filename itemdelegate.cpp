#include "itemdelegate.h"

#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include <QPainterPath>
#include "itemdef.h"

ItemDelegate::ItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

ItemDelegate::~ItemDelegate()
{
}

void ItemDelegate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option, const QModelIndex &index) const{
    if(index.isValid()){
        painter->save();

        auto variant = index.data(Qt::UserRole+1);

        ItemData data = variant.value<ItemData>();

        QStyleOptionViewItem viewOption(option);
        //draw item

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        const qreal radius = 0;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
        if(option.state.testFlag(QStyle::State_Selected)){
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(229, 241, 255));
            painter->drawPath(path);
        }else if(option.state.testFlag(QStyle::State_MouseOver)){
            painter->setPen(QPen(Qt::darkGray));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }else{
            painter->setPen(QPen(Qt::black));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
        QRect PixRect = QRect(rect.left()+5, rect.top()+5, rect.width()-10,rect.height()*3/4+10);
        QRect TextRect = QRect(rect.left()+5, rect.top()+rect.height()*3/4+15,
                               rect.width()-10,rect.height()/4);
        painter->drawPixmap(PixRect,*data.pix);
        painter->drawText(TextRect,*data.name);
        painter->restore();
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QSize(200, 160);
}
