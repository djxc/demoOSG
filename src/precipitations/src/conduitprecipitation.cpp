#include <precipitations/conduitprecipitation.h>

ConduitPrecipitation::ConduitPrecipitation(): m_fTheNum(800), m_fTheSize(0.2), m_fTheSpeech(100)
{
    //默认位置
    m_vecPosition.set(15, 64, 3);
}

osg::Node* ConduitPrecipitation::CreateConduit(osg::Group* root) {
    osgParticle::ParticleSystem* ps2 = createConduitPrecipitation(root);
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps2);
    return psu;
}

osgParticle::ParticleSystem* ConduitPrecipitation::createConduitPrecipitation(osg::Group* root) {
    osgParticle::Particle ptemplate;
    //生周期为2，对于喷泉已经够了
    ptemplate.setLifeTime(2);
    //设置图形变化范围
    ptemplate.setSizeRange(osgParticle::rangef(0.1f, 0.1f));
    //设置透明度变化范围
    ptemplate.setAlphaRange(osgParticle::rangef(1.0f, 0.5f));
    //设置颜色范围
    ptemplate.setColorRange(osgParticle::rangev4(
        osg::Vec4(0.7f, 1.0f, 1.0f, 1.5f),
        osg::Vec4(0.8f, 0.8f, 1.0f, 0.0f)));
    //设置半径
    ptemplate.setRadius(m_fTheSize);
    // 设置重量
    ptemplate.setMass(0.05f);
    osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
    ps->setDefaultAttributes("D:\\code\\c\\OpenSceneGraph-Data\\Images\\smoke.rgb", false, false);//纹理
    //加入模版
    ps->setDefaultParticleTemplate(ptemplate);
    //建立发射器,中包含发射枪，数目及位置设定
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
    //加入模版及总属性
    emitter->setParticleSystem(ps);
    //数目变化
    osgParticle::RandomRateCounter *counter = new osgParticle::RandomRateCounter;
    //数目变化，当前场景中的粒子数目
    counter->setRateRange(m_fTheNum, m_fTheNum);
    //加入到发射器中
    emitter->setCounter(counter);
    //设置位置
    osgParticle::PointPlacer *placer = new osgParticle::PointPlacer;
    placer->setCenter(m_vecPosition);
    //加入到发射器中
    emitter->setPlacer(placer);
    //设置发射枪，可以设置初速度等
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    //设置初速度
    shooter->setInitialSpeedRange(m_fTheSpeech, 0);
    emitter->setShooter(shooter);
    root->addChild(emitter);
    //设置影响操作，
    osgParticle::ModularProgram *program = new osgParticle::ModularProgram;
    program->setParticleSystem(ps);
    //速度操作
    osgParticle::AccelOperator *op1 = new osgParticle::AccelOperator;
    op1->setToGravity();
    program->addOperator(op1);
    osgParticle::FluidFrictionOperator *op3 = new osgParticle::FluidFrictionOperator;
    op3->setFluidToAir();
    program->addOperator(op3);
    root->addChild(program);
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
    root->addChild(geode);
    return ps ;
}


ConduitPrecipitation::~ConduitPrecipitation(){}
