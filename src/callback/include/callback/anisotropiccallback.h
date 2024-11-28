#ifndef ANISOTROPICCALLBACK_H
#define ANISOTROPICCALLBACK_H

#include <osg/NodeCallback>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osgText/Text>

class AnisotropicCallback: public osg::NodeCallback
{
public:
    AnisotropicCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0);
    virtual void operator()(osg::Node*, osg::NodeVisitor* nv);
    void setValues();

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

#endif // ANISOTROPICCALLBACK_H
