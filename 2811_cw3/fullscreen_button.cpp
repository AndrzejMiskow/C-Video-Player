#include "fullscreen_button.h"
#include <QTimer>
#include <QDebug>

void FullscreenButton::switchScreenState(){
    if(!parentWindow->parentWidget()->isFullScreen()){
        for(QObject* a : parentWindow->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != parentWindow) b->hide();
        }
        QTimer::singleShot(0, parentWindow->parentWidget(), SLOT(showFullScreen()));
        qDebug() << "attempt to make fullscreen";
    } else {
        for(QObject* a : parentWindow->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != parentWindow) b->show();
        }
        QTimer::singleShot(0, parentWindow->parentWidget(), SLOT(showNormal()));
        qDebug() << "attempt to make not fullscreen";
    }
}
