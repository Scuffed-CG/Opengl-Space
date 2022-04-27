#include "mainwindow.h"

MainWindow::MainWindow()
{
    setWindowTitle("OpenGlProject");
    resize(640, 480);
    initView();
}

void MainWindow::initView()
{
    setCentralWidget(&view);
}


