#include "mainwindow.h"

#include <QApplication>

#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/AutoTransform>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/LOD>

#include "rotatecallback.h"
#include "infocallback.h"
#include "cessnacallback.h"
#include "housemodel.h"

osg::Transform* createAutoTransform(double posX, osg::Node* model) {
    osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform;
    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
    at->setPosition(osg::Vec3(posX, 0.0, 0.0));
    at->addChild(model);
    return at.release();
}

osg::Transform* createMatrixTransform(double posX, double rotateZ, osg::Node* model) {
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->setMatrix(osg::Matrix::rotate(rotateZ, osg::Z_AXIS) * osg::Matrix::translate(posX, 0.0, 0.0));
    mt->addChild(model);
    return mt.release();
}

osg::Transform* createPositionAttitudeTransform(double posX, double rotateZ, osg::Node* model) {
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->setPosition(osg::Vec3(posX, 0.0, 0.0));
    pat->setAttitude(osg::Quat(rotateZ, osg::Z_AXIS));
    pat->addChild(model);
    return pat.release();
}

osg::ref_ptr<osg::Node> demoCallback(osg::Node* model) {
    // 创建一个位置旋转节点，用osg中的智能指针存储；每一帧都会调用回调函数,回调函数是用户继承NodeCallback自己实现的类。这里仅用到更新回调，另外还存在事件回调
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(model);
    pat->setUpdateCallback(new RotateCallback);
    pat->addUpdateCallback(new InfoCallback);
    return pat;
}

osg::ref_ptr<osg::Node> demoTransform(osg::Node* model) {
    // 创建一个组节点，添加3个子节点，每个子节点都是transform节点，相同的model都是transform的叶子节点，即一个model有三个示例。
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(createMatrixTransform(-5.0, osg::PI/4, model));
    root->addChild(createAutoTransform(0.0, model));
    root->addChild(createPositionAttitudeTransform(5.0, -osg::PI/4, model));
    return root;
}

osg::ref_ptr<osg::Node>  demoSwitch() {
    // 开关节点，通过设置叶子节点的true和false控制模型的显示或隐藏。这里采用更新回调
    osg::ref_ptr<osg::Switch> root1 = new osg::Switch;
    root1->addChild(osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessna.osg"), true);
    root1->addChild(osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessnafire.osg"), false);
    root1->setUpdateCallback(new CessnaCallback);
    return root1;
}

osg::ref_ptr<osg::Node> demoLOD() {
    osg::Node* model = osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-high.ive");
    float r = model->getBound().radius();
    osg::ref_ptr<osg::LOD> root = new osg::LOD;
    root->addChild(osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-low.ive"), r * 7, FLT_MAX);
    root->addChild(osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-mid.ive"), r * 3, r * 7);
    root->addChild(model, 0.0, r * 3);
    return root;
}


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

//    osg::ref_ptr<osg::Node> root = demoLOD(); // demoCallback(model); // demoTransform(model); // demoSwitch();

    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    HouseModel* housemodel = new HouseModel;
    root->addDrawable(housemodel->createHouseWall());
    root->addDrawable(housemodel->createHouseRoof());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(100, 100, 800, 600);
    return viewer.run();
}
