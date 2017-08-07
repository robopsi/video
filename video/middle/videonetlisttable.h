#ifndef VIDEONETLISTTABLE_H
#define VIDEONETLISTTABLE_H

#include <QTableWidget>

class VideoNetListTable:public QTableWidget
{
public:
    VideoNetListTable(QWidget *parent);
private:
    void init();
};

#endif // VIDEONETLISTTABLE_H
