#include "control_bar.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QMediaPlayer>

ControlBar::ControlBar(QWidget *parent, QMediaPlayer *player) : QWidget(parent)
{
    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    muteButton = new QToolButton(this);
    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);

    speedBox = new QComboBox(this);
    speedBox->addItem("0.5x", QVariant(0.5));
    speedBox->addItem("1.0x", QVariant(1.0));
    speedBox->addItem("1.5x", QVariant(1.5));
    speedBox->addItem("2.0x", QVariant(2.0));
    speedBox->setCurrentIndex(1);

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));
    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));
    connect(speedBox, SIGNAL(activated(int)), this, SLOT(updateSpeed()));

    connect(this, SIGNAL(play()), player, SLOT(play()));
    connect(this, SIGNAL(pause()), player, SLOT(pause()));
    connect(this, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(this, SIGNAL(muteUnmute(bool)), player, SLOT(setMuted(bool)));
    connect(this, SIGNAL(changeSpeed(qreal)), player, SLOT(setPlaybackRate(qreal)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), this, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), this, SLOT(setMute(bool)));

    QBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(playButton);
    layout->addWidget(volumeSlider);
    layout->addWidget(muteButton);
    layout->addWidget(speedBox);
    setLayout(layout);
}

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

void ControlBar::setVolume(int volume)
{
   volumeSlider->setValue(volume);
}

void ControlBar::setMute(bool muted)
{
    playerMuted = muted;

    if(muted)
        muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    else
        muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}

void ControlBar::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::PlayingState:
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
        case QMediaPlayer::StoppedState:
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

void ControlBar::playClicked()
{
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
