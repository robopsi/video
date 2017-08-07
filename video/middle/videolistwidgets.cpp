#include "videolistwidgets.h"

#include <QHeaderView>
#include <QDir>
#include <QTime>
#include <QFileDialog>
#include <QEventLoop>
#include <QMediaPlayer>
#include <QDirIterator>

#include "global_value.h"

VideoListWidgets::VideoListWidgets(QWidget *parent):BaseWidget(parent)
  ,m_curPlayingIndex(-1)
{
    //Set background color.
    setObjectName("VideoListWidgets");
    setStyleSheet("#VideoListWidgets{background:rgb(43,45,51)}");
    // Init video play list.
    m_playList = new VideoList(this);

    initLayout();
    initConnection();
}

void VideoListWidgets::initLayout()
{
    QVBoxLayout *vmianlyout = new QVBoxLayout;

    /*
     * The list widegts includes with header and list.
     * and the 'list' include net list and local list.
     */
    m_listHeader = new ListHeader(this);
    m_listHeader->setVisible(false);
    m_listHeader->setFixedHeight(100);

    m_stackedWid = new QStackedWidget(this);
    m_localTable = new VideoLocalListTable(m_stackedWid);
    m_netTable = new VideoNetListTable(m_stackedWid);

    m_stackedWid->addWidget(m_localTable);
    m_stackedWid->addWidget(m_netTable);

    vmianlyout->addWidget(m_listHeader);
    vmianlyout->addWidget(m_stackedWid);
    vmianlyout->setContentsMargins(40,0,40,0);

    setLayout(vmianlyout);
}

void VideoListWidgets::initConnection()
{
    connect(m_listHeader,SIGNAL(buttonLocalClick()),this,SLOT(slot_switchToLocalList()));
    connect(m_listHeader,SIGNAL(buttonNetClick()),this,SLOT(slot_switchToNetList()));
    //connect(m_localTable,SIGNAL(cellDoubleClicked(int,int)),this,SIGNAL(sig_localTableItemDoubleClick(int,int)));
}

void VideoListWidgets::setOriginState()
{
    if(m_curPlayingIndex!=-1)
    {
        m_localTable->setRowTextColor(m_curPlayingIndex,QColor(255,255,255));
        m_localTable->item(m_curPlayingIndex,1)->setText("");
        m_curPlayingIndex = -1;
        m_localTable->setCurrentCell(-1,0);
    }
}


/**
 * Used to find out all video files
 * @param path The path to search video
 */
QFileInfoList VideoListWidgets::findVideoFiles(const QString& path)
{
    QFileInfoList videoFiles;

    QDirIterator it(path,QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QString name = it.next();
        QFileInfo info(name);
        if (info.isDir())
        {
            videoFiles.append(findVideoFiles(name));
        }
        else
        {
            if (info.suffix() == "mp4" || info.suffix() == "avi" || info.suffix() == "rm"
                    ||info.suffix() == "rmvb" || info.suffix() == "wmv" || info.suffix() == "mkv"
                    ||info.suffix() == "mov" || info.suffix() == "asf")
            {
                videoFiles.append(info);
            }
        }
    }
    return videoFiles;
}

void VideoListWidgets::updateResUi(QFileInfoList fileList)
{
    // Clear list first.
    for(int i = m_localTable->rowCount();i > 0;i--)
    {
        m_localTable->removeRow(0);
    }
    m_playList->clearList();

    for(int i=0;i<fileList.size();i++){
        QFileInfo fileInfo = fileList.at(i);
        if(!m_playList->getUrlList().contains(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))){
            int rowCount = m_localTable->rowCount();
            insertIntoLocalTable(rowCount,fileInfo.fileName()," ");
            m_playList->addToPlayList(fileInfo.absoluteFilePath());
        }
    }
    m_curPlayingIndex = -1;
}


void VideoListWidgets::insertIntoLocalTable(int row, QString videoName, QString duration)
{
    m_localTable->insertRow(row);
    m_localTable->setItem(row,0,new QTableWidgetItem(videoName));
    m_localTable->setItem(row,1,new QTableWidgetItem(duration));
    m_localTable->item(row,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
}

void VideoListWidgets::updatePlayingItemStyle(QMediaContent content)
{
    QList<QUrl> urlList = m_playList->getUrlList();
    // Restore state next time.
    if(m_curPlayingIndex!=-1)
    {
        m_localTable->setRowTextColor(m_curPlayingIndex,QColor(255,255,255));
        m_localTable->item(m_curPlayingIndex,1)->setText(m_curPlayingDuration);
        m_curPlayingIndex = -1;
        m_localTable->setPlayingItemIndex(m_curPlayingIndex);
        m_localTable->setCurrentCell(-1,0);
    }
    int index = -1;
    for(int i=0;i < urlList.size();i++)
    {
        if(urlList.at(i)==content.canonicalUrl()){
            index = i;
            break;
        }
    }
    if(index!=-1)
    {
        m_curPlayingIndex = index;
        m_curPlayingDuration = m_localTable->item(index,1)->text();
        m_curPlayingVideoName = m_localTable->item(index,0)->text();

        m_localTable->setRowTextColor(m_curPlayingIndex,QColor(26,158,255));
        m_localTable->item(index,1)->setText(str_video_playing);

        m_localTable->setCurrentCell(index,0);
        m_localTable->setPlayingItemIndex(m_curPlayingIndex);
    }
    update();
}

void VideoListWidgets::addVideo()
{
    QFileDialog *dialog = new QFileDialog(mainWindow,"Selete File");
    if(dialog->exec()==QFileDialog::Accepted)
    {
        QStringList files = dialog->selectedFiles();
        if(files.isEmpty())
            return;
        for(int i=0;i<files.count();i++)
        {
            QFileInfo info(files[i]);
            if(!m_playList->getUrlList().contains(QUrl::fromLocalFile(files.value(i)))&&info.exists())
            {
                QString fileName=info.fileName();
                QString filePath=files.value(i);

                int rowCount = m_localTable->rowCount();
                insertIntoLocalTable(rowCount,fileName," ");
                m_playList->addToPlayList(filePath);
            }
        }
    }
}

VideoListWidgets::~VideoListWidgets()
{
    delete m_listHeader;
    delete m_stackedWid;
    delete m_localTable;
    delete m_netTable;
}
