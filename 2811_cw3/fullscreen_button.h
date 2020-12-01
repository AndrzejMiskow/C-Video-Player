#ifndef FULLSCREEN_BUTTON_H
#define FULLSCREEN_BUTTON_H

#include <QWidget>
#include <QPushButton>

class FullscreenButton : public QPushButton
{
    Q_OBJECT
private:
    QWidget* parentWindow;
    bool fullscreen;

public:
    FullscreenButton(QWidget* window) : QPushButton(tr("Fullscreen")){
        parentWindow = window;
        connect(this, SIGNAL(clicked()), this, SLOT(switchScreenState()));
    }

private slots:
    void switchScreenState();
};

#endif // FULLSCREEN_BUTTON_H
