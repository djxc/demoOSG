#include <handlers/keyboardhandler.h>

KeyboardHandler::KeyboardHandler()
{

}

bool KeyboardHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa,
                    osg::Object*, osg::NodeVisitor* )
{
   osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>( &aa );
   if ( !viewer ) return false;

   switch ( ea.getEventType() )
   {
   // 鼠标按下事件
   case osgGA::GUIEventAdapter::PUSH:
       break;

   case osgGA::GUIEventAdapter::KEYDOWN:
       if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Space )
       {
           int width = ea.getWindowWidth();
           int height = ea.getWindowHeight();
           viewer->requestWarpPointer( width*0.5, height*0.5 );
       }
       else
       {
           osg::Switch* root = dynamic_cast<osg::Switch*>( viewer->getSceneData() );
           if ( !root ) return false;

           if ( ea.getKey()=='1' )
           {
               root->setValue( 0, true );
               root->setValue( 1, false );
           }
           else if ( ea.getKey()=='2' )
           {
               root->setValue( 0, false );
               root->setValue( 1, true );
           }
           return true;
       }
       break;
   default:
       break;
   }
   return false;
}
