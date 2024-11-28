#ifndef IMAGEUPDATECALLBACK_H
#define IMAGEUPDATECALLBACK_H

#include <osg/NodeCallback>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osgText/Text>

class ImageUpdateCallback: public osg::NodeCallback
{
public:
    ImageUpdateCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0);

    void setValues();
    virtual void operator()(osg::Node*, osg::NodeVisitor* nv);

protected:


    typedef std::vector<osg::ref_ptr<osg::Image> > ImageList;
    typedef std::vector<std::string>                TextList;

    osg::ref_ptr<osg::Texture2D>    _texture;
    osg::ref_ptr<osgText::Text>     _text;
    double                          _delay;

    ImageList                       _imageList;
    TextList                        _textList;

    unsigned int                    _currPos;
    double                          _prevTime;
};

#endif // IMAGEUPDATECALLBACK_H
