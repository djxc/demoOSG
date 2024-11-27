#include <callback/cessnacallback.h>

CessnaCallback::CessnaCallback()
{
}

void CessnaCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
    osg::Switch* cessnaSwitch = dynamic_cast<osg::Switch*>(node);
    if (cessnaSwitch && nv) {
        const osg::FrameStamp* framStamp = nv->getFrameStamp();
        if (framStamp) {
            if (_fireStartFrame < framStamp->getFrameNumber()) {
                cessnaSwitch->setValue(0, false);
                cessnaSwitch->setValue(1, true);
            }
        }
    }
    traverse(node, nv);
}
