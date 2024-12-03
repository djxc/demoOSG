#ifndef CONDUITPRECIPITATION_H
#define CONDUITPRECIPITATION_H

#include <osg/Group>
#include <osgParticle/Particle>
#include <osgParticle/PointPlacer>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

class ConduitPrecipitation
{
public:
    ConduitPrecipitation();
    ~ConduitPrecipitation();

    osgParticle::ParticleSystem* createConduitPrecipitation(osg::Group* group);
    float m_fTheNum;
    float m_fTheSize;
    float m_fTheSpeech;
    osg::Vec3 m_vecPosition;

    osg::Node* CreateConduit(osg::Group * root);
};

#endif // CONDUITPRECIPITATION_H
