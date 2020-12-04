#ifndef VID_OBJECT_H
#define VID_OBJECT_H

#include <QUrl>
#include <QIcon>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QDebug>
#include <QImageReader>

class vid_object
{
public:
    QUrl* mediaLocation;
    QIcon* icon;//holds thumbnail of the video this represents

    vid_object(QUrl* media, QIcon* png) : mediaLocation(media), icon(png){
        //qDebug() << *mediaLocation << " : " << icon;
    }
    //for some reason, having a copy constructor in here causes build errors. I cannot find why
//    vid_object(vid_object &vo){
//        mediaLocation = vo.mediaLocation;
//        icon = vo.icon;
//    }
    vid_object(){};//empty constructor required for usage in gallery_button.h

    void copy(vid_object vo){//for use as a bootleg copy constructor
        mediaLocation = vo.mediaLocation;
        icon = vo.icon;
    }
};

#endif // VID_OBJECT_H
