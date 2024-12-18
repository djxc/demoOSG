#include <userScene/housemodel.h>

HouseModel::HouseModel()
{

}

osg::Drawable* HouseModel::createHouseRoof() {
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-0.2, -0.5, 3.5));
    vertices->push_back(osg::Vec3(6.2, -0.5, 3.5));
    vertices->push_back(osg::Vec3(0.8, 2.0, 6.0));
    vertices->push_back(osg::Vec3(5.2, 2.0, 6.0));
    vertices->push_back(osg::Vec3(-0.2, 4.5, 3.5));
    vertices->push_back(osg::Vec3(6.2, 4.5, 3.5));
    osg::ref_ptr<osg::DrawArrays> roof = new osg::DrawArrays(osg::DrawArrays::QUAD_STRIP, 0, 6);
    osg::ref_ptr<osg::DrawElementsUInt> roofSide = new osg::DrawElementsUInt(osg::DrawElementsUInt::TRIANGLES, 6);
    (*roofSide)[0] = 0;
    (*roofSide)[1] = 2;
    (*roofSide)[2] = 4;
    (*roofSide)[3] = 5;
    (*roofSide)[4] = 3;
    (*roofSide)[5] = 1;

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.25, 0.0, 0.0, 1.0));

    osg::ref_ptr<osg::Geometry> houseRoof = new osg::Geometry;
    houseRoof->setVertexArray(vertices.get());

    colors->setBinding(osg::Array::BIND_OVERALL);
    houseRoof->setColorArray(colors.get());

//    houseRoof->setColorBinding(osg::Geometry::BIND_OVERALL);
    houseRoof->addPrimitiveSet(roof.get());
    houseRoof->addPrimitiveSet(roofSide.get());
    osgUtil::SmoothingVisitor smv;
    smv.smooth(*houseRoof);
    return houseRoof.release();
}

osg::Drawable* HouseModel::createHouseWall() {
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0.0, 0.0, 4.0));
    vertices->push_back(osg::Vec3(0.0, 0.0, 0.0));
    vertices->push_back(osg::Vec3(6.0, 0.0, 4.0));
    vertices->push_back(osg::Vec3(6.0, 0.0, 0.0));
    vertices->push_back(osg::Vec3(6.0, 4.0, 4.0));
    vertices->push_back(osg::Vec3(6.0, 4.0, 0.0));
    vertices->push_back(osg::Vec3(0.0, 4.0, 4.0));
    vertices->push_back(osg::Vec3(0.0, 4.0, 0.0));
    vertices->push_back(osg::Vec3(0.0, 0.0, 4.0));
    vertices->push_back(osg::Vec3(0.0, 0.0, 0.0));

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(10);
    (*normals)[0].set(-0.707, -0.707, 0.0);
    (*normals)[1].set(-0.707, -0.707, 0.0);
    (*normals)[2].set(0.707, -0.707, 0.0);
    (*normals)[3].set(0.707, -0.707, 0.0);
    (*normals)[4].set(0.707, 0.707, 0.0);
    (*normals)[5].set(0.707, 0.707, 0.0);
    (*normals)[6].set(-0.707, 0.707, 0.0);
    (*normals)[7].set(-0.707, 0.707, 0.0);
    (*normals)[8].set(-0.707, -0.707, 0.0);
    (*normals)[9].set(-0.707, -0.707, 0.0);

    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array(10);
    (*texcoords)[0].set(0.0, 1.0);
    (*texcoords)[1].set(0.0, 0.0);
    (*texcoords)[2].set(0.3, 1.0);
    (*texcoords)[3].set(0.3, 0.0);
    (*texcoords)[4].set(0.5, 1.0);
    (*texcoords)[5].set(0.5, 0.0);
    (*texcoords)[6].set(0.8, 1.0);
    (*texcoords)[7].set(0.8, 0.0);
    (*texcoords)[8].set(1.0, 1.0);
    (*texcoords)[9].set(1.0, 0.0);

    osg::ref_ptr<osg::Geometry> houseWall = new osg::Geometry;
    houseWall->setVertexArray(vertices.get());
    houseWall->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUAD_STRIP, 0, 10));


    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("D:\\code\\c\\Examples\\data\\wall.png");
    texture->setImage(image.get());
    houseWall->setTexCoordArray(0, texcoords.get());
    houseWall->setNormalArray(normals.get());
    houseWall->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    houseWall->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
    return houseWall.release();
}
