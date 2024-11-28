#include <userScene/wallpapermodel.h>

WallPaperModel::WallPaperModel()
{

}


osg::Node* WallPaperModel::createSubloadWall(osg::BoundingBox& bb)
{
    osg::Group* group = new osg::Group;

    // left hand side of bounding box.
    osg::Vec3 top_left(bb.xMin(),bb.yMax(),bb.zMax());
    osg::Vec3 bottom_left(bb.xMin(),bb.yMax(),bb.zMin());
    osg::Vec3 bottom_right(bb.xMax(),bb.yMax(),bb.zMin());
    osg::Vec3 top_right(bb.xMax(),bb.yMax(),bb.zMax());
    osg::Vec3 center((bb.xMax()+bb.xMin())*0.5f,bb.yMax(),(bb.zMin()+bb.zMax())*0.5f);
    float height = bb.zMax()-bb.zMin();

    // create the geometry for the wall.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = top_left;
    (*vertices)[1] = bottom_left;
    (*vertices)[2] = bottom_right;
    (*vertices)[3] = top_right;
    geom->setVertexArray(vertices);

    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    (*texcoords)[0].set(0.0f,1.0f);
    (*texcoords)[1].set(0.0f,0.0f);
    (*texcoords)[2].set(1.0f,0.0f);
    (*texcoords)[3].set(1.0f,1.0f);
    geom->setTexCoordArray(0,texcoords);

    osg::Vec3Array* normals = new osg::Vec3Array(1);
    (*normals)[0].set(0.0f,-1.0f,0.0f);
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

    osg::Geode* geom_geode = new osg::Geode;
    geom_geode->addDrawable(geom);
    group->addChild(geom_geode);


    // set up the texture state.
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
    texture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);

    osg::StateSet* stateset = geom->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

    // create the text label.

    osgText::Text* text = new osgText::Text;
    text->setDataVariance(osg::Object::DYNAMIC);
    text->setFont("D:\\code\\c\\OpenSceneGraph-Data\\fonts\\arial.ttf");
    text->setPosition(center);
    text->setCharacterSize(height*0.03f);
    text->setAlignment(osgText::Text::CENTER_CENTER);
    text->setAxisAlignment(osgText::Text::XZ_PLANE);

    osg::Geode* text_geode = new osg::Geode;
    text_geode->addDrawable(text);

    osg::StateSet* text_stateset = text_geode->getOrCreateStateSet();
    text_stateset->setAttributeAndModes(new osg::PolygonOffset(-1.0f,-1.0f),osg::StateAttribute::ON);

    group->addChild(text_geode);

    // set the update callback to cycle through the various min and mag filter modes.
    group->setUpdateCallback(new ImageUpdateCallback(texture,text));

    return group;

}


osg::Node* WallPaperModel::createWrapWall(osg::BoundingBox& bb,const std::string& filename)
{
    osg::Group* group = new osg::Group;

    // left hand side of bounding box.
    osg::Vec3 top_left(bb.xMax(),bb.yMax(),bb.zMax());
    osg::Vec3 bottom_left(bb.xMax(),bb.yMax(),bb.zMin());
    osg::Vec3 bottom_right(bb.xMax(),bb.yMin(),bb.zMin());
    osg::Vec3 top_right(bb.xMax(),bb.yMin(),bb.zMax());
    osg::Vec3 center(bb.xMax(),(bb.yMin()+bb.yMax())*0.5f,(bb.zMin()+bb.zMax())*0.5f);
    float height = bb.zMax()-bb.zMin();

    // create the geometry for the wall.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = top_left;
    (*vertices)[1] = bottom_left;
    (*vertices)[2] = bottom_right;
    (*vertices)[3] = top_right;
    geom->setVertexArray(vertices);

    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    (*texcoords)[0].set(-1.0f,2.0f);
    (*texcoords)[1].set(-1.0f,-1.0f);
    (*texcoords)[2].set(2.0f,-1.0f);
    (*texcoords)[3].set(2.0f,2.0f);
    geom->setTexCoordArray(0,texcoords);

    osg::Vec3Array* normals = new osg::Vec3Array(1);
    (*normals)[0].set(-1.0f,0.0f,0.0f);
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

    osg::Geode* geom_geode = new osg::Geode;
    geom_geode->addDrawable(geom);
    group->addChild(geom_geode);


    // set up the texture state.
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setBorderColor(osg::Vec4(1.0f,1.0f,1.0f,0.5f)); // only used when wrap is set to CLAMP_TO_BORDER
    texture->setImage(osgDB::readRefImageFile(filename));

    osg::StateSet* stateset = geom->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    // create the text label.

    osgText::Text* text = new osgText::Text;
    text->setDataVariance(osg::Object::DYNAMIC);
    text->setFont("fonts/arial.ttf");
    text->setPosition(center);
    text->setCharacterSize(height*0.03f);
    text->setAlignment(osgText::Text::CENTER_CENTER);
    text->setAxisAlignment(osgText::Text::YZ_PLANE);

    osg::Geode* text_geode = new osg::Geode;
    text_geode->addDrawable(text);

    osg::StateSet* text_stateset = text_geode->getOrCreateStateSet();
    text_stateset->setAttributeAndModes(new osg::PolygonOffset(-1.0f,-1.0f),osg::StateAttribute::ON);

    group->addChild(text_geode);

    // set the update callback to cycle through the various min and mag filter modes.
    group->setUpdateCallback(new WrapCallback(texture,text));

    return group;

}


osg::Node* WallPaperModel::createFilterWall(osg::BoundingBox& bb,const std::string& filename)
{
    osg::Group* group = new osg::Group;

    // left hand side of bounding box.
    osg::Vec3 top_left(bb.xMin(),bb.yMin(),bb.zMax());
    osg::Vec3 bottom_left(bb.xMin(),bb.yMin(),bb.zMin());
    osg::Vec3 bottom_right(bb.xMin(),bb.yMax(),bb.zMin());
    osg::Vec3 top_right(bb.xMin(),bb.yMax(),bb.zMax());
    osg::Vec3 center(bb.xMin(),(bb.yMin()+bb.yMax())*0.5f,(bb.zMin()+bb.zMax())*0.5f);
    float height = bb.zMax()-bb.zMin();

    // create the geometry for the wall.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = top_left;
    (*vertices)[1] = bottom_left;
    (*vertices)[2] = bottom_right;
    (*vertices)[3] = top_right;
    geom->setVertexArray(vertices);

    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    (*texcoords)[0].set(0.0f,1.0f);
    (*texcoords)[1].set(0.0f,0.0f);
    (*texcoords)[2].set(1.0f,0.0f);
    (*texcoords)[3].set(1.0f,1.0f);
    geom->setTexCoordArray(0,texcoords);

    osg::Vec3Array* normals = new osg::Vec3Array(1);
    (*normals)[0].set(1.0f,0.0f,0.0f);
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

    osg::Geode* geom_geode = new osg::Geode;
    geom_geode->addDrawable(geom);
    group->addChild(geom_geode);


    // set up the texture state.
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setImage(osgDB::readRefImageFile(filename));

    osg::StateSet* stateset = geom->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

    // create the text label.

    osgText::Text* text = new osgText::Text;
    text->setDataVariance(osg::Object::DYNAMIC);

    text->setFont("D:\\code\\c\\OpenSceneGraph-Data\\fonts\\arial.ttf");
    text->setPosition(center);
    text->setCharacterSize(height*0.03f);
    text->setAlignment(osgText::Text::CENTER_CENTER);
    text->setAxisAlignment(osgText::Text::YZ_PLANE);

    osg::Geode* text_geode = new osg::Geode;
    text_geode->addDrawable(text);

    osg::StateSet* text_stateset = text_geode->getOrCreateStateSet();
    text_stateset->setAttributeAndModes(new osg::PolygonOffset(-1.0f,-1.0f),osg::StateAttribute::ON);

    group->addChild(text_geode);

    // set the update callback to cycle through the various min and mag filter modes.
    group->setUpdateCallback(new FilterCallback(texture,text));

    return group;

}


osg::Node* WallPaperModel::createAnisotripicWall(osg::BoundingBox& bb,const std::string& filename)
{
    osg::Group* group = new osg::Group;

    // left hand side of bounding box.
    osg::Vec3 top_left(bb.xMin(),bb.yMax(),bb.zMin());
    osg::Vec3 bottom_left(bb.xMin(),bb.yMin(),bb.zMin());
    osg::Vec3 bottom_right(bb.xMax(),bb.yMin(),bb.zMin());
    osg::Vec3 top_right(bb.xMax(),bb.yMax(),bb.zMin());
    osg::Vec3 center((bb.xMin()+bb.xMax())*0.5f,(bb.yMin()+bb.yMax())*0.5f,bb.zMin());
    float height = bb.yMax()-bb.yMin();

    // create the geometry for the wall.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = top_left;
    (*vertices)[1] = bottom_left;
    (*vertices)[2] = bottom_right;
    (*vertices)[3] = top_right;
    geom->setVertexArray(vertices);

    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    (*texcoords)[0].set(0.0f,1.0f);
    (*texcoords)[1].set(0.0f,0.0f);
    (*texcoords)[2].set(1.0f,0.0f);
    (*texcoords)[3].set(1.0f,1.0f);
    geom->setTexCoordArray(0,texcoords);

    osg::Vec3Array* normals = new osg::Vec3Array(1);
    (*normals)[0].set(0.0f,0.0f,1.0f);
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

    osg::Geode* geom_geode = new osg::Geode;
    geom_geode->addDrawable(geom);
    group->addChild(geom_geode);


    // set up the texture state.
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setImage(osgDB::readRefImageFile(filename));

    osg::StateSet* stateset = geom->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

    // create the text label.

    osgText::Text* text = new osgText::Text;
    text->setDataVariance(osg::Object::DYNAMIC);
    text->setFont("fonts/arial.ttf");
    text->setPosition(center);
    text->setCharacterSize(height*0.03f);
    text->setColor(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
    text->setAlignment(osgText::Text::CENTER_CENTER);
    text->setAxisAlignment(osgText::Text::XY_PLANE);

    osg::Geode* text_geode = new osg::Geode;
    text_geode->addDrawable(text);

    osg::StateSet* text_stateset = text_geode->getOrCreateStateSet();
    text_stateset->setAttributeAndModes(new osg::PolygonOffset(-1.0f,-1.0f),osg::StateAttribute::ON);

    group->addChild(text_geode);

    // set the update callback to cycle through the various min and mag filter modes.
    group->setUpdateCallback(new AnisotropicCallback(texture,text));

    return group;

}
