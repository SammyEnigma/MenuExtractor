#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QXmlStreamWriter>

class XmlEnumerator
{
public:
    XmlEnumerator() : writer(&buffer) {
        writer.setAutoFormatting(true);
        // The indentation is by 1 in the UI.
        writer.setAutoFormattingIndent(1);
    }

    QString xmlOutput() { return buffer; }

    void enumerateMenuActions(const QList<QAction*> actions)
    {
        // Write all the sub-menus before we write any of the actions,
        // because we
        foreach (QAction *ii, actions) {
            if (ii->menu()) {
                enumerateMenu(ii->menu());
            }
        }

        // Now write all the nested actions and submenus.
        foreach (QAction *ii, actions) {
            // There are 3 cases - a sub-menu, an ordinary action, or a separator.
            if (ii->isSeparator()) {
                // <addaction name="separator"/>
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", "separator");
            } else if (ii->menu()) {
                // <addaction name="menuFoo"/>
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", ii->menu()->objectName());
            } else {
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", ii->objectName());
            }
        }
    }

    void enumerateMenu(QMenu *menu)
    {
        // This is how the XML looks:
        //
        // <widget class="QMenu" name="menuFoo">
        //  <property name="title">
        //   <string>Foo</string>
        //  </property>
        //  ................
        // </widget>

        // First we write the header of the menu.
        writer.writeStartElement("widget");
        writer.writeAttribute("class", "QMenu");
        writer.writeAttribute("name", menu->objectName());
        writer.writeStartElement("property");
        writer.writeAttribute("name", "title");
        writer.writeTextElement("string", menu->title());
        writer.writeEndElement();

        enumerateMenuActions(menu->actions());

        // Close the widget XML tag.
        writer.writeEndElement();
    }

    // I use a separate function so it can receive a QMenuBar pointer.
    // Otherwise it's the same code as enumerateMenu().
    void enumerateMenuBar(QMenuBar *menuBar)
    {
        enumerateMenuActions(menuBar->actions());
    }

private:
    QList<QAction*> actions;
    QString buffer;
    QXmlStreamWriter writer;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    XmlEnumerator enumerator;
    enumerator.enumerateMenuBar(ui->menuBar);
    qDebug().noquote() << enumerator.xmlOutput();
}

MainWindow::~MainWindow()
{
    delete ui;
}
