#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mainview.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void initView();

private:
    MainView view;
};
#endif // MAINWINDOW_H
