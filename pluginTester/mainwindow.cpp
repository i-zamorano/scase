#include "mainwindow.h"

#include "documentwriterplugin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DocumentWriterPlugin dwp();
}

MainWindow::~MainWindow()
{

}
