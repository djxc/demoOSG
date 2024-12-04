#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>

#include <main/mainwindow.h>
#include <main/qtosgwidget.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(QSize(800, 600));
    QtOsgWidget* widget = new QtOsgWidget();
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
