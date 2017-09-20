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
    // Used to find out all video files
    QFileInfoList findVideoFiles(const QString& path = QString(""));
    void updatePlayingItemStyle(QMediaContent);
    void addVideo();
    void setOriginState();
    void addRefreshSuffix(QString suffix);
    VideoList* &getVideoList(){return m_playList;}
    void deleteItem(int);
private:
    VideoLocalListTable *m_localTable;
    ListHeader *m_listHeader;
    QStackedWidget *m_stackedWid;
    VideoNetListTable *m_netTable;

    VideoList *m_playList;
    // List of search suffix when refresh video resource.
    QList<QString> m_refreshSuffixList;

    void initData();
    void initLayout();
    void initConnection();
    void insertIntoLocalTable(QFileInfo);
signals:
    void sig_localTableItemClick(int,int);
    int tableLongPressed(int);
private slots:
    void slot_switchToLocalList(){m_stackedWid->setCurrentIndex(0);}
    void slot_switchToNetList(){m_stackedWid->setCurrentIndex(1);}
};

#endif // VIDEOMIDDLEWIDGETRIGHT_H
