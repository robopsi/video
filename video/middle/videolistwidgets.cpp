#include "videolistwidgets.h"

#include <QTime>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QFile>
#include <QThread>
#include "focusswitchmanager.h"
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

    FocusSwitchManager::getInstance()->insertIntoMap("2,1", m_localTable);

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

void VideoListWidgets::setOriginState()
{
    m_localTable->setOriginState();
}

void VideoListWidgets::deleteItem(int row)
{
    QFile file(m_playList->getUrlAt(row).path());
    if (file.exists())
        file.remove();

    m_localTable->removeTableItem(row);
    m_playList->removeItem(row);
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
