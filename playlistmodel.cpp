#include "playlistmodel.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QImageReader>
#include <QPixmap>


PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

PlaylistModel::~PlaylistModel()
{
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid()
            && row >= 0 && row < m_playlist->mediaCount()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (index.isValid() && role>=0) {

        QVariant value = m_data[index];
        if (!value.isValid() && index.column() == Title
                && index.row() < m_playlist->mediaCount()) {
            QUrl location = m_playlist->media(index.row()).request().url();
            auto path = location.toLocalFile();
            auto name = QFileInfo(path).fileName();
            auto thumb = path.left(path.length() - 4) + ".png";
            value = name;
            if (QFile(thumb).exists()) {
                auto imageReader = new QImageReader(thumb);
                auto sprite = imageReader->read();  // read the thumbnail
                if (!sprite.isNull()) {
                    auto pixmap = QPixmap::fromImage(sprite);
                    ItemData itemdata;
                    itemdata.pix = new QPixmap(pixmap);
                    itemdata.name = new QString(name);
                    value.setValue(itemdata);
                } else {
                    qDebug() << "warning: I couldn't process thumbnail " << thumb << endl;
                }
            } else {
                qDebug() << "warning: I couldn't find thumbnail " << thumb << endl;
                int pose = path.toStdString().find_last_of("/");
                thumb = path.left(pose) + "/nodata.png";
                auto imageReader = new QImageReader(thumb);
                auto sprite = imageReader->read();  // read the thumbnail
                if (!sprite.isNull()) {
                    auto pixmap = QPixmap::fromImage(sprite);
                    ItemData itemdata;
                    itemdata.pix = new QPixmap(pixmap);
                    itemdata.name = new QString(name);
                    value.setValue(itemdata);
                } else {
                    qDebug() << "warning: I couldn't process thumbnail " << thumb << endl;
                }
            }
            // Cache the item
            const_cast<PlaylistModel *>(this)->m_data[index] = value;
        }
        return value;
    }
    return QVariant();
}

QMediaPlaylist *PlaylistModel::playlist() const
{
    return m_playlist.data();
}

void PlaylistModel::setPlaylist(QMediaPlaylist *playlist)
{
    if (m_playlist) {
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted,
                   this, &PlaylistModel::beginInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaInserted,
                   this, &PlaylistModel::endInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved,
                   this, &PlaylistModel::beginRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaRemoved,
                   this, &PlaylistModel::endRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaChanged,
                   this, &PlaylistModel::changeItems);
    }

    beginResetModel();
    m_playlist.reset(playlist);

    if (m_playlist) {
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted,
                this, &PlaylistModel::beginInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaInserted,
                this, &PlaylistModel::endInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved,
                this, &PlaylistModel::beginRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaRemoved,
                this, &PlaylistModel::endRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaChanged,
                this, &PlaylistModel::changeItems);
    }

    endResetModel();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::beginInsertItems(int start, int end)
{
    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void PlaylistModel::endInsertItems()
{
    endInsertRows();
}

void PlaylistModel::beginRemoveItems(int start, int end)
{
    m_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistModel::endRemoveItems()
{
    endInsertRows();
}

void PlaylistModel::changeItems(int start, int end)
{
    m_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}
