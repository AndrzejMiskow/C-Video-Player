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
    QIcon* icon;

    vid_object(QUrl* media, QIcon* png) : mediaLocation(media), icon(png){
        //qDebug() << *mediaLocation << " : " << icon;
    }
//    vid_object(vid_object &vo){
//        mediaLocation = vo.mediaLocation;
//        icon = vo.icon;
//    }
    vid_object(){};
    void copy(vid_object vo){
        mediaLocation = vo.mediaLocation;
        icon = vo.icon;
    }
};

#endif // VID_OBJECT_H
