#include "mainview.h"
#include "Util.h"
#include <cmath>

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>




MainView::MainView(QWidget *parent): QOpenGLWidget(parent),  QGLFunctions()
{
    timer = new QTimer();
    connect( timer, SIGNAL(timeout()), this, SLOT(update()) );

    setFocusPolicy(Qt::StrongFocus);

    camPosx = 3.0,  camPosy = 5.0,    camPosz = 25.0;
    camViewx = 0.0, camViewy = 0.0, camViewz = 0.0;
    camUpx = 0.0,   camUpy = 1.0,   camUpz = 0.0;

    polygonMode = GL_FILL;

}

void MainView::initializeGL ()
{
    this->dayOfYear = 0;
    this->hourOfDay = 0;

    // Initialize QGLWidget (parent)
    QOpenGLWidget::initializeGL();

    glShadeModel(GL_SMOOTH);

    // Black canvas
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    // Place light
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable(GL_DEPTH_TEST);

    GLfloat light0_position [] = {0.1f, 0.1f, 0.1f, 0.1f};
    GLfloat light_diffuse []={ 1.0, 1.0, 1.0, 1.0 };
    glLightfv ( GL_LIGHT0, GL_POSITION, light0_position );
    glLightfv ( GL_LIGHT0, GL_DIFFUSE, light_diffuse );

    timer->start(50);
}

void MainView::resizeGL ( int width, int height )
{
    if ((width<=0) || (height<=0))
        return;

    //set viewport
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set persepective
    //change the next line order to have a different perspective
    GLdouble aspect_ratio=(GLdouble)width/(GLdouble)height;
    gluPerspective(40.0, aspect_ratio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainView::paintGL()
{
    this->dayOfYear = (this->dayOfYear+1);
    this->hourOfDay = (this->hourOfDay+1) % 24;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // store current matrix
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix( );

    gluLookAt(camPosx ,camPosy ,camPosz,
              camViewx,camViewy,camViewz,
              camUpx, camUpy, camUpz );

    //Draw Axes
    glDisable( GL_LIGHTING );
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();
    glEnable( GL_LIGHTING );

    // rotate the plane of the elliptic
    glRotated ( 5.0, 1.0, 0.0, 0.0 );

    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(20.0, 20.0, 20.0);
    glEnd();
    glFlush();

}

void MainView::keyPressEvent( QKeyEvent * e )
{
    switch (e->key()) {
        case Qt::Key_Control:
            this->camPosy += 0.5;
            this->camViewy += 0.5;
            break;
        case Qt::Key_Space:
            this->camPosy -= 0.5;
            this->camViewy -= 0.5;
            break;
        case Qt::Key_W:
            this->camPosz -= 0.5;
            this->camViewz -= 0.5;
            break;
        case Qt::Key_S:
            this->camPosz += 0.5;
            this->camViewz += 0.5;
            break;
        case Qt::Key_A:
            this->camPosx -= 0.5;
            this->camViewx -= 0.5;
            break;
        case Qt::Key_D:
            this->camPosx += 0.5;
            this->camViewx += 0.5;
        case Qt::Key_Right:
            this->camPosx += 0.5;
            break;
        case Qt::Key_Left:
            this->camPosx -= 0.5;
            break;
        case Qt::Key_End:
            this->close();
            break;
        case Qt::Key_M:
            if ( polygonMode == GL_LINE )
                polygonMode =  GL_FILL;
            else
                polygonMode = GL_LINE;
            break;

    }
}
