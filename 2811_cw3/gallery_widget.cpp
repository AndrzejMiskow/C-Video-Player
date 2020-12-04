#include "gallery_widget.h"
#include "vid_object.h"
#include "gallery_button.h"
#include <QTimer>
#include <QSize>
#include <QMetaObject>
#include <QComboBox>
#include <QStack>

GalleryWidget::GalleryWidget(ThePlayer* player, QString dirAddress) : QWidget() {
    this->player = player;//keep track of which player this gallery is connected to
    vids = QList<vid_object>();//create a list of all videos that will be available through this gallery
    buttonDisplay = new QWidget;//create a widget that video icons can be placed in
    {//layout the top level display, places controls at the top and the buttonDisplay below
        auto lay = new QVBoxLayout;
        lay->addWidget(new QComboBox());
        lay->addWidget(buttonDisplay);
        setLayout(lay);
    }

    connect(this, SIGNAL(reinstance()), this, SLOT(replaceButtons()));//emit reinstance(); will now cause the buttons to rerender.

    {//this code block causes the buttons to reload every half a second, this allows them to resize according to screen.
        //Originally resized every time the screen changed size, this caused flickering though.
        auto mTimer = new QTimer(this);
        mTimer->setInterval(500);
        connect(mTimer, SIGNAL(timeout()), this, SLOT(replaceButtons()));
        mTimer->start();
    }

    QDir dir(dirAddress);//open the video directory
    QDirIterator it(dir);//create an iterator over the files in this directory

    while(it.hasNext()){//while there are files that haven't been iterated over
        QString vidAd = it.next();//get address of current file

        if(vidAd.contains(".wmv")){//if it is a video of the correct encoding, this may require modifying to work on mac/linux
            QString thumbAd = vidAd.left(vidAd.length() - 4) + ".png";//calculate address of thumbnail

            if(QFile(thumbAd).exists()){//check that thumbnail exists

                QImageReader* reader = new QImageReader(thumbAd);//currently a memory leak, investigate if this could be made into a stack variable
                QImage img = reader->read();
                if(!img.isNull()){//check that thumbnail is valid image
                    vids.append(vid_object(new QUrl(QUrl::fromLocalFile(vidAd)), new QIcon(QPixmap::fromImage(img))));//add a new vid_object representing this video to vids
                } else {
                    qDebug() << thumbAd << " failed to read";//error message, thumbnail was not valid
                }
            } else {
                qDebug() << "No png found for " << vidAd;//error message, no thumbail exists
            }
        }
    }

    emit reinstance();//setup buttons in default configuration. NOTE: the sizes cannot be known until the tab is chosen, so for now this defaults at the wrong sizes which then fix themselves. This is not ideal
}

void GalleryWidget::addVid(QString vidAd){//function to add new video to collection, code is repeated in main function. TODO: replace constructor code with calls to this
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
    emit reinstance();
}

void GalleryWidget::replaceButtons(){//this function arranges buttons, currently in no particular order
    //qDebug() << "in replaceButtons()";
    qDeleteAll(buttonDisplay->children());//delete all current buttons

    int amountPerRow = buttonDisplay->width() / 200;//calculate the number of buttons that will fit on a row

    auto vlay = new QVBoxLayout();//top level layout
    buttonDisplay->setLayout(vlay);
    auto hlay = new QHBoxLayout();//first of numberous second level layouts
    vlay->addLayout(hlay);
    int x=0;//keeps track of how many buttons per row there have been
    for(int a=0; a<vids.length(); a++){//iterate over all videos in collection
        auto v = vids[a];
        //qDebug() << "within for, " << x << " : " << *v.mediaLocation;
        if(++x > amountPerRow) {hlay->addStretch(); hlay = new QHBoxLayout(); vlay->addLayout(hlay); x=1;}//if this buttons would be more than allowed per row, create a new row and reset the counter

        auto but = new GalleryButton(QSize(200, 110), QSize(200, 110), *v.icon, v);//create a new button

        connect(but, SIGNAL(clicked()), but, SLOT(createButtonInfo()));//create a temporary TheButtonInfo instance to pass to player when clicked
        connect(but, SIGNAL(changePlayer(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));//pass TheButtonInfo instance to player

        hlay->addWidget(but);
    }
    hlay->addStretch();//adjust spacings to look like a grid

    vlay->addStretch();//same as above
}
