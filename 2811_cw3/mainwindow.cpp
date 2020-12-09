#include "mainwindow.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    //self explain initial setup
    QAction *actPlay;
    QAction *actPause;
    QAction *actContinue;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *filebar;
    QMenu *controlbar;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        // setupUi

        if (MainWindow -> objectName().isEmpty()){
            MainWindow -> setObjectName(QString::fromUtf8("MainWindow"));
        }
        MainWindow -> resize( 800, 600 );
        //create actions
        //need to add more act on file and help
        actPlay = new QAction( MainWindow );
        actPlay -> setObjectName( QString::fromUtf8( "actPlay" ) );
        actPause = new QAction(MainWindow);
        actPause -> setObjectName( QString::fromUtf8( "actPause" ) );
        actContinue = new QAction(MainWindow);
        actContinue->setObjectName(QString::fromUtf8( "actContinue" ));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8( "centralwidget" ));
        MainWindow->setCentralWidget( centralwidget );
        menubar = new QMenuBar( MainWindow );
        menubar -> setObjectName(QString::fromUtf8( "menubar" ));
        menubar -> setGeometry( QRect(0, 0, 800, 23) );

        //setup each buttons in the tool bar
        filebar = new QMenu( menubar) ;
        filebar -> setObjectName( QString :: fromUtf8( "filebar" ));
        controlbar = new QMenu(menubar);
        controlbar -> setObjectName( QString::fromUtf8( "controlbar" ) );
        menuHelp = new QMenu(menubar);
        menuHelp -> setObjectName( QString :: fromUtf8( "menuHelp" ) );
        MainWindow -> setMenuBar( menubar );
        statusbar = new QStatusBar( MainWindow );
        statusbar -> setObjectName( QString :: fromUtf8( "statusbar" ) );
        MainWindow -> setStatusBar( statusbar );

        //add each bar into each catagory
        menubar -> addAction( filebar -> menuAction() );
        menubar -> addAction( controlbar -> menuAction() );
        menubar -> addAction( menuHelp -> menuAction());
        controlbar -> addAction( actPlay );
        controlbar -> addAction( actPause );
        controlbar -> addAction( actContinue );

        retranslateUi( MainWindow );

        QMetaObject::connectSlotsByName( MainWindow );
    }

    void retranslateUi(QMainWindow *MainWindow)
    {// retranslate.Ui file
        MainWindow -> setWindowTitle( QApplication :: translate ( "MainWindow", "MainWindow", nullptr ));
        actPlay -> setText ( QApplication :: translate ( "MainWindow", "Play", nullptr ));
        actPause -> setText( QApplication :: translate ( "MainWindow", "Pause", nullptr ));
        actContinue -> setText( QApplication :: translate( "MainWindow", "Continue", nullptr ));
        filebar -> setTitle( QApplication :: translate( "MainWindow", "File", nullptr ));
        controlbar -> setTitle( QApplication :: translate( "MainWindow", "Control", nullptr ));
        menuHelp -> setTitle( QApplication :: translate( "MainWindow", "Help", nullptr ));
    }

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

MainWindow :: MainWindow( QMediaPlayer *player ) :
    QMainWindow( nullptr ),
    ui( new Ui :: MainWindow ),
    player( player )
{
    //initial setup for the main window : slider
    ui -> setupUi( this );
    slider = new QSlider( Qt::Horizontal );
    slider->setObjectName("videoLength");
    slider -> setMinimum( 0 );

    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect()
{
    //connect the signals of each action with the tool bar actions
    connect( ui -> actPlay, &QAction :: triggered, [this]{player->play();});
    connect( ui -> actPause, &QAction :: triggered, [this]{player->pause();});
    connect( ui -> actContinue,&QAction::triggered, [this]{player->play();});
    connect( player, &QMediaPlayer :: durationChanged, [this]( qint64 duration){
        slider -> setMaximum( duration );
    });

    //connect the signals when changing the position of the slider
    connect( player, &QMediaPlayer :: positionChanged, [this]( qint64 position ){
        mediaChanged = true;
        slider->setValue( position );
        mediaChanged = false;
    });

    //connect the siganls of the video with the process bar
    connect(slider, &QSlider :: valueChanged, [this]( int value ){
        if( !mediaChanged ){
            player->setPosition( value );
        }
    });
}
