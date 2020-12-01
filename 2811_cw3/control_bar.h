#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAbstractButton>
#include <QAbstractSlider>
#include <QComboBox>

class ControlBar : public QWidget
{
    Q_OBJECT
public:
    ControlBar(QWidget *parent = 0, QMediaPlayer *player = 0);
    QMediaPlayer::State state();
    int volume();
    bool isMuted();
    qreal playbackSpeed();

private:
    QMediaPlayer::State playerState;
    bool playerMuted;
    QAbstractButton *playButton;
    QAbstractButton *muteButton;
    QAbstractSlider *volumeSlider;
    QComboBox *speedBox;
    void makeConnections(QMediaPlayer);

signals:
    void play();
    void pause();
    void changeVolume(int volume);
    void muteUnmute(bool mute);
    void changeSpeed(qreal speed);

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
