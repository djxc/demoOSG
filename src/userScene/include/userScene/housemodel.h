#ifndef HOUSEMODEL_H
#define HOUSEMODEL_H

#include <osg/Geometry>
#include <osgUtil/SmoothingVisitor>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include<osg/StateSet>
#include<osg/TexGen>
#include<osg/TexEnv>

class HouseModel
{
public:
    HouseModel();
    osg::Drawable* createHouseWall();
    osg::Drawable* createHouseRoof();
};

#endif // HOUSEMODEL_H
