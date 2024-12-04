#ifndef DEMODJ_H
#define DEMODJ_H

#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/LOD>
#include <osg/Geode>
#include <osg/TexGen>
#include <osg/StateSet>
#include <osg/LineWidth>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/DrawPixels>
#include <osg/DrawPixels>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osg/PolygonOffset>
#include <osg/TextureCubeMap>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgViewer/ViewerEventHandlers> //事件监听

#include <osgGA/DriveManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>

#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier> //简化几何体
#include <osgUtil/DelaunayTriangulator>


#include <callback/rotatecallback.h>
#include <callback/infocallback.h>
#include <callback/cessnacallback.h>
#include <callback/dynamiclinecallback.h>

#include <userScene/housemodel.h>
#include <userScene/wallpapermodel.h>

#include <handlers/keyboardhandler.h>

class demoDj
{
public:
    demoDj();
    osg::Transform* createAutoTransform(double posX, osg::Node* model);
    osg::Transform* createMatrixTransform(double posX, double rotateZ, osg::Node* model);
    osg::Transform* createPositionAttitudeTransform(double posX, double rotateZ, osg::Node* model);
    osg::ref_ptr<osg::Node> demoTransform(osg::Node* model);
    osg::ref_ptr<osg::Node> demoCallback(osg::Node* model);
    osg::ref_ptr<osg::Node>  demoSwitch();
    osg::ref_ptr<osg::Node> demoLOD();
    osg::ref_ptr<osg::Node> demoChangeGeometry();
    osg::ref_ptr<osg::Node> drawHouse();
    osg::ref_ptr<osg::Node> drawBitmap();
    void createTexture1D( osg::StateSet& ss );
    void createTexture2D( osg::StateSet& ss );
    void createTextureCubeMap( osg::StateSet& ss );
    osg::ref_ptr<osg::Node> demoTexture();
    osg::Node* readCowModel();
    osg::ref_ptr<osg::Geode> createBox();
    osg::ref_ptr<osg::Node> createWallPaperModel();
    osg::ref_ptr<osg::Node> demoKeyEvent();
};

#endif // DEMODJ_H
