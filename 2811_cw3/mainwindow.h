#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QMediaPlayer* player);
    ~MainWindow();

public:
    QSlider* slider;

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    bool mediaChanged = false;

    void initConnect();
};

#endif // MAINWINDOW_H
