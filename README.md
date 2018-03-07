# Licence Plates Recognition

- This project based on Qt and opencv.  
- Translated from the Matlab vision: https://github.com/WangHongshuo/Licence-Plates-Recognition-Matlab  
## Introduction： ##
                    
I can upload files again after nearly 11 months due to the unstable network.              
This program started in 11/10/2016 and finished in 02/14/2017. It was my first c++ program, I'm a beginer, but I learned a lot in coding this project. It's better than only read the book. I translated it from Matlab script to C++ with Opencv 3.2, it could work in my test but still have some bugs that can lead to terrible crash. But I don't want to fix it by now because it was my first program with UI and it reads badly. I think if I want to recode it, I will write a class and use struct.           
Ahhhhhhhh, my English is also bad.         

在被墙了11个月后我又可以上github了。         
这个程序从2016.11.10开始的，在2017.02.14结束的，是我第一个具备完整功能的程序，也是我0基础写出来的，在写的过程中真是学到了很多东西。用了Qt库写界面，opencv库处理图像。直接从Matlab脚本语言硬翻到C艹的，全是面向过程语言，但是可以工作，虽然有些时候会导致错误。因为是第一次写，所以写的很粗糙，我现在也改不动了，如果要改的话，那就重写好了。再重写我一定要写成类的形式，那个时候还没有类的概念。           
## Screenshots： ##
![img](https://github.com/WangHongshuo/Readme_Images_Repository/blob/master/Licence_Plates_Recognition-Qt-opencv/Demo.gif) 

## Change Log: ##

- 2018.03.07:

Finished rebase, squrshed some commits and deleted some useless files in some commits.

完成变基，合并重复的提交和删除部分提交中不需要的文件。

- 2018.01.09:     
 
Set `ImageWidget` and `mat_qimage_convert` to submodule.     

将 `ImageWidget` 和 `mat_qimage_convert` 设置为子模块。     

- 2017.12.23

There will be a list in the pop-up window when you choose multiple pictures. But this function is just like a shit because I have to use QStringList to store files path and may lead to memory leaks. I don't like QStringList after reading Qt source code and documentation. I find that QStringList is something like QList<QString> (similars to vector<>), the memory QStringList occupied will not be released when execute clear function. It makes me feel that there are memory leaks existed in my program.              
PS: I did something about UI in the past few days, I believe that I should focus in programing same little machine vision projects with the help of Opencv, not the UI.       

在选中多个图片时，会弹出图片列表。但是该功能完成的很屎，可能存在内存泄漏。在看了Qt的相关源码和文档后，不喜欢QStringList，该类大概是QList<QString>，由Qt托管内存，类似于vector<>，执行clear后不会立即释放内存，没有找到合适的方法来手动释放，一直以为是我什么地方有问题导致内存泄漏。     
PS：这几天一直在做UI相关的东西，有点偏离方向，我觉得我应该把精力放在用Opencv实现一些小机器视觉算法功能上，而不是专注于写UI。

- 2017.12.19

Added a small function, there will be a list in the pop-up window when you choose multiple pictures.     

添加个小功能，在选取多张图片时，会弹出列表。         

- 2017.12.16 #2

Fixed the problem in the selectrect class and now the selectrect class can fit ImageWidget class when the size of ImageWidget changed(overrided resizeEvent in ImageWidget and added a pair of signal and slot).     

修复了selectrect类的问题，现在selectrect类可以随ImageWidget类的大小变化了（重写了ImageWidget的resizeEvent并增加了一对信号槽）。

- 2017.12.16 #1

Updated mat_qimage_convert.cpp and now the Mat(QImage) can be converted to QImage(Mat) by deep copy(safer) or shallow copy(faster).    
What's more, it took about 33ms to process the same image compared with the last version, because I chose shallow copy.      
But the selectrect class can't fit ImageWidget class when the size of ImageWidget changed. 

更新了mat_qimage_convert.cpp，现在Mat和QImage可以用深拷贝（更安全）或浅拷贝（更快）的方式来进行转换。     
由于使用了浅拷贝，现在处理与上次相同的图片需要33ms左右。但是selectrect类在窗口变化时不能自适应ImageWidget类的大小。

- 2017.12.15

Nothing changed, just updated Qt to 5.10.0 version.      
But processed the same image only take 50ms compared with the last version. The most time-consuming part is Mat2QImage(when process big image) and recognize_characters function in LPRecognizer class.

什么都没有干，只是升级到了Qt5.10.0版本，处理与上次相同的图片需要50ms左右。已经探查到耗时的部分为Mat2QImage在转换大尺寸三通道图像和LPRecognizer在recognize_characters函数中识别汉字。

- 2017.12.12

Fixed ImageWidget when code main program, added some new function, and designed UI, added a exception-handling function to recall initialization failure.      
I will improve processing speed when I'm free, but now I need to do something very shit. :( 

在写主程序时修改了ImageWidget，添加了一些功能，大致设计了一下UI，给LPRecognizer添加了一个异常处理（响应初始化失败事件）。有空再优处理速度，要去忙别的了。 :(

- 2017.12.11


Finished rebuilding and saved the old version files.      
It will take 70ms to get the output(image size: 3086 * 887), it can be improved after checking the most time-consuming part.

完成重写，备份旧版本文件，目前处理3086 * 887大小图片要70ms左右（与上次测试图片相同），应该还可以更快，检查耗时的部分。

- 2017.12.05

Ready to rebuild this shit.      
It will take 100ms to get the output(image size: 3086 * 887).

准备抽时间重写这个了，或许界面也应该重写一下吧？目前处理3086 * 887大小图片要130ms。

- 2017.11.27

Updated bwareaopen function and now it's similar to the one in matlab (relies on connectedComponentsWithStats function in opencv3.3.1).

更新了bwareaopen函数，现在和matlab中的bwareaopen函数效果差不多了(依赖opencv3.3.1中的connectedComponentsWithStats函数）.

- 2017.09.21

Fixed a bug in bwareaopen when using opencv3.3(it seems to be that findContours has some changes).

升级到opencv3.3时修复了bwareaopen函数的错误(findContours函数在opencv3.3时好像发生了变换，不会对原图进行修改了)。







