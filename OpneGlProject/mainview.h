#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QGLFunctions>

class MainView : public QOpenGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    MainView(QWidget *parent = nullptr);

protected:
    void initializeGL ();
    void resizeGL ( int width, int height );
    void paintGL ();

    void keyPressEvent(QKeyEvent * e);

private:
    GLint polygonMode;
    int dayOfYear, hourOfDay;
    double camPosx,camPosy,camPosz;
    double camUpx,camUpy,camUpz;
    double camViewx,camViewy,camViewz;
    QTimer* timer;
};

#endif // MAINVIEW_H
