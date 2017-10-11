#include "videolistwidgets.h"

#include <QDir>
#include <QTime>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QDirIterator>
#include <QFile>
#include <QThread>
#include "global_value.h"

VideoListWidgets::VideoListWidgets(QWidget *parent):BaseWidget(parent)
{
    //Set background color.
    setObjectName("VideoListWidgets");
    setStyleSheet("#VideoListWidgets{background:rgb(43,45,51)}");

    initData();
    initLayout();
    initConnection();
}

void VideoListWidgets::initData()
{
    // Initialize video play list.
    m_playList = new VideoList(this);

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
    connect(m_localTable,SIGNAL(cellClicked(int,int)),this,SIGNAL(sig_localTableItemClick(int,int)));
    connect(m_localTable,SIGNAL(longPressedEvent(int)),this,SIGNAL(tableLongPressed(int)));
}

void VideoListWidgets::addRefreshSuffix(QString suffix)
{
    m_refreshSuffixList.append(suffix);
}

void VideoListWidgets::setOriginState()
{
    m_localTable->setOriginState();
}

void VideoListWidgets::deleteItem(int row)
{
    QFile file(m_playList->getUrlAt(row).path());
    if(file.exists()){
        file.remove();
    }
    m_localTable->removeTableItem(row);
    m_playList->removeItem(row);
}

QFileInfoList VideoListWidgets::findVideoFiles(const QString& path)
{
    QFileInfoList videoFiles;

    QDirIterator it(path,QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    while (!QThread::currentThread()->isInterruptionRequested()&&it.hasNext()){
        QString name = it.next();
        QFileInfo info(name);
        if (info.isDir()){
            videoFiles.append(findVideoFiles(name));
        }
        else{
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
    m_localTable->clearTable();
    m_playList->clearList();

    for(int i=0;i<fileList.size();i++){
        QFileInfo fileInfo = fileList.at(i);
        if(!m_playList->getUrlList().contains(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))){
            insertIntoLocalTable(fileInfo);
        }
    }
}

void VideoListWidgets::insertIntoLocalTable(QFileInfo fileInfo)
{
    QString videoName = fileInfo.fileName();
    QString filePath = fileInfo.absoluteFilePath();

    m_localTable->insertIntoTable(videoName,"");

    m_playList->addPlayList(filePath);
}

void VideoListWidgets::updatePlayingItemStyle(QMediaContent content)
{
    QList<QUrl> urlList = m_playList->getUrlList();
    int index = -1;
    for(int i=0;i < urlList.size();i++){
        if(urlList.at(i)==content.canonicalUrl()){
            index = i;
            break;
        }
    }
    if(index!=-1){
        m_localTable->playingItemChanged(index);
    }
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
                insertIntoLocalTable(info);
            }
        }
    }
}

VideoListWidgets::~VideoListWidgets()
{
}
