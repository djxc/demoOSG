# 学习OSG
- 1、首先获取OSG的动态库，可以自己从源码进行编译，也可以直接下载编译好的库如从OSG网站下载`http://www.osgchina.org/downloads-son.php?id=34`，自己编译的存在问题，加载模型没有纹理，且报警告
- 2、创建cmake项目，将动态库和头文件添加到项目中。
- 3、编写OSG程序
- 4、运行，运行需要将OSG的动态库放到可执行文件相同目录下
- 5、用其他人编译好的一直存在问题，图片纹理无效果，用vckpt安装也存在问题。最后通过cmake + vs2017从源码进行编译，程序正常。需要首先下载整理好的第三方库。
- 6、按键盘导致程序卡死问题，通过设置windows输入法，常规中的兼容性选中，则可以解决该问题。输入法的中英文确实也影响键盘的监听事件。
- 7、Qt继承osg，首先需要安装osgqt库，生成osgOpenGLWidget的lib以及头文件，放在include以及lib文件夹中，由于需要qt库，需要在cmakeList中添加include_directories(qtPath).需要创建一个视图类，然后将osgWidget添加到视图类中即可。


