#ifndef CESSNACALLBACK_H
#define CESSNACALLBACK_H

#include <osg/NodeCallback>
#include <osg/Switch>

class CessnaCallback:public osg::NodeCallback
{
public:
    static const int _fireStartFrame = 120;

    CessnaCallback();
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};

#endif // CESSNACALLBACK_H
