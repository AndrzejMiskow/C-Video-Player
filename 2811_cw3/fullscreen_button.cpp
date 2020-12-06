#include "fullscreen_button.h"
#include <QTimer>
#include <QDebug>
#include <QTabBar>

void FullscreenButton::hideHierarchy(QWidget* start, QWidget* end){
    if(start != end){
        for(QObject* a : start->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != start) {
                if(b->isHidden()) previousState.insert(std::make_pair(b, true));
                else previousState.insert(std::make_pair(b, false));

                b->hide();
            }
        }
        hideHierarchy(start->parentWidget(), end);
    }
}

void FullscreenButton::showHierarchy(QWidget* start, QWidget* end){
    if(start != end){
        for(QObject* a : start->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != start) {
                if(!previousState[b]) b->show();
            }
        }
        showHierarchy(start->parentWidget(), end);
    }
}


void FullscreenButton::switchScreenState(){
    if(!QWidget::window()->isFullScreen()){//check if already fullscreen
        hideHierarchy(parentWindow, QWidget::window());

        QTimer::singleShot(0, QWidget::window(), SLOT(showFullScreen()));//Set widget to fullscreen
    } else {
        showHierarchy(parentWindow, QWidget::window());
        previousState.clear();

        QTimer::singleShot(0, QWidget::window(), SLOT(showNormal()));//return widget to normal size
    }
}


