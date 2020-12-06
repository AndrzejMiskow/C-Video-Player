#ifndef GALLERY_WIDGET_H
#define GALLERY_WIDGET_H

#include <QWidget>
#include <vector>
#include "the_button.h"
#include "the_player.h"
#include "vid_object.h"
#include <QUrl>
#include <QString>
#include <QDir>
#include <QIcon>
#include <QDirIterator>
#include <QImageReader>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>

class GalleryWidget : public QWidget
{
    Q_OBJECT
private:
    ThePlayer* player;
    QList<vid_object> vids;
    QWidget* buttonDisplay;
    QComboBox* sortBox;
    QLineEdit* text;

    QList<int> vidsToDisplay;//contains the indexes of which vids should be placed in buttonDisplay. Currently setup to always hold all vids

    void allVideosDisplayable();
public:
    GalleryWidget(ThePlayer* player, QString dirAddress);

    ~GalleryWidget(){
        delete buttonDisplay;//not strictly necessary, but good practice
        delete sortBox;
    }

    void addVid(QString);
public slots:
    void replaceButtons();
    void changeDisplayedVideos(QString);

signals:
    void reinstance();
};

#endif // GALLERY_WIDGET_H
