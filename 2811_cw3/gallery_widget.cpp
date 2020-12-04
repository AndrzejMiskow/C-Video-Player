#include "gallery_widget.h"
#include "vid_object.h"
#include "gallery_button.h"
#include <QTimer>
#include <QSize>
#include <QMetaObject>

GalleryWidget::GalleryWidget(ThePlayer* player, QString dirAddress) : QWidget() {
    this->player = player;
    vids = QList<vid_object>();
    buttonDisplay = new QWidget;
    {
        auto lay = new QVBoxLayout;
        lay->addWidget(buttonDisplay);
        setLayout(lay);
    }

    connect(this, SIGNAL(reinstance()), this, SLOT(replaceButtons()));

    QDir dir(dirAddress);
    QDirIterator it(dir);

    while(it.hasNext()){
        QString vidAd = it.next();

        if(vidAd.contains(".wmv")){
            QString thumbAd = vidAd.left(vidAd.length() - 4) + ".png";

            if(QFile(thumbAd).exists()){

                QImageReader* reader = new QImageReader(thumbAd);
                QImage img = reader->read();
                if(!img.isNull()){
                    vids.append(vid_object(new QUrl(QUrl::fromLocalFile(vidAd)), new QIcon(QPixmap::fromImage(img))));
                } else {
                    qDebug() << thumbAd << " failed to read";
                }
            } else {
                qDebug() << "No png found for " << vidAd;
            }
        }
    }

    emit reinstance();
}

void GalleryWidget::addVid(QString vidAd){
    if(vidAd.contains(".wmv")){
        QString thumbAd = vidAd.left(vidAd.length() - 4) + ".png";

        if(QFile(thumbAd).exists()){

            QImageReader reader(thumbAd);
            QImage img = reader.read();
            if(!img.isNull()){

                vid_object newVid(new QUrl(vidAd), new QIcon(QPixmap::fromImage(img)));
                vids.append(newVid);
            } else {
                qDebug() << thumbAd << " failed to read";
            }
        } else {
            qDebug() << "No png found for " << vidAd;
        }
    }
}

void GalleryWidget::replaceButtons(){
    //qDebug() << "in replaceButtons()";
    qDeleteAll(buttonDisplay->children());//delete all current buttons

    auto vlay = new QVBoxLayout();
    auto hlay = new QHBoxLayout();
    buttonDisplay->setLayout(vlay);
    vlay->addLayout(hlay);
    int x=0;
    for(int a=0; a<vids.length(); a++){
        auto v = vids[a];
        //qDebug() << "within for, " << x << " : " << *v.mediaLocation;
        if(++x > 3) {hlay = new QHBoxLayout(); vlay->addLayout(hlay); x=0;}

        auto but = new GalleryButton(QSize(200, 110), QSize(200, 110), *v.icon, v);

        connect(but, SIGNAL(clicked()), but, SLOT(createButtonInfo()));
        connect(but, SIGNAL(changePlayer(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));

        hlay->addWidget(but);
    }
}

