#ifndef VIDEOLOCALLISTTABLE_H
#define VIDEOLOCALLISTTABLE_H

#include <QObject>
#include <QTableWidget>

class videoLocalListTable:public QTableWidget
{
    Q_OBJECT
public:
    videoLocalListTable(QWidget *parent);

    void setPlayingItemIndex(int index){m_playingItemRow = index;}

    void setRowTextColor(int row,const QColor &color)const;
private:
    void init();
    void initConnection();


    int m_previousFousedRow;
    int m_playingItemRow;
protected:
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent*);

private slots:
    void slot_cellEnter(int,int);
};

#endif // VIDEOLOCALLISTTABLE_H
