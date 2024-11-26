#ifndef ROTATECALLBACK_H
#define ROTATECALLBACK_H

#include <iostream>

#include <osg/PositionAttitudeTransform>

class RotateCallback:public osg::NodeCallback
{
public:
    RotateCallback();
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
    double _rotateZ;
};

#endif // ROTATECALLBACK_H
