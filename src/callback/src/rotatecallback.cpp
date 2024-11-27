#include <callback/rotatecallback.h>

RotateCallback::RotateCallback()
    :osg::NodeCallback()
{
    _rotateZ = 0.0;
}

void RotateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
    osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
    if (pat) {
        osg::Quat quat(osg::DegreesToRadians(_rotateZ), osg::Z_AXIS);
        pat->setAttitude(quat);
        _rotateZ += 1.0;
    }
    traverse(node, nv);
}
