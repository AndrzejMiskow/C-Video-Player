#include "control_bar.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QMediaPlayer>
#include <QScrollBar>


ControlBar::ControlBar(QWidget *parent, QMediaPlayer *player) : QWidget(parent)
{
    //Firstly create the buttons and sliders
    //QToolButton allows for the use of icons
    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playButton->setObjectName(tr("name1"));
    playButton->setMinimumSize(QSize(60,40));


    muteButton = new QToolButton(this);
    muteButton->setObjectName(tr("muteBtn"));
    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    muteButton->setMinimumSize(QSize(60,40));

    volumeSlider = new QScrollBar(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setMinimumSize(QSize(600,40));
    volumeSlider->setObjectName("volume");

    //Create a spinbox with various speed options
    //The "playbackrate" is a qreal


    //Connect internal signals and slots - generally updating displayed values
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));
    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));

    //Connect controller signals and player slots - these actually control playback
    connect(this, SIGNAL(play()), player, SLOT(play()));
    connect(this, SIGNAL(pause()), player, SLOT(pause()));
    connect(this, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(this, SIGNAL(muteUnmute(bool)), player, SLOT(setMuted(bool)));
    connect(this, SIGNAL(changeSpeed(qreal)), player, SLOT(setPlaybackRate(qreal)));
    //Connect player signals and controller slots - these update displayed values
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), this, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), this, SLOT(setMute(bool)));

    //Lay the components out nicely
    QBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(playButton);
    layout->addWidget(volumeSlider);
    layout->addWidget(muteButton);
    setLayout(layout);
}

//Getters to obtain the current values
QMediaPlayer::State ControlBar::state()
{
    return playerState;
}

int ControlBar::volume()
{
    return volumeSlider->value();
}

bool ControlBar::isMuted()
{
    return playerMuted;
}

qreal ControlBar::playbackSpeed()
{
    return speedBox->itemData(speedBox->currentIndex()).toDouble();
}

//Setters for the buttons, sliders, and combo box values and (condequently) appearances (public slots)
void ControlBar::setVolume(int volume)
{
   volumeSlider->setValue(volume);
}

void ControlBar::setMute(bool muted)
{
    playerMuted = muted;

    if(muted)
        muteButton->setIcon(QIcon(":/muted.png"));
    else
        muteButton->setIcon(QIcon(":/Sound.png"));
}

void ControlBar::setState(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::State::StoppedState) //Ignore stopped state as content is always looping
        return;
    if (state != playerState) {
        playerState = state;

        //Decide button icon based on current state
        switch (state) {
        case QMediaPlayer::PlayingState:
        case QMediaPlayer::StoppedState:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void ControlBar::setSpeed(float speed)
{
    for (int i=0; i< speedBox->count(); i++) {
        if (speed == speedBox->itemData(i).toDouble()) {
            speedBox->setCurrentIndex(i);
            return;
        }
    }
}

//Private slots for when buttons are clicked
void ControlBar::playClicked()
{
    //Emitted signal dependent on current player state
    switch(playerState) {
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
        emit play();
        break;
    }
}

void ControlBar::updateSpeed()
{
    emit changeSpeed(playbackSpeed());
}

void ControlBar::muteClicked()
{
    emit muteUnmute(!playerMuted);
}
