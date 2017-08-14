#ifndef VIDEOQUICKCONTENTWIDGET_H
#define VIDEOQUICKCONTENTWIDGET_H
#include <QQuickWidget>
#include <QObject>
#include <QTimer>
#include <QQuickItem>

class VideoQuickContentWidget:public QQuickWidget
{
    Q_OBJECT
public:
    VideoQuickContentWidget(QWidget *parent = 0);

    void setFullScreen(bool fullScreen);
    bool isFullScreenSize(){return isFullScreen;}
private:
    int normalWidth;
    int normalHeight;
    void init();

    bool isFullScreen;
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

#endif // VIDEOQUICKCONTENTWIDGET_H
