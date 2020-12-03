#ifndef NEW_VIDEO_BUTTON_H
#define NEW_VIDEO_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <string>
#include <QMultimedia>
#include <QVideoFrame>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QVideoWidget>
#include "the_player.h"

class NewVideoButton : public QPushButton {
    Q_OBJECT
private:
    QFileDialog *file;//host OS default filer finder
    QString video_directory;//Location of where the video folder is, taken from the arguments
    ThePlayer* vidWindow;

    bool currentVidFrameSaved = false;//Read only the first frame, otherwise videoprobe reads many frames
    QString videoName;//extracts name of video from address
    QStringList remainingUrls;//Originally ability to add many files at once, function removed because of issues with access. Single file at a time

    bool addUrlToVideos(QString);//performs mediaplayer setting

    QMediaPlayer* player;//objects for extracting first frame from video. This runs player, not visible to user and muted
    QVideoWidget* vw;//viewwindow is not visible to user
    QVideoProbe* probe;//extracts frame information

public:
    NewVideoButton(std::string video_location, ThePlayer* vid) : QPushButton(tr("Add new video")) {
        video_directory = QString::fromStdString(video_location);
        vidWindow = vid;

        file = new QFileDialog();//create file dialog, uses system native
        file->setFileMode(QFileDialog::ExistingFile);//only allows the selection of a single, existing file
        file->setNameFilter(tr("Videos (*.wmv)"));//only allow wmv files, originally allowed mp4 but qt does not have supporting codec
        file->setViewMode(QFileDialog::List);//do not show unnecesary details
        file->setDirectory(video_directory);

        player = new QMediaPlayer();
        player->setVolume(0);//prevent user from hearing videos being processed
        probe = new QVideoProbe();
        vw = new QVideoWidget();

        connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(getImageFromFrame(QVideoFrame)));//connect videoprobe to function that saves image

        connect(this, SIGNAL(clicked()), this, SLOT(openFileDialog()));//open file dialog when button is clicked
    }

    ~NewVideoButton(){
        delete player;//clean up objects when button destroyed, not technically required but good practice
        delete probe;
        delete vw;
        delete file;
    }

private slots:
    void openFileDialog();//allow object react to events
    void getImageFromFrame(QVideoFrame frame);
};

#endif // NEW_VIDEO_BUTTON_H
