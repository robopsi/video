#include "videolocallisttable.h"

#include <QHeaderView>
#include <QScrollBar>

#include "global_value.h"

#ifdef DEVICE_EVB
int video_item_height = 60;
int move_distance_next_step = 300;
#else
int video_item_height = 35;
int move_distance_next_step = 100;
#endif

VideoLocalListTable::VideoLocalListTable(QWidget *parent):BaseTableWidget(parent,move_distance_next_step)
  ,playingItemIndex(-1)
{
    init();
    initConnection();
}

void VideoLocalListTable::init()
{
    m_previousFousedRow = -1;
    m_playingItemRow = -1;

    insertColumn(0);
    insertColumn(1);

    verticalHeader()->setDefaultSectionSize(video_item_height);
}

void VideoLocalListTable::initConnection()
{
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEnter(int,int)));
}

void VideoLocalListTable::playingItemChanged(int index)
{
    if(playingItemIndex != -1){
        item(playingItemIndex,1)->setText(playingItemSuffix);
    }

    if(index != -1){
        playingItemIndex = index;
        playingItemSuffix = item(index,1)->text();
        setCurrentCell(index,0);
        item(index,1)->setText("Playing");
    }
}

void VideoLocalListTable::insertIntoTable(QString item1Text, QString item2Text)
{
    int count= rowCount();
    insertRow(count);

    setItem(count,0,new QTableWidgetItem(item1Text));
    setItem(count,1,new QTableWidgetItem(item2Text));

    item(count,1)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
}

void VideoLocalListTable::removeTableItem(int row)
{
    this->removeRow(row);
    if(row < playingItemIndex){
        playingItemIndex --;
    }else if(row == playingItemIndex){
        playingItemIndex = -1;
    }
}

void VideoLocalListTable::setOriginState()
{
    setCurrentCell(-1,-1);
    playingItemChanged(-1);
}

void VideoLocalListTable::clearTable()
{
    for(int i = rowCount();i > 0;i--){
        removeRow(0);
    }
}

void VideoLocalListTable::slot_cellEnter(int row,int column)
{
    QTableWidgetItem *it = item(m_previousFousedRow,0);
    if(it != NULL){
        if(m_playingItemRow!=m_previousFousedRow){
            setRowTextColor(m_previousFousedRow,QColor(255,255,255));
        }
    }
    it = item(row, column);
    if(it != NULL){
        setRowTextColor(row,QColor(26,158,255));
    }
    m_previousFousedRow = row;
}

void VideoLocalListTable::setRowTextColor(int row, const QColor &color)const
{
    for(int col=0; col<columnCount(); col++){
        QTableWidgetItem *it = item(row, col);
        it->setTextColor(color);
    }
}

void VideoLocalListTable::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    slot_cellEnter(-1, -1);
}

void VideoLocalListTable::resizeEvent(QResizeEvent *event)
{
#ifdef DEVICE_EVB
    horizontalHeader()->resizeSection(0,width()-130);
    horizontalHeader()->resizeSection(1,130);
#else
    horizontalHeader()->resizeSection(0,width()-70);
    horizontalHeader()->resizeSection(1,70);
#endif
    QTableWidget::resizeEvent(event);
}

