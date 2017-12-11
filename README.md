# Licence Plates Recognition based on Qt and opencv. 
Translated from the Matlab vision: https://github.com/WangHongshuo/Licence-Plates-Recognition-Matlab

I can upload files again after nearly 11 months due to the unstable network.

This program started in 11/10/2016 and finished in 02/14/2017. It was my first c++ program, I'm a beginer, but I learned a lot in coding this project. It's better than only read the book. I translated it from Matlab script to C++ with Opencv 3.2, it could work in my test but still have some bugs that can lead to terrible crash. But I don't want to fix it by now because it was my first program with UI and it reads badly. I think if I want to recode it, I will write a class and use struct.

Ahhhhhhhh, my English is also bad.

在被墙了11个月后我又可以上github了。

这个程序从2016.11.10开始的，在2017.02.14结束的，是我第一个具备完整功能的程序，也是我0基础写出来的，在写的过程中真是学到了很多东西。用了Qt库写界面，opencv库处理图像。直接从Matlab脚本语言硬翻到C艹的，全是面向过程语言，但是可以工作，虽然有些时候会导致错误。因为是第一次写，所以写的很粗糙，我现在也改不动了，如果要改的话，那就重写好了。再重写我一定要写成类的形式，那个时候还没有类的概念。

2017.09.21

fixed a bug in bwareaopen when using opencv3.3(it seems to be that findContours has some changes).

升级到opencv3.3时修复了bwareaopen函数的错误(findContours函数在opencv3.3时好像发生了变换，不会对原图进行修改了)。

2017.11.27

updated bwareaopen function and now it's similar to the one in matlab (relies on connectedComponentsWithStats function in opencv3.3.1).

更新了bwareaopen函数，现在和matlab中的bwareaopen函数效果差不多了(依赖opencv3.3.1中的connectedComponentsWithStats函数）.

2017.12.05

ready to rebuild this shit. It will take 100ms to get the output.

准备抽时间重写这个了，或许界面也应该重写一下吧？目前处理一张图片要100ms。

2017.12.11

finished rebuilding and saved the old version files. It will take 70ms to get the output, it can be improved after checking the most time-consuming part.

完成重写，备份旧版本文件，目前处理一张图片要70ms左右，应该还可以更快，检查耗时的部分。

