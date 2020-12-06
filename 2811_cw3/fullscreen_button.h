#ifndef FULLSCREEN_BUTTON_H
#define FULLSCREEN_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QTabWidget>
#include <map>

class FullscreenButton : public QPushButton
{
    Q_OBJECT
private:
    QWidget* parentWindow;
    std::map<QWidget*, bool> previousState;


    void hideHierarchy(QWidget*, QWidget*);
    void showHierarchy(QWidget*, QWidget*);
public:
    FullscreenButton(QWidget* window) : QPushButton(tr("Fullscreen")){
        parentWindow = window;
        connect(this, SIGNAL(clicked()), this, SLOT(switchScreenState()));//set button to switchstates when clicked
    }

private slots:
    void switchScreenState();//template for switchstate slot/function
};

#endif // FULLSCREEN_BUTTON_H
