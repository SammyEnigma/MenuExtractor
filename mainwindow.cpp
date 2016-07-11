#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

static void enumerateMenu(QMenu *menu, int level)
{
    QString s(level*2, ' ');
    foreach (QObject *ii, menu->findChildren<QObject*>()) {
        if (QMenu *menu = qobject_cast<QMenu*>(ii)) {
            qDebug().noquote() << s << menu;
            enumerateMenu(menu, level+1);
        } else if (QAction *action = qobject_cast<QAction *>(ii)) {
            qDebug().noquote() << s << action << (action->isSeparator() ? QString("separator") : action->text());
        }
    }
}

// I use a separate function so it can receive a QMenuBar pointer.
// Otherwise it's the same code as enumerateMenu().
static void enumerateMenuBar(QMenuBar *menuBar)
{
    foreach (QMenu *menu, menuBar->findChildren<QMenu*>()) {
        qDebug() << menu;
        enumerateMenu(menu, 1);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    enumerateMenuBar(ui->menuBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}
