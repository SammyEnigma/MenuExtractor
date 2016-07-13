A class that can enumerate an existing QMenu, and generate XML that can be added to an .ui file.

Usage:

    XmlEnumerator enumerator;
    enumerator.enumerateMenuBar(ui->menuBar);
    qDebug() << enumerator.xmlOutput();


For example if we have this menu bar:

File
    Foo
        Bar1
        Bar2
    Foo2
    <separator>
Edit
    Edit1
    Edit2
Action

It will print out this:

<widget class="QMenu" name="menuFile">
 <property name="title">
  <string>File</string>
 </property>
 <widget class="QMenu" name="menuFoo">
  <property name="title">
   <string>Foo</string>
  </property>
  <addaction name="actionBar1"/>
  <addaction name="actionBar2"/>
 </widget>
 <addaction name="menuFoo"/>
 <addaction name="actionFoo2"/>
 <addaction name="separator"/>
</widget>
<widget class="QMenu" name="menuEdit">
 <property name="title">
  <string>Edit</string>
 </property>
 <addaction name="actionEdit1"/>
 <addaction name="actionEdit2"/>
</widget>
<widget class="QMenu" name="menuAction">
 <property name="title">
  <string>Action</string>
 </property>
</widget>
