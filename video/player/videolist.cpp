#include "videolist.h"
#include <QTime>

VideoList::VideoList(QObject *parent):QObject(parent)
{
    m_currentIndex = 0;
    m_list.empty();

    setPlayMode(PlayInOrder);
}

void VideoList::setPlayMode(PlayMode playmode)
{
    m_playmode = playmode;
}

void VideoList::changePlayMode()
{
    switch (m_playmode) {
    case PlayInOrder:
        m_playmode = PlayRandom;
        break;
    case PlayRandom:
        m_playmode = PlayOneCircle;
        break;
    case PlayOneCircle:
        m_playmode = PlayInOrder;
        break;
    }
}

void VideoList::removeItem(int index)
{
    m_list.removeAt(index);
}

QUrl VideoList::getUrlAt(int index)
{
    if(m_list.isEmpty())
        return QUrl("");
    m_currentIndex = index;
    return m_list.value(index);
}

QUrl VideoList::getNextVideoUrl()
{
    switch(m_playmode){
    case PlayOneCircle:
        break;
    case PlayInOrder:
        if(m_currentIndex+1 >= m_list.count()){
            m_currentIndex = 0;
        }else{
            m_currentIndex ++;
        }
        break;
    case PlayRandom:
        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int xxx=qrand()%m_list.count();
        m_currentIndex = xxx;
        break;
    }
    return getUrlAt(m_currentIndex);
}

QUrl VideoList::getPreVideoUrl()
{
    switch(m_playmode){
    case PlayOneCircle:
        break;
    case PlayInOrder:
        if(m_currentIndex == 0){
            m_currentIndex = 0;
        }else{
            m_currentIndex--;
        }
        break;
    case PlayRandom:
        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int xxx=qrand()%m_list.count();
        m_currentIndex = xxx;
        break;
    }
    return getUrlAt(m_currentIndex);
}

void VideoList::clearList()
{
    m_list.clear();
    m_currentIndex=0;
}
