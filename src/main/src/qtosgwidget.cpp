#include <main/qtosgwidget.h>


QtOsgWidget::QtOsgWidget(QWidget *parent)
    : osgQOpenGLWidget(parent),
    _mViewer(new osgViewer::Viewer),
    _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(this->x(),this->y(),this->width(),this->height()))
{
    setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(100, 100);
}

/**
����OpenGL��Դ��״̬
*/
void QtOsgWidget::initializeGL() {
    osg::Camera* camera = _mViewer->getCamera();
    camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    camera->setProjectionMatrixAsPerspective(30, (double)this->width() / this->height(), 1, 1000);
    camera->setViewport(0, 0, this->width(), this->height());
    camera->setGraphicsContext(_mGraphicsWindow.get());

    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator();

    manipulator->setAllowThrow(false);
    _mViewer->setCameraManipulator(manipulator.get());
    _mViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    osg::ref_ptr<osg::Group> root = new osg::Group();

    QByteArray barr = QString("root").toLocal8Bit();
    char* bdata = barr.data();
    root->setName(bdata);

    osg::ref_ptr<osg::Node> node = osgDB::readRefNodeFile("D:\\code\\c\\OpenSceneGraph-Data\\glider.osg");
    root->addChild(node.get());

    //�Ż���������
    /*osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get());*/

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
    ////    root =
    ////            demoKeyEvent();
    ////            drawHouse();
    ////            createWallPaperModel();
    ////            demoTexture(); // drawBitmap(); //demoChangeGeometry(); //demoLOD(); // demoCallback(model); // demoTransform(model); // demoSwitch();


    ////    osg::ref_ptr<osg::MatrixTransform> spMatrixTransform = new osg::MatrixTransform;
    ////    // 绕y、z轴转动下，这样便于观察效果
    ////    spMatrixTransform->setMatrix(osg::Matrix::rotate(osg::PI / 3.0, osg::Vec3(0, 0, 1)) * osg::Matrix::rotate(osg::PI / 5.0, osg::Vec3(1, 0, 0)));
    ////    spMatrixTransform->addChild(createBox());

    ////    viewer.setSceneData(spMatrixTransform);

    //    viewer->setUpViewInWindow(100, 100, 800, 600);
    //    viewer->setSceneData(root.get());
    //    // 添加常用状态事件回调，w会切换显示网格，再点击一下显示节点；s显示帧率，再点击会显示详情；l控制灯光；f会切换全屏
    //    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    //    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    //    viewer->addEventHandler(new osgViewer::StatsHandler);
    //    {
    //        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keysSwitchManipulator = new osgGA::KeySwitchMatrixManipulator;
    //        keysSwitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator);
    //        keysSwitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator);
    //        keysSwitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator);
    //        keysSwitchManipulator->addMatrixManipulator('4', "Terrain", new osgGA::TerrainManipulator);
    //        viewer->setCameraManipulator(keysSwitchManipulator);
    //    }
    //    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

    //    viewer->addEventHandler( new KeyboardHandler );

    _mViewer->setSceneData(root);
    _mViewer->realize();
}

/**
�ڳ�������ʱ��ȾOpenGL
*/
void QtOsgWidget::paintGL() {
    _mViewer->frame();
}
/**
����OpenGL�ӿڣ�ͶӰ��
*/
void QtOsgWidget::resizeGL(int w, int h) {
    this->getEventQueue()->windowResize(this->x(), this->y(), w, h);
    _mGraphicsWindow->resized(this->x(), this->y(), w, h);
    osg::Camera* camera = _mViewer->getCamera();
    camera->setViewport(this->x(), this->y(), w, h);
}
/**
��ȡOSG�¼�����
*/
osgGA::EventQueue* QtOsgWidget::getEventQueue()const {
    return _mGraphicsWindow->getEventQueue();
};

void QtOsgWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    setKeyboardModifiers(event);
    int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    }
    getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
    QOpenGLWidget::mouseDoubleClickEvent(event);
}

void QtOsgWidget::mouseMoveEvent(QMouseEvent *event) {
    setKeyboardModifiers(event);
    getEventQueue()->mouseMotion(event->x(), event->y());
    QOpenGLWidget::mouseMoveEvent(event);
}

void QtOsgWidget::mousePressEvent(QMouseEvent *event) {
    setKeyboardModifiers(event);
    int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    }
    getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
    QOpenGLWidget::mousePressEvent(event);
}

void QtOsgWidget::mouseReleaseEvent(QMouseEvent *event) {
    setKeyboardModifiers(event);
    int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MidButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    }
    getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
    QOpenGLWidget::mouseReleaseEvent(event);
}

void QtOsgWidget::wheelEvent(QWheelEvent *event) {
    setKeyboardModifiers(event);
    getEventQueue()->mouseScroll(event->orientation() == Qt::Vertical ?
        (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
        (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
    QOpenGLWidget::wheelEvent(event);
}

bool QtOsgWidget::event(QEvent *event) {
    bool handled = QOpenGLWidget::event(event);
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
    case QEvent::MouseButtonDblClick:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        this->update();
        break;
    default:
        break;
    }
    return handled;
}

void QtOsgWidget::setKeyboardModifiers(QInputEvent* event) {
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier);
    unsigned mask = 0;
    if (modkey & Qt::ShiftModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }
    if (modkey & Qt::AltModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }
    if (modkey & Qt::ControlModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }
    getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}
void QtOsgWidget::keyPressEvent(QKeyEvent *event) {
    setKeyboardModifiers(event);
    //QByteArray KeyData = event->text().toLocal8Bit();
    /*const char* cKey = KeyData.data();
    osgGA::GUIEventAdapter::KeySymbol symbol = osgGA::GUIEventAdapter::KeySymbol(event->key());*/
    //getEventQueue()->keyPress();
    switch (event->key())
    {
    case Qt::Key_F:
    {
        //�ӹ۲��߻�ȡ�ڵ�ķ�ʽ
        osg::Geode* geode = dynamic_cast<osg::Geode*>(_mViewer->getSceneData());
        int count = geode->getNumDrawables();
        for (int i = 0; i < count; ++i) {
            osg::ShapeDrawable* draw = dynamic_cast<osg::ShapeDrawable*> (geode->getDrawable(i));
            std::string name = draw->getName();
            if (name == "zidingyi1") {
                draw->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));
            }
        }
    }
        break;
    case Qt::Key_D:
    {
        //�ӹ۲��߻�ȡ�ڵ�ķ�ʽ
        osg::Geode* geode = dynamic_cast<osg::Geode*>(_mViewer->getSceneData());
        int count = geode->getNumDrawables();
        for (int i = 0; i < count; ++i) {
            osg::Drawable* draw = geode->getDrawable(i);
            std::string name = draw->getName();
            if (name == "zidingyi1") {
                geode->removeDrawable(draw);
            }
        }
    }
        break;
    default:
        break;
    }
    QOpenGLWidget::keyPressEvent(event);
}

void QtOsgWidget::keyReleaseEvent(QKeyEvent *event) {
    setKeyboardModifiers(event);

    QOpenGLWidget::keyReleaseEvent(event);
}
