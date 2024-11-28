#include<callback/wrapcallback.h>


WrapCallback::WrapCallback(osg::Texture2D* texture,osgText::Text* text,double delay):
    _texture(texture),
    _text(text),
    _delay(delay),
    _currPos(0),
    _prevTime(0.0)
{

    _wrapList.push_back(osg::Texture2D::CLAMP);
    _textList.push_back("Default tex coord clamp\nsetWrap(WRAP_S,CLAMP)");

    _wrapList.push_back(osg::Texture2D::CLAMP_TO_EDGE);
    _textList.push_back("Clamp to edge extension\nsetWrap(WRAP_S,CLAMP_TO_EDGE)");

    _wrapList.push_back(osg::Texture2D::CLAMP_TO_BORDER);
    _textList.push_back("Clamp to border color extension\nsetWrap(WRAP_S,CLAMP_TO_BORDER)");

    _wrapList.push_back(osg::Texture2D::REPEAT);
    _textList.push_back("Repeat wrap\nsetWrap(WRAP_S,REPEAT)");

    _wrapList.push_back(osg::Texture2D::MIRROR);
    _textList.push_back("Mirror wrap extension\nsetWrap(WRAP_S,MIRROR)");

    setValues();
}

void WrapCallback::operator()(osg::Node*, osg::NodeVisitor* nv)
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
            if (_currPos>=_wrapList.size()) _currPos=0;

            // record time
            _prevTime = currTime;
        }
    }
}

void WrapCallback::setValues()
{
    _texture->setWrap(osg::Texture2D::WRAP_S,_wrapList[_currPos]);
    _texture->setWrap(osg::Texture2D::WRAP_T,_wrapList[_currPos]);

    _text->setText(_textList[_currPos]);
}
