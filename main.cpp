#include "mainwindow.h"

#include <QApplication>

#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>

#include "rotatecallback.h"
#include "infocallback.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    osg::ArgumentParser arguments(&argc, argv);
    osg::Node* model = osgDB::readNodeFiles(arguments);
    if (!model) {
        model = osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cow.osg");
    }
    // 创建一个位置旋转节点，用osg中的智能指针存储；每一帧都会调用回调函数,回调函数是用户继承NodeCallback自己实现的类。这里仅用到更新回调，另外还存在事件回调
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(model);
    pat->setUpdateCallback(new RotateCallback);
    pat->addUpdateCallback(new InfoCallback);
    osgViewer::Viewer viewer;
    viewer.setSceneData(pat.get());
    viewer.setUpViewInWindow(100, 100, 800, 600);
    return viewer.run();
}
