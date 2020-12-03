#include "fullscreen_button.h"
#include <QTimer>
#include <QDebug>
#include <QTabBar>

void hideHierarchy(QWidget* start, QWidget* end){
    if(start->parentWidget() != end){
        for(QObject* a : start->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != start) b->hide();
        }
        hideHierarchy(start->parentWidget(), end);
    }
}

void showHierarchy(QWidget* start, QWidget* end){
    if(start->parentWidget() != end){
        for(QObject* a : start->parentWidget()->children()){
            auto b = dynamic_cast<QWidget*>(a);
            if(b != 0)
            if(b != start) b->show();
        }
        showHierarchy(start->parentWidget(), end);
    }
}


void FullscreenButton::switchScreenState(){
    if(!tabWidget->isFullScreen()){//check if already fullscreen
        hideHierarchy(parentWindow, tabWidget);

        tabWidget->tabBar()->hide();
        QTimer::singleShot(0, tabWidget, SLOT(showFullScreen()));//Set widget to fullscreen
    } else {
        showHierarchy(parentWindow, tabWidget);

        tabWidget->tabBar()->show();
        QTimer::singleShot(0, tabWidget, SLOT(showNormal()));//return widget to normal size
    }
}


