Windows Path环境变量助手
========================

windows自带的环境变量设置工具设置PATH这样的变量时不方便，本工具能辅助你设置。

EnvVars非MFC实现版
__________________

此版本将完全采用windows api实现。
源码采用少量的C++代码处理String, Vector, Map等数据结构，其余主要是C代码。
源码大量采用宏替换，以求减少代码量，所以看起来会有点奇特。

	运行平台：WinXP/Win7
	编写语言：C/C++，STL, win32 api
	构建工具：Visual C++ 6.0或以上
	技术支持：[X86编程之家](http://www.x86pro.com)
