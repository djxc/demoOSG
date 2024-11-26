#include "infocallback.h"

InfoCallback::InfoCallback()
{

}

void InfoCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
    osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
    if (pat) {
        double angle = 0.0;
        osg::Vec3f axis;
        pat->getAttitude().getRotate(angle, axis);
        std::cout << "Node is rotating around the (x: " << axis.x() << "; y: " << axis.y() << "; z: " << axis.z() << ")axis," << osg::RadiansToDegrees(angle) << " degrees" << std::endl;
    }
    traverse(node, nv);
}
