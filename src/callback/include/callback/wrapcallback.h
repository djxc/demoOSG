#ifndef WRAPCALLBACK_H
#define WRAPCALLBACK_H

#include <osg/NodeCallback>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osgText/Text>

class WrapCallback: public osg::NodeCallback
{
public:
    WrapCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0);
    virtual void operator()(osg::Node*, osg::NodeVisitor* nv);
    void setValues();

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

#endif // WRAPCALLBACK_H
