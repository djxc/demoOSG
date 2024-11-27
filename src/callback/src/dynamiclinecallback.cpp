#include <callback/dynamiclinecallback.h>

DynamicLineCallback::DynamicLineCallback():_angle(0.0){}


void DynamicLineCallback::update(osg::NodeVisitor* nv, osg::Drawable* drawable) {
    osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
    if (!geom) return;

    osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
    if (vertices) {
        for (osg::Vec3Array::iterator itr = vertices->begin(); itr != vertices->end() - 1; ++itr) {
            itr->set((*(itr + 1)));
        }
        _angle += 1.0/20.0;
        osg::Vec3& pt = vertices->back();
        pt.set(10.0*cos(_angle), 0.0, 10.0*sin(_angle));
        vertices->dirty();
    }
}
