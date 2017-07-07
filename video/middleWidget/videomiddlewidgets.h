#ifndef VIDEOMIDDLEWIDGETS_H
#define VIDEOMIDDLEWIDGETS_H

#include "basewidget.h"
#include "videomiddlewidgetright.h"
#include "videomiddlewidgetleft.h"

class videoMiddleWidgets:public baseWidget
{
public:
    videoMiddleWidgets(QWidget *parent =0);

    videoMiddleWidgetRight *m_rightWid;
    videoMiddleWidgetLeft *m_leftWid;

    void initLayout();
};

#endif // VIDEOMIDDLEWIDGETS_H
