#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAbstractButton>
#include <QAbstractSlider>
#include <QComboBox>

class control_bar : public QWidget
{
    Q_OBJECT
public:
    control_bar(QWidget *parent);
    QMediaPlayer::State state();
    int volume();
    bool isMuted();
    float playbackSpeed();

private:
    QMediaPlayer::State playerState;
    bool playerMuted;
    QAbstractButton *playButton;
    QAbstractButton *muteButton;
    QAbstractSlider *volumeSlider;
    QComboBox *speedBox;

signals:
    void play();
    void pause();
    void changeVolume(int volume);
    void muteUnmute(bool mute);
    void changeSpeed(float speed);

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMute(bool mute);
    void setSpeed(float rate);

private slots:
    void playClicked();
    void muteClicked();
    void updateSpeed();

};

#endif // CONTROL_BAR_H
