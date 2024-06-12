# 模板目录说明

├─app
├─bsp
├─board
├─libraries
├─module
└─project

* app：存放应用程序，由用户编写
* bsp：存放和底层相关的支持包
* board：存放和板子初始化和链接文件
* libraries：存放各种库文件，CMSIS，芯片固件库，文件系统库，网络库等
* module：主要存放各种软件模块，比如软件定时器，PID,FIFO,状态机等
* project：存放工程文件（目前只支持MDK5）