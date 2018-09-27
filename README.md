# Brainfuck

#### 项目介绍
[Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) 是一种非常适合用来打发无聊时间，且同时可以锻炼思维能力的微型程序设计语言。本项目为`Brainfuck`的一个解释器（注：不是编译器），可以在命令行中运行编写的`Brainfuck`代码并查看结果。

#### 软件架构

目录结构说明：

│  Brainfuck.sln                        VS解决方案配置文件

│  README.md                            README

│

└─Brainfuck                            项目目录

    │  Brainfuck.vcxproj                 VS项目配置文件

    │  Brainfuck.vcxproj.filters         VS项目结构配置文件

    │  Brainfuck.vcxproj.user            VS项目配置文件

	│

    │  interpreter.cpp                 解释器代码

    │  parser.cpp                       命令行参数解析代码

    │  main.cpp                         入口点

    │

    ├─example                          Hello, world! 例程目录

    │      helloworld.bf                一个例子

    │

    └─include                          用到的库/头文件

            declared.h                  函数声明及用到的异常类  

            throw_if_failed.hpp         GranDYu: THROW_IF_FAILED


#### 注意事项

- 本项目使用了 [C++ 17](https://isocpp.org/std/the-standard) 的少量新特性，所以需要使用支持新特性的编译器才能编译成功。
- 本项目使用了 [Boost库](https://www.boost.org/) 和 [GranDYu](https://gitee.com/spwang/GranDYu) 第三方库。
- 本项目虽然使用 [Visual Studio 2017](https://visualstudio.microsoft.com/) 开发，但是代码理论上可以在其他平台进行编译。

#### 使用说明

命令行参数解释如下：

![bf.exe](http://omjn1u0j6.bkt.clouddn.com/18-9-27/4058221.jpg)

使用的例子：

![example](http://omjn1u0j6.bkt.clouddn.com/18-9-27/78986759.jpg)

#### 参与贡献

1. 报名MSC
2. 没了
