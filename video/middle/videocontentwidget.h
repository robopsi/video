#ifndef VIDEOCONTENTWIDGET_H
#define VIDEOCONTENTWIDGET_H

#include <QObject>
#include <QVideoWidget>
#include <QTimer>

class VideoContentWidget: public QVideoWidget
{
    Q_OBJECT
public:
    VideoContentWidget(QWidget *parent = 0);
private:
    void init();

    QTimer *m_timer;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void contentOneClick();
    void contentDoubleClick();
private slots:
    void onOneClick();
};

#endif // VIDEOCONTENTWIDGET_H
