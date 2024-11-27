#ifndef DYNAMICLINECALLBACK_H
#define DYNAMICLINECALLBACK_H

#include <osg/Drawable>
#include <osg/Geometry>

class DynamicLineCallback:public osg::Drawable::UpdateCallback
{
public:
    DynamicLineCallback();
    virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable);

protected:
    float _angle;
};

#endif // DYNAMICLINECALLBACK_H
