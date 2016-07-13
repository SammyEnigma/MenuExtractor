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

        // Now write all the nested actions and submenus.
        foreach (QAction *ii, menu->actions()) {
            // There are 3 cases - a sub-menu, an ordinary action, or a separator.
            if (ii->isSeparator()) {
                // <addaction name="separator"/>
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", "separator");
            } else if (ii->menu()) {
                // <addaction name="menuFoo"/>
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", ii->menu()->objectName());

                // This is not strictly how Qt Designer generates the XML.
                // It first writes all the sub-menu widget declarations,
                // and only then the addactions, but I think the sub-menu
                // declarations have no influence on the menu-order. They
                // can be placed wherever you want.
                enumerateMenu(ii->menu());
            } else {
                writer.writeEmptyElement("addaction");
                writer.writeAttribute("name", ii->objectName());
            }
        }

        // Close the widget XML tag.
        writer.writeEndElement();
    }

    void enumerateAction(QAction *action)
    {
        // This is how the XML looks:
        //
        // <action name="actionBar1">
        //  <property name="text">
        //   <string>Bar1</string>
        //  </property>
        // </action>

        writer.writeStartElement("action");
        writer.writeAttribute("name", action->objectName());
        writer.writeStartElement("property");
        writer.writeAttribute("name", "text");
        writer.writeTextElement("string", action->text());
        writer.writeEndElement();
        writer.writeEndElement();
    }

    // I use a separate function so it can receive a QMenuBar pointer.
    // Otherwise it's the same code as enumerateMenu().
    void enumerateMenuBar(QMenuBar *menuBar)
    {
        foreach (QAction *ii, menuBar->actions()) {
            if (ii->menu()) {
                enumerateMenu(ii->menu());
            }
        }
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
