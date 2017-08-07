#include "videolist.h"
#include <QTime>

VideoList::VideoList(QObject *parent):QObject(parent)
{
    m_currentIndex = 0 ;
    m_list.empty();
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
    if(m_currentIndex+1>=m_list.count())
    {
        m_currentIndex = 0;
    }
    else
    {
        m_currentIndex ++;
    }
    return getUrlAt(m_currentIndex);
}

QUrl VideoList::getPreVideoUrl()
{
    if(m_currentIndex == 0)
    {
        m_currentIndex = 0;
    }
    else
    {
        m_currentIndex--;
    }
    return getUrlAt(m_currentIndex);
}

void VideoList::clearList()
{
    m_list.clear();
    m_currentIndex=0;
}
