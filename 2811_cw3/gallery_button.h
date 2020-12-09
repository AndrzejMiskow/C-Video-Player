#ifndef GALLERY_BUTTON_H
#define GALLERY_BUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QSize>
#include <QIcon>
#include "the_button.h"
#include "vid_object.h"

class GalleryButton : public QToolButton {
    Q_OBJECT
vid_object vo;
public:
    GalleryButton(vid_object &voc) : QToolButton() {
        //setting the thumbnail in the gallery
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        this->setIcon(*voc.icon);
        this->setIconSize(QSize(200, 110));

        //file name taken form the URL of the video
        QString filename = voc.mediaLocation->fileName();
        this->setText(filename);

        //settting the size of the gallery buttons
        this->setMinimumSize(200,110);
        vo.copy(voc);//placing this in the definition causes errors, I don't know why. Still works here though

        this->setObjectName(tr("gallery_button"));
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
