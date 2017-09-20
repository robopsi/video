#ifndef VIDEOLOCALLISTTABLE_H
#define VIDEOLOCALLISTTABLE_H

#include <QObject>
#include "base/basetablewidget.h"

class VideoLocalListTable:public BaseTableWidget
{
    Q_OBJECT
public:
    VideoLocalListTable(QWidget *parent);

    void setPlayingItemIndex(int index){m_playingItemRow = index;}
    void setRowTextColor(int row,const QColor &color)const;

    void insertIntoTable(QString item1Text,QString item2Text);
    void removeTableItem(int row);
    void clearTable();
    void playingItemChanged(int index);
    void setOriginState();
private:
    void init();
    void initConnection();

    int playingItemIndex;
    QString playingItemSuffix;

    int m_previousFousedRow;
    int m_playingItemRow;
protected:
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent*);
private slots:
    void slot_cellEnter(int,int);
};

#endif // VIDEOLOCALLISTTABLE_H
