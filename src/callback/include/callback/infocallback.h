#ifndef INFOCALLBACK_H
#define INFOCALLBACK_H

#include <iostream>

#include <osg/PositionAttitudeTransform>

class InfoCallback:public osg::NodeCallback
{
public:
    InfoCallback();
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};

#endif // INFOCALLBACK_H
