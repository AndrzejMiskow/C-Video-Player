#include "new_video_button.h"
#include "the_button.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QDialog>
#include <QDebug>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QVideoFrame>
#include <iostream>
#include <QVideoWidget>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <QIcon>
#include <QImageReader>

void NewVideoButton::openFileDialog(){
    if(file->exec()){ //if file dialog opens, wait on results then act on them.
        remainingUrls = file->selectedFiles();//extract results chose by user.
        addUrlToVideos(remainingUrls.value(0));//there will only ever be one with the current configuration.

        QStringList a = remainingUrls.value(0).split("/");//split the file address into segments, the last of which is the filename
        videoName = a.at(a.length() - 1);//get the last segment
        videoName = videoName.split(".").value(0);//remove the file extention, note that there is a bug here in that it will behave strangely with videos that have periods in their name

        std::ifstream in;//instantiate in and out objects
        std::ofstream out;
        in.open(remainingUrls.takeFirst().toStdString(), std::ios::in | std::ios::binary);//read from the file pointed at by user, read bytes as is
        out.open((video_directory + "/" + videoName + ".wmv").toStdString(), std::ios::out | std::ios::binary);//create new file at video folder, write bytes directly

        out << in.rdbuf();//this basically copies the file

        in.close();//clean up objects
        out.close();


    }
}

bool NewVideoButton::addUrlToVideos(QString url){
    currentVidFrameSaved = false;//support multiple uses in single instance, reset states when starting with new

    player->setVideoOutput(vw);//set videoplayer to show on virtual screen, without this causes encoding errors, note: this might make more sense to be in the constructor instead of here
    probe->setSource(player);//setup probe that extract information from mediaplayer, note: this might make more sense to be in the constructor instead of here

    player->setMedia(QUrl::fromLocalFile(url));//point player to media that the user input
    player->play();//play player, videoprobe will automatically call getImageFromFrame when a new frame is played

    return true;
}

void NewVideoButton::getImageFromFrame(QVideoFrame frame){
    //turn frame into image, save in file folder
    if(!currentVidFrameSaved){//only save a frame if it has not already done so for this video
        currentVidFrameSaved = true;//mark that this video has already had a frame saved

        QImage a = frame.image();//extract image from videoprobe frame

        a = a.mirrored();//flip image, otherwise they are upside down. I believe this is an artefact of the videowindow having no width or height

        a.save(video_directory + "/" + videoName + ".png");//save the image to the videodirectory, name is the same as video except with a png extention

        QTimer::singleShot(0, player, &QMediaPlayer::stop);//call a slot on the mediaplayer to stop playback. This does not work if the function, stop(), is accessed directly for unclear reasons, see below comment

        //if(remainingUrls.length()!=0) addUrlToVideos(remainingUrls.takeFirst());
        //this would allow for multiple videos to be done in a single selection, however because of the same problem as above methods in mediaplayer cannot be accessed from the GUI thread that slots run on.
        //I wasn't able to find a slot in mediaplayer that addresses this, perhaps in future versions a custom wrapper around QMediaPlayer could solve this.

        QIcon *ico = new QIcon(QPixmap::fromImage(a));//create ico from image, needs to exist beyond this function. This is technically a memory leak, but since it doesn't need to be freed until the end of the program it doesn't matter
        QUrl *ur = new QUrl(QUrl::fromLocalFile(video_directory + "\\" + videoName + ".wmv"));//not certain this is necessary, but it works
        TheButtonInfo but(ur, ico);//create a new buttoninfo representing the new video

        vidWindow->newContent(but);//add that buttoninfo to those already in the player
        gallery->addVid(video_directory + "/" + videoName + ".wmv");
    }
}
