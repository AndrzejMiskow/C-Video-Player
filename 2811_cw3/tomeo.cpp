/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QTabWidget>
#include <QScrollArea>
#include "control_bar.h"

#include "new_video_button.h"
#include <QString>
#include "fullscreen_button.h"
#include "gallery_widget.h"

#include "mainwindow.h"


using namespace std;
int vid_number = 0;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );
        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    QScrollArea *scroll = new QScrollArea;
    // the ControlBar used to control playback settings
    ControlBar *controls = new ControlBar(0,player);
    controls->setMute(controls->isMuted());
    controls->setState(player->state());
    controls->setVolume(player->volume());

    //Widget for playback controls (likely to be expanded beyond what's here now) ****remember to edit this coment****
    QHBoxLayout *playbackLayout = new QHBoxLayout();
    playbackLayout->addWidget(controls);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    //scroll area for the
    scroll->setWidget(buttonWidget);
    scroll->setWidgetResizable(true);
    scroll->setMinimumHeight(160);

    //Create a layout for the buttons and playback controls
    QVBoxLayout *controlLayout = new QVBoxLayout();
    controlLayout->addLayout(playbackLayout);
    //controlLayout->addWidget(buttonWidget);//code moved to line 172 to prevent buttons from maxising with controls

    // create the buttons for the number of videos in the folder
    for ( int i = 0; i < (int)videos.size(); i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    //QWidget *window = new QWidget();
    MainWindow w(player);
    QWidget& window = *w.centralWidget();

    QVBoxLayout *top = new QVBoxLayout();
    QTabWidget *tabs = new QTabWidget();

    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    //Code for fullscreen button, widgets in layout fsh will be made fullscreen
    QWidget fullScreenHolder;
    QVBoxLayout *fsh = new QVBoxLayout();
    fullScreenHolder.setLayout(fsh);
    fsh->setMargin(0);

    // add the video and the buttons to the top level widget
    //top->addWidget(videoWidget);//Not sure what this does, commenting it out does not appear to affect the program, and it should be redundant because of line 189.EDIT: qobjects can only exist in one place at once, so this command is replaced almost instantly by line 189.

    top->addWidget(scroll);

    auto Gallery = new GalleryWidget(player, argv[1]);

    //navigation tabs in the program
    tabs->addTab(&window,"Video Player");
    tabs->addTab(Gallery, "Gallery");

    top->addWidget(new NewVideoButton(argv[1], player, Gallery));
    top->addWidget(&fullScreenHolder);
    fsh->addWidget(new FullscreenButton(&fullScreenHolder, tabs));
    fsh->addWidget(videoWidget);
    fsh->addLayout(controlLayout);
    fsh->addWidget(w.slider);

    top->addWidget(buttonWidget);

    fsh->addWidget(w.slider);

    // showtime!
    tabs->show();


    // wait for the app to terminate
    return app.exec();
}


