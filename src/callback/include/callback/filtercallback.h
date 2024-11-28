#ifndef FILTERCALLBACK_H
#define FILTERCALLBACK_H

#include <osg/NodeCallback>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osgText/Text>

class FilterCallback: public osg::NodeCallback
{
public:
    FilterCallback(osg::Texture2D* texture,osgText::Text* text,double delay=1.0);
    virtual void operator()(osg::Node*, osg::NodeVisitor* nv);
    void setValues();

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

#endif // FILTERCALLBACK_H
