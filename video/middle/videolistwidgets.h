#ifndef VIDEOMIDDLEWIDGETRIGHT_H
#define VIDEOMIDDLEWIDGETRIGHT_H

#include "basewidget.h"
#include "listheader.h"
#include "videolocallisttable.h"
#include "videonetlisttable.h"
#include "player/videolist.h"

#include <QFileInfoList>
#include <QObject>
#include <QStackedWidget>
#include <QMediaContent>
#include <QEventLoop>

/**
 * Manager UI of video list widgets and send siganls and
 * methods to 'videowidgets' for run.
 */
class VideoListWidgets:public BaseWidget
{
    Q_OBJECT
public:
    VideoListWidgets(QWidget *parent=0);
    ~VideoListWidgets();
public:
    void updateResUi(QFileInfoList fileList);
    QFileInfoList findVideoFiles(const QString& path = QString(""));
    QString getCurrentVideoName(){return m_curPlayingVideoName;}
    VideoList* getVideoList(){return m_playList;}
    void updatePlayingItemStyle(QMediaContent);
    void addVideo();
    void setOriginState();

    VideoLocalListTable *m_localTable;
private:
    ListHeader *m_listHeader;
    QStackedWidget *m_stackedWid;
    VideoNetListTable *m_netTable;
    // Play list information.
    int m_curPlayingIndex;
    QString m_curPlayingDuration;
    QString m_curPlayingVideoName;
    VideoList *m_playList;

    void initLayout();
    void initConnection();
    void insertIntoLocalTable(int row,QString videoName,QString duration);
signals:
//    sig_localTableItemDoubleClick(int,int);
private slots:
    void slot_switchToLocalList(){m_stackedWid->setCurrentIndex(0);}
    void slot_switchToNetList(){m_stackedWid->setCurrentIndex(1);}
};

#endif // VIDEOMIDDLEWIDGETRIGHT_H
