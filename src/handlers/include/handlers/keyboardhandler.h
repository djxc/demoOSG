#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>

class KeyboardHandler: public osgGA::GUIEventHandler
{
public:
    KeyboardHandler();
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa,
                             osg::Object*, osg::NodeVisitor* );
};

#endif // KEYBOARDHANDLER_H
