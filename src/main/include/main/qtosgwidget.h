#ifndef QTOSGWIDGET_H
#define QTOSGWIDGET_H

#include <osgQOpenGL/osgQOpenGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include <osg/ShapeDrawable>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>

#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>

#include <osgViewer/GraphicsWindow>
#include <osgParticle/PrecipitationEffect>

#include <precipitations/conduitprecipitation.h>

class QtOsgWidget: public osgQOpenGLWidget
{
public:
    QtOsgWidget(QWidget *parent = nullptr);

protected:
    void initializeGL()override;
    void paintGL()override;
    void resizeGL(int w, int h)override;
    //
    void mouseDoubleClickEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void wheelEvent(QWheelEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;

    bool event(QEvent *event) override;
private:
    osgGA::EventQueue* getEventQueue()const;
    void setKeyboardModifiers(QInputEvent* event);

private:
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
    osg::ref_ptr<osgViewer::Viewer> _mViewer;
};

#endif // QTOSGWIDGET_H
