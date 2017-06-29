#ifndef THREEDWINDOW_H
#define THREEDWINDOW_H
#include <QAction>
#include <QMainWindow>

class threeDWindow : public QMainWindow
{
    Q_OBJECT
public:
    threeDWindow(QWidget* pParent = 0);
    //void CreateMenu( void );
private slots:
    //void ToggleFullScreen( void );
    //void About( void );
private:
    //QAction*            m_pToggleFullScreen;
    //QAction             *m_pAbout, *m_pAboutQt;
};

#endif // THREEDWINDOW_H
