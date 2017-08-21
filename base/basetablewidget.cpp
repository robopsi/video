#include "basetablewidget.h"

#include <QHeaderView>
#include <QScrollBar>

QLineDelegate::QLineDelegate(QTableView* tableView)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor, 0, tableView->gridStyle());
    view = tableView;
}

void QLineDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    QStyledItemDelegate::paint(painter, option, index);
    QPen oldPen = painter->pen();
    painter->setPen(pen);
    //painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    painter->setPen(oldPen);
}

BaseTableWidget::BaseTableWidget(QWidget *parent,int moveDistanceNextStep):QTableWidget(parent)
{
    m_moveDistanceNextStep = moveDistanceNextStep;
    init();
}

void BaseTableWidget::init()
{
    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);

    setShowGrid(false);
    setItemDelegate(new QLineDelegate(this));
    setEditTriggers(QTableWidget::NoEditTriggers);
    setSelectionBehavior (QAbstractItemView::SelectRows);
    setSelectionMode (QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 0.5px;margin: 0px 0px 0px 0px;}"
                                       "QScrollBar::handle{background:rgb(217,217,217);border-radius:0px;}"
                                       "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                       "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 0px;}"
                                       "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 0px;}"
                                       "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");

    setStyleSheet("QTableWidget{background:transparent}"
                  "QTableWidget{color:rgb(255,255,255);}"
                  "QTableWidget::item:selected{background:rgb(43,45,51);}"
                  "QTableWidget::item{selection-color:rgb(26,158,255);}"
                  );
}

void BaseTableWidget::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    m_pressPoint = event->pos();
}

void BaseTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTableWidget::mouseReleaseEvent(event);
    m_pressPoint = QPoint(0,0);
}

void BaseTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    //    QTableWidget::mouseMoveEvent(event);
    if(!mutex.tryLock()){
        return;
    }
    int currentScrollBarValue;
    int nextScrollBarValue;
    int moveDistance;

    if(m_pressPoint.y() == 0){
        m_pressPoint = event->pos();
        goto EXIT;
    }

    moveDistance = event->pos().y() - m_pressPoint.y();
    if(abs(moveDistance) > m_moveDistanceNextStep){
        scrollBarMaximum = verticalScrollBar()->maximum();
        currentScrollBarValue = verticalScrollBar()->value();
        if(moveDistance < 0){
            if(currentScrollBarValue < scrollBarMaximum){
                nextScrollBarValue = currentScrollBarValue + 1;
            }else{
                goto EXIT;
            }
        }else{
            if(currentScrollBarValue > 0){
                nextScrollBarValue = currentScrollBarValue - 1;
            }else{
                goto EXIT;
            }
        }
        verticalScrollBar()->setValue(nextScrollBarValue);
        m_pressPoint = event->pos();
    }
EXIT:
    mutex.unlock();
}
