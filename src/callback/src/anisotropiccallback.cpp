#include <callback/anisotropiccallback.h>


AnisotropicCallback::AnisotropicCallback(osg::Texture2D* texture,osgText::Text* text,double delay):
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

void AnisotropicCallback::operator()(osg::Node*, osg::NodeVisitor* nv)
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

void AnisotropicCallback::setValues()
{
    _texture->setMaxAnisotropy(_maxAnisotropyList[_currPos]);

    _text->setText(_textList[_currPos]);
}
