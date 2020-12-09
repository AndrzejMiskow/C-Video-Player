#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H

#include <QWidget>
#include <QMediaPlayer>
#include <QToolButton>
#include <QSlider>
#include <QComboBox>
#include <QScrollBar>

class ControlBar : public QWidget
{
    Q_OBJECT
public:
    ControlBar(QWidget *parent = 0, QMediaPlayer *player = 0);
    //Getters
    QMediaPlayer::State state();
    int volume();
    bool isMuted();
    qreal playbackSpeed();

private:
    QMediaPlayer::State playerState;
    bool playerMuted;
    //Buttons, combobox and slider to adjust settings
    QToolButton *playButton;
    QToolButton *muteButton;
    QScrollBar *volumeSlider;
    QComboBox *speedBox;

signals:
    //Self explanatory signals
    void play();
    void pause();
    void changeVolume(int volume);
    void muteUnmute(bool mute);
    void changeSpeed(qreal speed);

public slots:
    //Slots to update the values of the sliders,buttons, etc
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMute(bool mute);
    void setSpeed(float rate);

private slots:
    //Private slots to react to interactions with the buttons etc
    void playClicked();
    void muteClicked();
    void updateSpeed();

};

#endif // CONTROL_BAR_H
