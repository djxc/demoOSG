#include <callback/filtercallback.h>

FilterCallback::FilterCallback(osg::Texture2D* texture,osgText::Text* text,double delay):
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

void FilterCallback::operator()(osg::Node*, osg::NodeVisitor* nv)
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

void FilterCallback::setValues()
{
    _texture->setFilter(osg::Texture2D::MIN_FILTER,_minFilterList[_currPos]);
    _texture->setFilter(osg::Texture2D::MAG_FILTER,_magFilterList[_currPos]);

    _text->setText(_textList[_currPos]);
}
