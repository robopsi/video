#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QObject>
#include <QUrl>

enum PlayMode{
    PlayInOrder,
    PlayRandom,
    PlayOneCircle
};

/**
 * Used for manager the video list.
 *
 * Each video item saved with url and you can add、remove
 * or get video item.
 */
class VideoList:public QObject
{
    Q_OBJECT
public:
    VideoList(QObject *parent = 0);

    void clearList();
    void setPlayMode(PlayMode);
    int getPlayMode(){return m_playmode;}
    inline void addPlayList(const QString& path){m_list.append(QUrl::fromLocalFile(path));}
    QUrl getUrlAt(int index);
    QUrl getNextVideoUrl();
    QUrl getPreVideoUrl();
    void removeItem(int index);
    QList<QUrl> getUrlList(){return m_list;}
    void changePlayMode();
    PlayMode getCurrentPlayMode(){return m_playmode;}
private:
    // Current play list.
    QList<QUrl> m_list;
    int m_currentIndex;
    // Current play index.
    PlayMode m_playmode;
};

#endif // VIDEOLIST_H
