#ifndef ITEMDEF_H
#define ITEMDEF_H

#include <QMetaType>
#include <QVariant>

struct ItemData{
    QPixmap* pix;
    QString* name;
};
Q_DECLARE_METATYPE(ItemData)

#endif // ITEMDEF_H
