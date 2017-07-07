#include "videolocallisttable.h"

#include <QHeaderView>
#include <QScrollBar>

#include "global_value.h"

#ifdef DEVICE_EVB
int video_item_height = 55;
#else
int video_item_height = 35;
#endif

videoLocalListTable::videoLocalListTable(QWidget *parent):QTableWidget(parent)
{
    init();
    initConnection();
}

void videoLocalListTable::init()
{
    m_previousFousedRow = -1;
    m_playingItemRow = -1;

    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//垂直scrollbar禁用
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    setShowGrid(false);       //隐藏线
    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    setEditTriggers(QTableWidget::NoEditTriggers);  // 设置表格不可编辑
    setSelectionBehavior (QAbstractItemView::SelectRows); //设置点击选取行
    setSelectionMode (QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    insertColumn(0);
    insertColumn(1);
    verticalHeader()->setDefaultSectionSize(video_item_height);//设置默认item高度

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;margin: 0px 2px 0px 0px;}"
                                       "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                                       "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                       "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                       "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                       "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");

    setStyleSheet("QTableWidget{background:transparent}"
                  "QTableWidget{color:rgb(255,255,255);}"
                  "QTableWidget::item:selected{background:rgb(43,45,51);}"
                  "QTableWidget::item{selection-color:rgb(26,158,255);}"
                  );
}

void videoLocalListTable::initConnection()
{
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEnter(int,int)));
}

void videoLocalListTable::slot_cellEnter(int row,int column)
{
    QTableWidgetItem *it = item(m_previousFousedRow,0);
    // 还原上次选中的item
    if(it != NULL)
    {
        if(m_playingItemRow!=m_previousFousedRow){
            setRowTextColor(m_previousFousedRow,QColor(255,255,255));
        }
    }
    // 设置当前选中item的字体颜色
    it = item(row, column);
    if(it != NULL)
    {
        setRowTextColor(row,QColor(26,158,255));
    }
    m_previousFousedRow = row;
}

void videoLocalListTable::setRowTextColor(int row, const QColor &color)const
{
    for(int col=0; col<columnCount(); col++)
    {
        QTableWidgetItem *it = item(row, col);
        it->setTextColor(color);
    }
}

void videoLocalListTable::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    slot_cellEnter(-1, -1);
}

void videoLocalListTable::mouseMoveEvent(QMouseEvent *event)
{
    QTableWidget::mouseMoveEvent(event);
    // 不在格子上移动
    if(itemAt(mapFromGlobal(QCursor::pos()))==Q_NULLPTR)
    {
        slot_cellEnter(-1,-1);
    }
}

void videoLocalListTable::resizeEvent(QResizeEvent *)
{
#ifdef DEVICE_EVB
    horizontalHeader()->resizeSection(0,width()-120);
    horizontalHeader()->resizeSection(1,120);
#else
    horizontalHeader()->resizeSection(0,width()-80);
    horizontalHeader()->resizeSection(1,80);
#endif
}
