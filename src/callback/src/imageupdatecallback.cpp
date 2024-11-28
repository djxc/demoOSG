#include <callback/imageupdatecallback.h>


ImageUpdateCallback::ImageUpdateCallback(osg::Texture2D* texture, osgText::Text* text, double delay):
    _texture(texture),
    _text(text),
    _delay(delay),
    _currPos(0),
    _prevTime(0.0)
{

#if 1
        osg::ref_ptr<osg::Image> originalImage = osgDB::readRefImageFile("D:\\code\\c\\OpenSceneGraph-Data\\Images\\dog_left_eye.jpg");

        osg::ref_ptr<osg::Image> subImage = new osg::Image;
        subImage->setUserData(originalImage.get()); // attach the originalImage as user data to prevent it being deleted.

        // now assign the appropriate portion data from the originalImage
        subImage->setImage(originalImage->s()/2, originalImage->t()/2, originalImage->r(), // half the width and height
                           originalImage->getInternalTextureFormat(), // same internal texture format
                           originalImage->getPixelFormat(),originalImage->getDataType(), // same pixel format and data type
                           originalImage->data(originalImage->s()/4,originalImage->t()/4), // offset the start point to 1/4 into the image
                           osg::Image::NO_DELETE, // don't attempt to delete the image data, leave this to the originalImage
                           originalImage->getPacking(), // use the same packing
                           originalImage->s()); // use the width of the original image as the row width


        subImage->setPixelBufferObject(new osg::PixelBufferObject(subImage.get()));

#if 0
        OSG_NOTICE<<"orignalImage iterator"<<std::endl;
        for(osg::Image::DataIterator itr(originalImage.get()); itr.valid(); ++itr)
        {
            OSG_NOTICE<<"  "<<(void*)itr.data()<<", "<<itr.size()<<std::endl;
        }

        OSG_NOTICE<<"subImage iterator, size "<<subImage->s()<<", "<<subImage->t()<<std::endl;
        unsigned int i=0;
        for(osg::Image::DataIterator itr(subImage.get()); itr.valid(); ++itr, ++i)
        {
            OSG_NOTICE<<"  "<<i<<", "<<(void*)itr.data()<<", "<<itr.size()<<std::endl;

            for(unsigned char* d=const_cast<unsigned char*>(itr.data()); d<(itr.data()+itr.size()); ++d)
            {
                *d = 255-*d;
            }
        }
#endif


        _imageList.push_back(subImage);

#else
        _imageList.push_back(osgDB::readRefImageFile("Images/dog_left_eye.jpg"));
#endif
        _textList.push_back("Subloaded Image 1 - dog_left_eye.jpg");

        _imageList.push_back(osgDB::readRefImageFile("D:\\code\\c\\OpenSceneGraph-Data\\Images\\dog_right_eye.jpg"));
        _textList.push_back("Subloaded Image 2 - dog_right_eye.jpg");

        setValues();
    }

void ImageUpdateCallback::operator()(osg::Node*, osg::NodeVisitor* nv)
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
            if (_currPos>=_imageList.size()) _currPos=0;

            // record time
            _prevTime = currTime;
        }
    }
}

void ImageUpdateCallback::setValues()
{
    // Note, as long as the images are the same dimensions subloading will be used
    // to update the textures.  If dimensions change then the texture objects have
    // to be deleted and re-recreated.
    //
    // The load/subload happens during the draw traversal so doesn't happen on
    // the setImage which just updates internal pointers and modified flags.

    _texture->setImage(_imageList[_currPos].get());

    //_texture->dirtyTextureObject();

    _text->setText(_textList[_currPos]);
}
