#ifndef VIDEOMIDDLEWIDGETS_H
#define VIDEOMIDDLEWIDGETS_H

#include "basewidget.h"
#include "videolistwidgets.h"
#include "videocontentwidgets.h"

#include <QStackedWidget>

/**
 * Manager the UI and control of video middle widgets.
 *
 * The middle widgets has two modes(vide list mode and play mode)
 * and UI changes when mode changes.
 * {@link videolistwidgets.h}、{@link videocontentwidgets.h}
 */
class VideoMiddleWidgets:public BaseWidget
{
    Q_OBJECT
public:
    VideoMiddleWidgets(QWidget *parent =0);
    ~VideoMiddleWidgets();

    VideoListWidgets *m_listWid;
    VideoContentWidgets *m_contentWid;
private:

    void initLayout();
};
#endif // VIDEOMIDDLEWIDGETS_H
