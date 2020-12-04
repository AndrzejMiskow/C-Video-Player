#include "gallery_widget.h"
#include "vid_object.h"
#include "gallery_button.h"
#include <QTimer>
#include <QSize>
#include <QMetaObject>
#include <QComboBox>
#include <QStack>
#include <QToolButton>

GalleryWidget::GalleryWidget(ThePlayer* player, QString dirAddress) : QWidget() {
    this->player = player;//keep track of which player this gallery is connected to
    vids = QList<vid_object>();//create a list of all videos that will be available through this gallery
    buttonDisplay = new QWidget;//create a widget that video icons can be placed in
    {//layout the top level display, places controls at the top and the buttonDisplay below
        auto topLay = new QHBoxLayout;
        sortBox = new QComboBox;
        searchButton = new QPushButton("Search");
        text = new QLineEdit;
        topLay->addWidget(text);
        topLay->addWidget(searchButton);
        topLay->addWidget(sortBox);

        auto lay = new QVBoxLayout;
        lay->addLayout(topLay);
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

        addVid(vidAd);//add this video to the collection
    }

    emit reinstance();//setup buttons in default configuration. NOTE: the sizes cannot be known until the tab is chosen, so for now this defaults at the wrong sizes which then fix themselves. This is not ideal
}

void GalleryWidget::addVid(QString vidAd){//function to add new video to collection, code is repeated in main function. TODO: replace constructor code with calls to this
    if(vidAd.contains(".wmv")){//if it is a video of the correct encoding, this may require modifying to work on mac/linux
        QString thumbAd = vidAd.left(vidAd.length() - 4) + ".png";//calculate address of thumbnail

        if(QFile(thumbAd).exists()){//check that thumbnail exists

            QImageReader reader(thumbAd);
            QImage img = reader.read();
            if(!img.isNull()){//check that thumbnail is valid image

                vid_object newVid(new QUrl(vidAd), new QIcon(QPixmap::fromImage(img)));//add a new vid_object representing this video to vids
                vids.append(newVid);
                vidsToDisplay.append(vids.length() - 1);//by default, add this to the videos that should be displayed
            } else {
                qDebug() << thumbAd << " failed to read";//error message, thumbnail was not valid
            }
        } else {
            qDebug() << "No png found for " << vidAd;//error message, no thumbail exists
        }
    }
    emit reinstance();
    //there should be a SIGNAL here to recalculate which videos should be displayed based on the state of the user input objects
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
    for(auto a : vidsToDisplay){//iterate over all videos in collection
        auto v = vids[a];
        //qDebug() << "within for, " << x << " : " << *v.mediaLocation;
        if(++x > amountPerRow) {hlay->addStretch(); hlay = new QHBoxLayout(); vlay->addLayout(hlay); x=1;}//if this buttons would be more than allowed per row, create a new row and reset the counter

        auto but = new GalleryButton(v);//create a new button

        connect(but, SIGNAL(clicked()), but, SLOT(createButtonInfo()));//create a temporary TheButtonInfo instance to pass to player when clicked
        connect(but, SIGNAL(changePlayer(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));//pass TheButtonInfo instance to player
        //connect(but, SIGNAL(clicked()), , SLOT(setCurrentIndex(0)));//attempt to change tab when button is pressed, does not work

        hlay->addWidget(but);
    }
    hlay->addStretch();//adjust spacings to look like a grid

    vlay->addStretch();//same as above
    vlay->setMargin(0);
}
