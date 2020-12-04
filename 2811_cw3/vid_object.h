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
    ~vid_object(){
        //delete mediaLocation;
        //delete icon;
    }
};

#endif // VID_OBJECT_H
