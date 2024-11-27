//#include <QApplication>

#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/LineWidth>
#include <osg/DrawPixels>
#include <osg/AutoTransform>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/LOD>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/PolygonOffset>
#include <osgUtil/Optimizer>
#include <osg/StateSet>
#include <osg/DrawPixels>

#include <osgViewer/ViewerEventHandlers> //事件监听
#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制
#include <osgUtil/Simplifier> //简化几何体
#include <osgUtil/DelaunayTriangulator>


#include <callback/rotatecallback.h>
#include <callback/infocallback.h>
#include <callback/cessnacallback.h>
#include <callback/dynamiclinecallback.h>

#include <userScene/housemodel.h>


osg::Transform* createAutoTransform(double posX, osg::Node* model) {
    osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform;
    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
    at->setPosition(osg::Vec3(posX, 0.0, 0.0));
    at->addChild(model);
    return at.release();
}

osg::Transform* createMatrixTransform(double posX, double rotateZ, osg::Node* model) {
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->setMatrix(osg::Matrix::rotate(rotateZ, osg::Z_AXIS) * osg::Matrix::translate(posX, 0.0, 0.0));
    mt->addChild(model);
    return mt.release();
}

osg::Transform* createPositionAttitudeTransform(double posX, double rotateZ, osg::Node* model) {
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->setPosition(osg::Vec3(posX, 0.0, 0.0));
    pat->setAttitude(osg::Quat(rotateZ, osg::Z_AXIS));
    pat->addChild(model);
    return pat.release();
}

osg::ref_ptr<osg::Node> demoCallback(osg::Node* model) {
    // 创建一个位置旋转节点，用osg中的智能指针存储；每一帧都会调用回调函数,回调函数是用户继承NodeCallback自己实现的类。这里仅用到更新回调，另外还存在事件回调
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(model);
    pat->setUpdateCallback(new RotateCallback);
    pat->addUpdateCallback(new InfoCallback);
    return pat;
}

osg::ref_ptr<osg::Node> demoTransform(osg::Node* model) {
    // 创建一个组节点，添加3个子节点，每个子节点都是transform节点，相同的model都是transform的叶子节点，即一个model有三个示例。
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(createMatrixTransform(-5.0, osg::PI/4, model));
    root->addChild(createAutoTransform(0.0, model));
    root->addChild(createPositionAttitudeTransform(5.0, -osg::PI/4, model));
    return root;
}

osg::ref_ptr<osg::Node>  demoSwitch() {
    // 开关节点，通过设置叶子节点的true和false控制模型的显示或隐藏。这里采用更新回调
    osg::ref_ptr<osg::Switch> root1 = new osg::Switch;
    root1->addChild(osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessna.osg"), true);
    root1->addChild(osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessnafire.osg"), false);
    root1->setUpdateCallback(new CessnaCallback);
    return root1;
}

osg::ref_ptr<osg::Node> demoLOD() {
    osg::Node* model = osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-high.ive");
    float r = model->getBound().radius();
    osg::ref_ptr<osg::LOD> root = new osg::LOD;
    root->addChild(osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-low.ive"), r * 7, FLT_MAX);
    root->addChild(osgDB::readNodeFile("D:\\code\\c\\Examples\\data\\bunny-mid.ive"), r * 3, r * 7);
    root->addChild(model, 0.0, r * 3);
    return root;
}

osg::ref_ptr<osg::Node> demoChangeGeometry() {
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(10);
    for (unsigned int i = 0; i < 10; ++i) {
        (*vertices)[i].set(float(i), 0.0, 0.0);
    }
    osg::ref_ptr<osg::Geometry> lineGeom = new osg::Geometry;
    lineGeom->setVertexArray(vertices.get());
    lineGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, 10));
    lineGeom->setInitialBound(osg::BoundingBox(osg::Vec3(-10.0, -10.0, -10.0), osg::Vec3(10.0, 10.0, 10.0)));
    lineGeom->setUpdateCallback(new DynamicLineCallback);
    lineGeom->setUseDisplayList(false);
    lineGeom->setUseVertexBufferObjects(true);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0));
    geode->addDrawable(lineGeom.get());
    return geode;
}

osg::ref_ptr<osg::Node> drawHouse() {
    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    HouseModel* housemodel = new HouseModel;
    root->addDrawable(housemodel->createHouseWall());
    root->addDrawable(housemodel->createHouseRoof());
    return root;
}

osg::ref_ptr<osg::Node> drawBitmap() {
    osg::ref_ptr<osg::DrawPixels> bitmap1 = new osg::DrawPixels;
    bitmap1->setPosition( osg::Vec3(0.0, 0.0, 0.0) );
    bitmap1->setImage( osgDB::readImageFile("D:\\code\\c\\Examples\\data\\osg64.png") );

    osg::ref_ptr<osg::DrawPixels> bitmap2 = new osg::DrawPixels;
    bitmap2->setPosition( osg::Vec3(80.0, 0.0, 0.0) );
    bitmap2->setImage( osgDB::readImageFile("D:\\code\\c\\Examples\\data\\osg128.png") );

    osg::ref_ptr<osg::DrawPixels> bitmap3 = new osg::DrawPixels;
    bitmap3->setPosition( osg::Vec3(200.0, 0.0, 0.0) );
    bitmap3->setImage( osgDB::readImageFile("D:\\code\\c\\Examples\\data\\osg256.png") );
    bitmap3->setSubImageDimensions( 64, 64, 128, 128 );
    bitmap3->setUseSubImage( true );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( bitmap1.get() );
    geode->addDrawable( bitmap2.get() );
    geode->addDrawable( bitmap3.get() );
    return geode;
}

void createTexture1D( osg::StateSet& ss )
{
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->setImage( 256, 1, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
                     new unsigned char[4 * 256], osg::Image::USE_NEW_DELETE );

    unsigned char* ptr = image->data();
    for ( unsigned int i=0; i<256; ++i )
    {
        *ptr++ = i; *ptr++ = i; *ptr++ = 255;
        *ptr++ = 255;
    }

    osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D;
    texture->setImage( image.get() );
    texture->setWrap( osg::Texture1D::WRAP_S, osg::Texture1D::REPEAT );
    texture->setFilter( osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR );

    ss.setTextureAttributeAndModes( 0, texture.get() );
}

void createTexture2D( osg::StateSet& ss )
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( osgDB::readImageFile("D:\\code\\c\\Examples\\data\\clockface.jpg") );

    texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
    texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
    texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER );
    texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER );
    texture->setBorderColor( osg::Vec4(1.0, 1.0, 0.0, 1.0) );

    ss.setTextureAttributeAndModes( 0, texture.get() );
}

void createTextureCubeMap( osg::StateSet& ss )
{
    osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap;

    texture->setImage( osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\posx.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\negx.jpg") );
    texture->setImage( osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\posy.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\negy.jpg") );
    texture->setImage( osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\posz.jpg") );
    texture->setImage( osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("D:\\code\\c\\Examples\\data\\negz.jpg") );

    texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
    texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
    texture->setWrap( osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE );
    texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
    texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

    ss.setTextureAttributeAndModes( 0, texture.get() );
    ss.setTextureAttributeAndModes( 0, new osg::TexGen );
}

osg::ref_ptr<osg::Node> demoTexture() {
    osg::ref_ptr<osg::Geode> quad1 = new osg::Geode;
    quad1->addDrawable( osg::createTexturedQuadGeometry(
       osg::Vec3(-3.0,0.0,-0.5), osg::Vec3(1.0,0.0,0.0), osg::Vec3(0.0,0.0,1.0), 0.0, 0.0, 3.0, 1.0) );
    createTexture1D( *(quad1->getOrCreateStateSet()) );

    osg::ref_ptr<osg::Geode> quad2 = new osg::Geode;
    quad2->addDrawable( osg::createTexturedQuadGeometry(
       osg::Vec3(-0.5,0.0,-0.5), osg::Vec3(1.0,0.0,0.0), osg::Vec3(0.0,0.0,1.0), -0.1, -0.1, 1.1, 1.1) );
    createTexture2D( *(quad2->getOrCreateStateSet()) );

    osg::ref_ptr<osg::Geode> box = new osg::Geode;
    box->addDrawable( new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(3.0,0.0,0.0), 1.0)) );
    createTextureCubeMap( *(box->getOrCreateStateSet()) );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( quad1.get() );
    root->addChild( quad2.get() );
    root->addChild( box.get() );
    return root;
}

osg::Node* readCowModel()
{
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cow.osg");
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("D:\\code\\c\\Examples\\data\\osg128.png");
    if (image.get())
    {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
        texture->setImage(image.get());

        //设置自动生成纹理坐标
        osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
        texgen->setMode(osg::TexGen::SPHERE_MAP);

        //设置纹理环境，模式为BLEND
        osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
        texenv->setMode(osg::TexEnv::Mode::BLEND);// ADD
        texenv->setColor(osg::Vec4(0.1, 0.6, 0.1, 0.0));

        //启动单元一自动生成纹理坐标，并使用纹理
        osg::ref_ptr<osg::StateSet> state = new osg::StateSet;
        state->setTextureAttributeAndModes(1, texture.get(), osg::StateAttribute::ON);
//        state->setTextureAttributeAndModes(1, texgen.get(), osg::StateAttribute::ON);
//        state->setTextureAttribute(0, texenv.get());

        node->setStateSet(state.get());
    }
    else
    {
        std::cout << "Unable to load data file Exiting." << std::endl;
    }

    return node.release();
}

osg::ref_ptr<osg::Geode> createBox()
{

    osg::ref_ptr<osg::Geode> spGeode = new osg::Geode;// Geode是Node的派生类，为了绘制图元的管理类

    osg::ref_ptr<osg::Geometry> spGeometory = new osg::Geometry;
    spGeode->addChild(spGeometory);
    //spGeode->addDrawable(spGeometory);  // 可以将addChild替换为这句。
    osg::ref_ptr<osg::Vec3Array> spCoordsArray = new osg::Vec3Array;

    // 右侧面
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, -1.0));  // 前右下顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, -1.0));   // 后右下顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, 1.0));    // 后右上顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, 1.0));   // 前右上顶点

    // 前面
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, -1.0));  // 右下顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, 1.0));   // 右上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, 1.0));  // 左上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, -1.0)); // 左下顶点

    // 左侧面
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, -1.0));  // 前左下顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, 1.0));   // 前左上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, 1.0));    // 后左上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, -1.0));   // 后左下顶点

    // 后面
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, -1.0));    // 后下顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, 1.0));     // 后上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, 1.0));    // 左上顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, -1.0));   // 左下顶点

    // 上面
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, 1.0));     // 前右顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, 1.0));      // 后右顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, 1.0));     // 后左顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, 1.0));    // 前左顶点

    // 底面
    spCoordsArray->push_back(osg::Vec3d(1.0, -1.0, -1.0));     // 前右顶点
    spCoordsArray->push_back(osg::Vec3d(1.0, 1.0, -1.0));     // 后右顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, 1.0, -1.0));    // 后左顶点
    spCoordsArray->push_back(osg::Vec3d(-1.0, -1.0, -1.0));   // 前左顶点

    spGeometory->setVertexArray(spCoordsArray);

    osg::DrawElementsUShort* pDrawElemt{ nullptr };
    for (auto nCoordIndex = 0; nCoordIndex < spCoordsArray->size(); ++nCoordIndex)
    {
        if (0 == (nCoordIndex % 4))
        {
            pDrawElemt = new osg::DrawElementsUShort(GL_QUADS);
            pDrawElemt->push_back(nCoordIndex);
            spGeometory->addPrimitiveSet(pDrawElemt);
        }
        else
        {
            pDrawElemt->push_back(nCoordIndex);
        }
    }

    // 设置纹理
    osg::ref_ptr<osg::Texture2D>spTexture2D = new osg::Texture2D;
    osg::ref_ptr<osg::Image> spImage = osgDB::readImageFile("D:\\code\\c\\Examples\\data\\clockface.jpg");
    if (spImage.valid())
    {
        spTexture2D->setImage(spImage.get());
    }

    spTexture2D->setWrap(osg::Texture2D::WRAP_S, osg::Texture::CLAMP);
    spTexture2D->setWrap(osg::Texture2D::WRAP_T, osg::Texture::CLAMP);
    spTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture::LINEAR);
    spTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture::LINEAR);

    // 设置纹理坐标
    osg::ref_ptr<osg::Vec2Array> spTextureCoordsArray = new osg::Vec2Array;
    auto nPrimitiveSetSize = spGeometory->getPrimitiveSetList().size(); // 面的个数
    for (auto i = 0; i < nPrimitiveSetSize; i++) // 设置每个面的纹理坐标
    {
        spTextureCoordsArray->push_back(osg::Vec2(0, 0));
        spTextureCoordsArray->push_back(osg::Vec2(0, 1));
        spTextureCoordsArray->push_back(osg::Vec2(1, 1));
        spTextureCoordsArray->push_back(osg::Vec2(1, 0));
    }

    spGeometory->setTexCoordArray(0, spTextureCoordsArray, osg::Array::Binding::BIND_PER_PRIMITIVE_SET);
    spGeometory->getOrCreateStateSet()->setTextureAttributeAndModes(0, spTexture2D.get(), osg::StateAttribute::ON); // 开启纹理

    // 开启光照，要不然几何体有些面转到正对相机时是黑色的
    spGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    osg::ref_ptr<osg::Light> spLight = new osg::Light;
    spLight->setDiffuse(osg::Vec4d(0.0, 1.0, 0.5, 1.0)); // 漫反射光颜色
    spLight->setAmbient(osg::Vec4d(0.6, 0.6, 0.6, 1.0)); // 设置环境光颜色
    spLight->setPosition(osg::Vec4d(1, -1, 1, 0));       // 设置光源位置
    spGeode->getOrCreateStateSet()->setAttributeAndModes(spLight, osg::StateAttribute::ON); // 开启光照
    return spGeode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// filter wall and animation callback.
//

class FilterCallback : public osg::NodeCallback
{
public:

    FilterCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0):
        _texture(texture),
        _text(text),
        _delay(delay),
        _currPos(0),
        _prevTime(0.0)
    {
        // start with a mip mapped mode to ensure that
        _minFilterList.push_back(osg::Texture2D::LINEAR_MIPMAP_LINEAR);
        _magFilterList.push_back(osg::Texture2D::LINEAR);
        _textList.push_back("Tri-linear mip mapping (default filtering)\nsetFilter(MIN_FILTER,LINEAR_MIP_LINEAR)\nsetFilter(MAG_FILTER,LINEAR)");

        _minFilterList.push_back(osg::Texture2D::NEAREST);
        _magFilterList.push_back(osg::Texture2D::NEAREST);
        _textList.push_back("Nearest filtering\nsetFilter(MIN_FILTER,NEAREST)\nsetFilter(MAG_FILTER,NEAREST)");

        _minFilterList.push_back(osg::Texture2D::LINEAR);
        _magFilterList.push_back(osg::Texture2D::LINEAR);
        _textList.push_back("Linear filtering\nsetFilter(MIN_FILTER,LINEAR)\nsetFilter(MAG_FILTER,LINEAR)");

        _minFilterList.push_back(osg::Texture2D::NEAREST_MIPMAP_NEAREST);
        _magFilterList.push_back(osg::Texture2D::LINEAR);
        _textList.push_back("nearest mip mapping (default filtering)\nsetFilter(MIN_FILTER,)\nsetFilter(MAG_FILTER,LINEAR)");

        _minFilterList.push_back(osg::Texture2D::LINEAR_MIPMAP_NEAREST);
        _magFilterList.push_back(osg::Texture2D::LINEAR);
        _textList.push_back("bi-linear mip mapping\nsetFilter(MIN_FILTER,LINEAR_MIPMAP_NEAREST)\nsetFilter(MAG_FILTER,LINEAR)");

        _minFilterList.push_back(osg::Texture2D::NEAREST_MIPMAP_LINEAR);
        _magFilterList.push_back(osg::Texture2D::LINEAR);
        _textList.push_back("bi-linear mip mapping\nsetFilter(MIN_FILTER,NEAREST_MIPMAP_LINEAR)\nsetFilter(MAG_FILTER,LINEAR)");


        setValues();
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (nv->getFrameStamp())
        {
            double currTime = nv->getFrameStamp()->getSimulationTime();
            if (currTime-_prevTime>_delay)
            {
                // update filter modes and text.
                setValues();

                // advance the current position, wrap round if required.
                _currPos++;
                if (_currPos>=_minFilterList.size()) _currPos=0;

                // record time
                _prevTime = currTime;
            }
        }
    }

    void setValues()
    {
        _texture->setFilter(osg::Texture2D::MIN_FILTER,_minFilterList[_currPos]);
        _texture->setFilter(osg::Texture2D::MAG_FILTER,_magFilterList[_currPos]);

        _text->setText(_textList[_currPos]);
    }

protected:

    typedef std::vector<osg::Texture2D::FilterMode> FilterList;
    typedef std::vector<std::string>                TextList;

    osg::ref_ptr<osg::Texture2D>    _texture;
    osg::ref_ptr<osgText::Text>     _text;
    double                          _delay;

    FilterList                      _minFilterList;
    FilterList                      _magFilterList;
    TextList                        _textList;

    unsigned int                    _currPos;
    double                          _prevTime;

};

osg::Node* createFilterWall(osg::BoundingBox& bb,const std::string& filename)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// anisotropic wall and animation callback.
//

class AnisotropicCallback : public osg::NodeCallback
{
public:

    AnisotropicCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0):
        _texture(texture),
        _text(text),
        _delay(delay),
        _currPos(0),
        _prevTime(0.0)
    {

        _maxAnisotropyList.push_back(1.0f);
        _textList.push_back("No anisotropic filtering (default)\nsetMaxAnisotropy(1.0f)");

        _maxAnisotropyList.push_back(2.0f);
        _textList.push_back("Anisotropic filtering\nsetMaxAnisotropy(2.0f)");

        _maxAnisotropyList.push_back(4.0f);
        _textList.push_back("Anisotropic filtering\nsetMaxAnisotropy(4.0f)");

        _maxAnisotropyList.push_back(8.0f);
        _textList.push_back("Anisotropic filtering\nsetMaxAnisotropy(8.0f)");

        _maxAnisotropyList.push_back(16.0f);
        _textList.push_back("Highest quality anisotropic filtering\nsetMaxAnisotropy(16.0f)");

        setValues();
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (nv->getFrameStamp())
        {
            double currTime = nv->getFrameStamp()->getSimulationTime();
            if (currTime-_prevTime>_delay)
            {
                // update filter modes and text.
                setValues();

                // advance the current position, wrap round if required.
                _currPos++;
                if (_currPos>=_maxAnisotropyList.size()) _currPos=0;

                // record time
                _prevTime = currTime;
            }
        }
    }

    void setValues()
    {
        _texture->setMaxAnisotropy(_maxAnisotropyList[_currPos]);

        _text->setText(_textList[_currPos]);
    }

protected:

    typedef std::vector<float>          AnisotropyList;
    typedef std::vector<std::string>    TextList;

    osg::ref_ptr<osg::Texture2D>    _texture;
    osg::ref_ptr<osgText::Text>     _text;
    double                          _delay;

    AnisotropyList                  _maxAnisotropyList;
    TextList                        _textList;

    unsigned int                    _currPos;
    double                          _prevTime;

};

osg::Node* createAnisotripicWall(osg::BoundingBox& bb,const std::string& filename)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// wrap wall and animation callback.
//
class WrapCallback : public osg::NodeCallback
{
public:

    WrapCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0):
        _texture(texture),
        _text(text),
        _delay(delay),
        _currPos(0),
        _prevTime(0.0)
    {

        _wrapList.push_back(osg::Texture2D::CLAMP);
        _textList.push_back("Default tex coord clamp\nsetWrap(WRAP_S,CLAMP)");

        _wrapList.push_back(osg::Texture2D::CLAMP_TO_EDGE);
        _textList.push_back("Clamp to edge extension\nsetWrap(WRAP_S,CLAMP_TO_EDGE)");

        _wrapList.push_back(osg::Texture2D::CLAMP_TO_BORDER);
        _textList.push_back("Clamp to border color extension\nsetWrap(WRAP_S,CLAMP_TO_BORDER)");

        _wrapList.push_back(osg::Texture2D::REPEAT);
        _textList.push_back("Repeat wrap\nsetWrap(WRAP_S,REPEAT)");

        _wrapList.push_back(osg::Texture2D::MIRROR);
        _textList.push_back("Mirror wrap extension\nsetWrap(WRAP_S,MIRROR)");

        setValues();
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (nv->getFrameStamp())
        {
            double currTime = nv->getFrameStamp()->getSimulationTime();
            if (currTime-_prevTime>_delay)
            {
                // update filter modes and text.
                setValues();

                // advance the current position, wrap round if required.
                _currPos++;
                if (_currPos>=_wrapList.size()) _currPos=0;

                // record time
                _prevTime = currTime;
            }
        }
    }

    void setValues()
    {
        _texture->setWrap(osg::Texture2D::WRAP_S,_wrapList[_currPos]);
        _texture->setWrap(osg::Texture2D::WRAP_T,_wrapList[_currPos]);

        _text->setText(_textList[_currPos]);
    }

protected:

    typedef std::vector<osg::Texture2D::WrapMode> WrapList;
    typedef std::vector<std::string>              TextList;

    osg::ref_ptr<osg::Texture2D>    _texture;
    osg::ref_ptr<osgText::Text>     _text;
    double                          _delay;

    WrapList                        _wrapList;
    TextList                        _textList;

    unsigned int                    _currPos;
    double                          _prevTime;

};

osg::Node* createWrapWall(osg::BoundingBox& bb,const std::string& filename)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// sublooad wall and animation callback.
//

class ImageUpdateCallback : public osg::NodeCallback
{
public:

    ImageUpdateCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0):
        _texture(texture),
        _text(text),
        _delay(delay),
        _currPos(0),
        _prevTime(0.0)
    {

#if 1
        osg::ref_ptr<osg::Image> originalImage = osgDB::readRefImageFile("D:\\code\\c\\OpenSceneGraph-Data\\Images\\dog_left_eye.jpg");

        osg::ref_ptr<osg::Image> subImage = new osg::Image;
        subImage->setUserData(originalImage.get()); // attach the originalImage as user data to prevent it being deleted.

        // now assign the appropriate portion data from the originalImage
        subImage->setImage(originalImage->s()/2, originalImage->t()/2, originalImage->r(), // half the width and height
                           originalImage->getInternalTextureFormat(), // same internal texture format
                           originalImage->getPixelFormat(),originalImage->getDataType(), // same pixel format and data type
                           originalImage->data(originalImage->s()/4,originalImage->t()/4), // offset the start point to 1/4 into the image
                           osg::Image::NO_DELETE, // don't attempt to delete the image data, leave this to the originalImage
                           originalImage->getPacking(), // use the same packing
                           originalImage->s()); // use the width of the original image as the row width


        subImage->setPixelBufferObject(new osg::PixelBufferObject(subImage.get()));

#if 0
        OSG_NOTICE<<"orignalImage iterator"<<std::endl;
        for(osg::Image::DataIterator itr(originalImage.get()); itr.valid(); ++itr)
        {
            OSG_NOTICE<<"  "<<(void*)itr.data()<<", "<<itr.size()<<std::endl;
        }

        OSG_NOTICE<<"subImage iterator, size "<<subImage->s()<<", "<<subImage->t()<<std::endl;
        unsigned int i=0;
        for(osg::Image::DataIterator itr(subImage.get()); itr.valid(); ++itr, ++i)
        {
            OSG_NOTICE<<"  "<<i<<", "<<(void*)itr.data()<<", "<<itr.size()<<std::endl;

            for(unsigned char* d=const_cast<unsigned char*>(itr.data()); d<(itr.data()+itr.size()); ++d)
            {
                *d = 255-*d;
            }
        }
#endif


        _imageList.push_back(subImage);

#else
        _imageList.push_back(osgDB::readRefImageFile("Images/dog_left_eye.jpg"));
#endif
        _textList.push_back("Subloaded Image 1 - dog_left_eye.jpg");

        _imageList.push_back(osgDB::readRefImageFile("D:\\code\\c\\OpenSceneGraph-Data\\Images\\dog_right_eye.jpg"));
        _textList.push_back("Subloaded Image 2 - dog_right_eye.jpg");

        setValues();
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (nv->getFrameStamp())
        {
            double currTime = nv->getFrameStamp()->getSimulationTime();
            if (currTime-_prevTime>_delay)
            {
                // update filter modes and text.
                setValues();

                // advance the current position, wrap round if required.
                _currPos++;
                if (_currPos>=_imageList.size()) _currPos=0;

                // record time
                _prevTime = currTime;
            }
        }
    }

    void setValues()
    {
        // Note, as long as the images are the same dimensions subloading will be used
        // to update the textures.  If dimensions change then the texture objects have
        // to be deleted and re-recreated.
        //
        // The load/subload happens during the draw traversal so doesn't happen on
        // the setImage which just updates internal pointers and modified flags.

        _texture->setImage(_imageList[_currPos].get());

        //_texture->dirtyTextureObject();

        _text->setText(_textList[_currPos]);
    }

protected:


    typedef std::vector< osg::ref_ptr<osg::Image> > ImageList;
    typedef std::vector<std::string>                TextList;

    osg::ref_ptr<osg::Texture2D>    _texture;
    osg::ref_ptr<osgText::Text>     _text;
    double                          _delay;

    ImageList                       _imageList;
    TextList                        _textList;

    unsigned int                    _currPos;
    double                          _prevTime;

};

osg::Node* createSubloadWall(osg::BoundingBox& bb)
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


osg::Node* createModel()
{

    // create the root node which will hold the model.
    osg::Group* root = new osg::Group();

    // turn off lighting
    root->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    osg::BoundingBox bb(0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);

    root->addChild(createFilterWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\lz.rgb"));
    root->addChild(createAnisotripicWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\primitives.gif"));
    root->addChild(createWrapWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\tree0.rgba"));
    root->addChild(createSubloadWall(bb));

    return root;
}

int main(int argc, char *argv[])
{
    osg::ArgumentParser arguments(&argc, argv);
    osg::Node* model = osgDB::readNodeFiles(arguments);
    if (!model) {
        model = osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cow.osg");
    }

//    osg::ref_ptr<osg::Node> root = demoTexture(); // drawBitmap(); //demoChangeGeometry(); //demoLOD(); // demoCallback(model); // demoTransform(model); // demoSwitch();

//    osgViewer::Viewer viewer;

//    osg::ref_ptr<osg::MatrixTransform> spMatrixTransform = new osg::MatrixTransform;
//    // 绕y、z轴转动下，这样便于观察效果
//    spMatrixTransform->setMatrix(osg::Matrix::rotate(osg::PI / 3.0, osg::Vec3(0, 0, 1)) * osg::Matrix::rotate(osg::PI / 5.0, osg::Vec3(1, 0, 0)));
//    spMatrixTransform->addChild(createBox());

//    viewer.setSceneData(spMatrixTransform);

////    viewer.setSceneData( createModel() );
//    viewer.setUpViewInWindow(100, 100, 800, 600);
//    return viewer.run();

    //添加到根节点
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(readCowModel());

    //优化场景数据
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get());

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    viewer->setUpViewInWindow(100, 100, 800, 600);
    //方便查看在多边形之间切换，以查看三角网
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->addEventHandler(new osgViewer::StatsHandler());
    viewer->addEventHandler(new osgViewer::WindowSizeHandler());
    viewer->setSceneData(root.get());
    viewer->realize();

    return viewer->run();
}
