#include "fullscreen_button.h"
#include <QTimer>
#include <QDebug>

void FullscreenButton::switchScreenState(){
    if(!parentWindow->parentWidget()->isFullScreen()){//check if already fullscreen
        for(QObject* a : parentWindow->parentWidget()->children()){//iterate through all children of the parent of the window to be fullscreened
            auto b = dynamic_cast<QWidget*>(a);//only hide QWidgets
            if(b != 0)
            if(b != parentWindow) b->hide();//hide QWidgets
        }
        QTimer::singleShot(0, parentWindow->parentWidget(), SLOT(showFullScreen()));//Set widget to fullscreen
    } else {
        for(QObject* a : parentWindow->parentWidget()->children()){//same as above, but instead show the QWidgets
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != parentWindow) b->show();
        }
        QTimer::singleShot(0, parentWindow->parentWidget(), SLOT(showNormal()));//return widget to normal size
    }
}
