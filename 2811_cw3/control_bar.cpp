#include "control_bar.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>

control_bar::control_bar(QWidget *parent) : QWidget(parent)
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

    QBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(playButton);
    layout->addWidget(volumeSlider);
    layout->addWidget(muteButton);
    layout->addWidget(speedBox);
    setLayout(layout);
}

QMediaPlayer::State control_bar::state()
{
    return playerState;
}

int control_bar::volume()
{
    return volumeSlider->value();
}

bool control_bar::isMuted()
{
    return playerMuted;
}

float control_bar::playbackSpeed()
{
    return speedBox->itemData(speedBox->currentIndex()).toFloat();
}

void control_bar::setVolume(int volume)
{
   volumeSlider->setValue(volume);
}

void control_bar::setMute(bool muted)
{
    playerMuted = muted;

    if(muted)
        muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    else
        muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}

void control_bar::setState(QMediaPlayer::State state)
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

void control_bar::setSpeed(float speed)
{
    for (int i=0; i< speedBox->count(); i++) {
        if (speed == speedBox->itemData(i).toDouble()) {
            speedBox->setCurrentIndex(i);
            return;
        }
    }
}

void control_bar::playClicked()
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

void control_bar::updateSpeed()
{
    emit changeSpeed(playbackSpeed());
}

void control_bar::muteClicked()
{
    emit muteUnmute(!playerMuted);
}
