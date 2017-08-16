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
    // Initialize video play list.
    m_playList = new VideoList(this);

    initData();
    initLayout();
    initConnection();
}

void VideoListWidgets::initData()
{
    m_refreshSuffixList.append("mp4");
    m_refreshSuffixList.append("avi");
    m_refreshSuffixList.append("rm");
    m_refreshSuffixList.append("rmvb");
    m_refreshSuffixList.append("wmv");
    m_refreshSuffixList.append("mkv");
    m_refreshSuffixList.append("asf");
    m_refreshSuffixList.append("mov");
    m_refreshSuffixList.append("ts");
    m_refreshSuffixList.append("mpg");
    m_refreshSuffixList.append("mpg");
    m_refreshSuffixList.append("m2ts");
    m_refreshSuffixList.append("trp");
    m_refreshSuffixList.append("flv");
    m_refreshSuffixList.append("WEBM");
    m_refreshSuffixList.append("3GP");
    m_refreshSuffixList.append("Vob");
    m_refreshSuffixList.append("MPG");
    m_refreshSuffixList.append("DAT");
}

void VideoListWidgets::initLayout()
{
    QVBoxLayout *vmianlyout = new QVBoxLayout;

    /*
     * The list widegts includes with header and list.
     * and the 'list' include net list and local list.
     */
    m_listHeader = new ListHeader(this);

    m_stackedWid = new QStackedWidget(this);
    m_localTable = new VideoLocalListTable(m_stackedWid);
    m_netTable = new VideoNetListTable(m_stackedWid);

    m_stackedWid->addWidget(m_localTable);
    m_stackedWid->addWidget(m_netTable);

    vmianlyout->addWidget(m_listHeader);
    vmianlyout->addWidget(m_stackedWid);
    vmianlyout->setContentsMargins(10,0,10,0);

    setLayout(vmianlyout);
}

void VideoListWidgets::initConnection()
{
    connect(m_listHeader,SIGNAL(buttonLocalClick()),this,SLOT(slot_switchToLocalList()));
    connect(m_listHeader,SIGNAL(buttonNetClick()),this,SLOT(slot_switchToNetList()));
    //connect(m_localTable,SIGNAL(cellDoubleClicked(int,int)),this,SIGNAL(sig_localTableItemDoubleClick(int,int)));
}

void VideoListWidgets::addRefreshSuffix(QString suffix)
{
    m_refreshSuffixList.append(suffix);
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
            for(int i=0;i<m_refreshSuffixList.count();i++){
                if(info.suffix().compare(m_refreshSuffixList.at(i),Qt::CaseInsensitive) == 0){
                    videoFiles.append(info);
                }
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
}
