#ifndef GALLERY_BUTTON_H
#define GALLERY_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QSize>
#include <QIcon>
#include "the_button.h"
#include "vid_object.h"

class GalleryButton : public QPushButton {
    Q_OBJECT
vid_object vo;
public:
    GalleryButton(QSize fixedSize, QSize iconSize, QIcon icon, vid_object &voc) : QPushButton() {
        this->setFixedSize(fixedSize);
        this->setIcon(icon);
        this->setIconSize(iconSize);
        vo.copy(voc);//placing this in the definition causes errors, I don't know why. Still works here though
    }
signals:
    void changePlayer(TheButtonInfo*);
public slots:
    void createButtonInfo(){//create a temporary TheButtonInfo in order to pass it to Player
        TheButtonInfo but(vo.mediaLocation, vo.icon);
        emit changePlayer(&but);
    }
};

#endif // GALLERY_BUTTON_H
