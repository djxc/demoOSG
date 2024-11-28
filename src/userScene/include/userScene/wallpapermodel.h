#ifndef WALLPAPERMODEL_H
#define WALLPAPERMODEL_H

#include <osg/Node>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/NodeCallback>
#include <osg/PolygonOffset>

#include <osgDB/ReadFile>
#include <osgText/Text>

#include <callback/imageupdatecallback.h>
#include <callback/wrapcallback.h>
#include <callback/anisotropiccallback.h>
#include <callback/filtercallback.h>

class WallPaperModel
{
public:
    WallPaperModel();
    osg::Node* createSubloadWall(osg::BoundingBox& bb);
    osg::Node* createWrapWall(osg::BoundingBox& bb,const std::string& filename);
    osg::Node* createAnisotripicWall(osg::BoundingBox& bb,const std::string& filename);
    osg::Node* createFilterWall(osg::BoundingBox& bb,const std::string& filename);
};

#endif // WALLPAPERMODEL_H
