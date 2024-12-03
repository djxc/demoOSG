//#include <QApplication>

#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/LOD>
#include <osg/Geode>
#include <osg/TexGen>
#include <osg/StateSet>
#include <osg/LineWidth>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/DrawPixels>
#include <osg/DrawPixels>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osg/PolygonOffset>
#include <osg/TextureCubeMap>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgParticle/PrecipitationEffect>

#include <osgViewer/ViewerEventHandlers> //事件监听

#include <osgGA/DriveManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>

#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier> //简化几何体
#include <osgUtil/DelaunayTriangulator>


#include <callback/rotatecallback.h>
#include <callback/infocallback.h>
#include <callback/cessnacallback.h>
#include <callback/dynamiclinecallback.h>

#include <userScene/housemodel.h>
#include <userScene/wallpapermodel.h>

#include <handlers/keyboardhandler.h>

#include <precipitations/conduitprecipitation.h>


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


osg::ref_ptr<osg::Node> demoTransform(osg::Node* model) {
    // 创建一个组节点，添加3个子节点，每个子节点都是transform节点，相同的model都是transform的叶子节点，即一个model有三个示例。
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(createMatrixTransform(-5.0, osg::PI/4, model));
    root->addChild(createAutoTransform(0.0, model));
    root->addChild(createPositionAttitudeTransform(5.0, -osg::PI/4, model));
    return root;
}

osg::ref_ptr<osg::Node> demoCallback(osg::Node* model) {
    // 创建一个位置旋转节点，用osg中的智能指针存储；每一帧都会调用回调函数,回调函数是用户继承NodeCallback自己实现的类。这里仅用到更新回调，另外还存在事件回调
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(model);
    pat->setUpdateCallback(new RotateCallback);
    pat->addUpdateCallback(new InfoCallback);
    return pat;
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

osg::ref_ptr<osg::Node> createWallPaperModel()
{

    // create the root node which will hold the model.
    osg::ref_ptr<osg::Group> root = new osg::Group();

    // turn off lighting
    root->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    osg::BoundingBox bb(0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);
    WallPaperModel* wallPaperModel = new WallPaperModel;
    root->addChild(wallPaperModel->createFilterWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\lz.rgb"));
    root->addChild(wallPaperModel->createAnisotripicWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\primitives.gif"));
    root->addChild(wallPaperModel->createWrapWall(bb,"D:\\code\\c\\OpenSceneGraph-Data\\Images\\tree0.rgba"));
    root->addChild(wallPaperModel->createSubloadWall(bb));

    return root;
}

osg::ref_ptr<osg::Node> demoKeyEvent() {
    osg::ref_ptr<osg::Switch> root = new osg::Switch;
    root->addChild( osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessna.osg"), true );
    root->addChild( osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\cessnafire.osg"), false );
    return root;
}

int main(int argc, char *argv[])
{
    osg::ArgumentParser arguments(&argc, argv);
    osg::ref_ptr<osg::Group> root = new osg::Group();
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFiles(arguments);
    if (!model) {
        model = osgDB::readNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\glider.osg");
    }
    root->addChild(model.get());

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

    // 雪花例子特效
    osg::ref_ptr<osgParticle::PrecipitationEffect> precipitationEffect = new osgParticle::PrecipitationEffect;
    precipitationEffect->snow(0.5);
    precipitationEffect->setParticleColor(osg::Vec4(1, 1, 1, 1));
    precipitationEffect->setWind(osg::Vec3(2, 0, 0));

    root->addChild(precipitationEffect.get());

    //申请喷泉对象
    ConduitPrecipitation od;
    //设置位置
    od.m_vecPosition.set(0, 0, 0) ;
    //加入到场景当中，就开始喷了
    root ->addChild(od.CreateConduit(root)) ;
//    root =
//            demoKeyEvent();
//            drawHouse();
//            createWallPaperModel();
//            demoTexture(); // drawBitmap(); //demoChangeGeometry(); //demoLOD(); // demoCallback(model); // demoTransform(model); // demoSwitch();


//    osg::ref_ptr<osg::MatrixTransform> spMatrixTransform = new osg::MatrixTransform;
//    // 绕y、z轴转动下，这样便于观察效果
//    spMatrixTransform->setMatrix(osg::Matrix::rotate(osg::PI / 3.0, osg::Vec3(0, 0, 1)) * osg::Matrix::rotate(osg::PI / 5.0, osg::Vec3(1, 0, 0)));
//    spMatrixTransform->addChild(createBox());

//    viewer.setSceneData(spMatrixTransform);

    viewer->setUpViewInWindow(100, 100, 800, 600);
    viewer->setSceneData(root.get());
    // 添加常用状态事件回调，w会切换显示网格，再点击一下显示节点；s显示帧率，再点击会显示详情；l控制灯光；f会切换全屏
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    viewer->addEventHandler(new osgViewer::StatsHandler);
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keysSwitchManipulator = new osgGA::KeySwitchMatrixManipulator;
        keysSwitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator);
        keysSwitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator);
        keysSwitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator);
        keysSwitchManipulator->addMatrixManipulator('4', "Terrain", new osgGA::TerrainManipulator);
        viewer->setCameraManipulator(keysSwitchManipulator);
    }
    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

    viewer->addEventHandler( new KeyboardHandler );
    return viewer->run();
}
