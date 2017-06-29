#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include "GLWidget.h"
#include "threeDWindow.h"
threeDWindow::threeDWindow( QWidget* pParent ):
    QMainWindow( pParent ) {
    //CreateMenu( );
    qDebug("window");
    GLWidget* pGLWidget = new GLWidget( this );
    //connect( pGLWidget, SIGNAL( ToggleFullScreen( ) ), this, SLOT( ToggleFullScreen( ) ) );
    setCentralWidget( pGLWidget );
    resize( pGLWidget->width( ), pGLWidget->height( ));
    setWindowTitle( tr( "Color cube example" ) );           // 设置窗口标题
    qDebug("end");
}

/*
void threeDWindow::CreateMenu( void )
{
    // 创建菜单
    QMenu* pOption = new QMenu( tr( "Option" ), this );
    QMenu* pAbout = new QMenu( tr( "About" ), this );

    pOption->addAction( QIcon( ":/Icons/fullscreenIcon.png"  ),
                        tr( "Toggle full screen" ), this,
                        SLOT( ToggleFullScreen( ) ),
                        QKeySequence::FullScreen );
    pAbout->addAction( QIcon( ":/Icons/aboutIcon.png" ),
                       tr( "About..." ), this,
                       SLOT( About( ) ),
                       tr( "F1" ) );
    pAbout->addAction( QIcon( ":/Icons/qtIcon.png" ),
                       tr( "About Qt..." ), qApp,
                       SLOT( aboutQt( ) ),
                       tr( "F2" ) );

    // 在菜单栏中添加菜单
    menuBar( )->addMenu( pOption );
    menuBar( )->addMenu( pAbout );
}
*/

/*
void threeDWindow::About( void )
{
    QMessageBox::information( this,
                              tr( "Program Information" ),
                              tr( "<h1><font color=red>Made By Wangyanan</font></h1>\ncreated in July 2013" ) );
}
*/
/*
void threeDWindow::ToggleFullScreen( void )
{
    static bool fullScreen = true;

    if ( fullScreen )
    {
        menuBar( )->hide( );
        showFullScreen( );
    }
    else
    {
        menuBar( )->show( );
        showNormal( );
    }
    fullScreen = !fullScreen;
}
*/
