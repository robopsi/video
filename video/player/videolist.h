#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QObject>
#include <QUrl>

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

    enum PlayMode{
        PlayInOrder,
        PlayRandom,
        PlayOneCircle
    };

    QUrl getUrlAt(int index);
    QUrl getNextVideoUrl();
    QUrl getPreVideoUrl();
    void clearList();

    QList<QUrl> getUrlList(){return m_list;}
    inline void addToPlayList(const QString& path){m_list.append(QUrl::fromLocalFile(path));}

    void changePlayMode();
    PlayMode getCurrentPlayMode(){return m_currentPlayMode;}
private:
    // Current play list.
    QList<QUrl> m_list;
    // Current play index.
    int m_currentIndex;

    PlayMode m_currentPlayMode;
};

#endif // VIDEOLIST_H
