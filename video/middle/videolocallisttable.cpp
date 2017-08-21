#include "videolocallisttable.h"

#include <QHeaderView>
#include <QScrollBar>

#include "global_value.h"

#ifdef DEVICE_EVB
int video_item_height = 55;
int move_distance_next_step = 300;
#else
int video_item_height = 35;
int move_distance_next_step = 100;
#endif

VideoLocalListTable::VideoLocalListTable(QWidget *parent):BaseTableWidget(parent,move_distance_next_step)
{
    init();
    initConnection();
}

void VideoLocalListTable::init()
{
    m_previousFousedRow = -1;
    m_playingItemRow = -1;

    QFont listFont = this->font();
    listFont.setPixelSize(font_size_big);
    this->setFont(listFont);

    insertColumn(0);
    insertColumn(1);

    verticalHeader()->setDefaultSectionSize(video_item_height);
}

void VideoLocalListTable::initConnection()
{
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEnter(int,int)));
}

void VideoLocalListTable::slot_cellEnter(int row,int column)
{
    QTableWidgetItem *it = item(m_previousFousedRow,0);
    if(it != NULL)
    {
        if(m_playingItemRow!=m_previousFousedRow){
            setRowTextColor(m_previousFousedRow,QColor(255,255,255));
        }
    }
    it = item(row, column);
    if(it != NULL)
    {
        setRowTextColor(row,QColor(26,158,255));
    }
    m_previousFousedRow = row;
}

void VideoLocalListTable::setRowTextColor(int row, const QColor &color)const
{
    for(int col=0; col<columnCount(); col++)
    {
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
    horizontalHeader()->resizeSection(0,width()-120);
    horizontalHeader()->resizeSection(1,118);
#else
    horizontalHeader()->resizeSection(0,width()-58);
    horizontalHeader()->resizeSection(1,60);
#endif
    QTableWidget::resizeEvent(event);
}

