# 这时一个简易的电池功耗计量插件

感谢你来到这里下载。本人的源码水平相当丑陋所以暂时不~~懒得~~开源。

# 日志

2024-05-22
> 使用最新构建工具重新构建。添加了ARM64EC平台的支持。

2024-07-21

> Added unit string change functionality.

2024-08-17

> ![1723880812341](image/README/1723880812341.png)  
> 添加所有电池相关的可选监控项目。 重构部分代码。可以调整单位字符设置。
>
2024-08-22

> ![image](https://github.com/user-attachments/assets/f2731d15-1c09-4169-b67c-181ea9b2b72c)  
> 添加了智能功耗显示，在使用电池的时候显示电池放电功耗，接入电源的时候显示所有GPU+CPU功耗。
> 重构了大量代码，加入了winrt运行时。 从本版本开始仅支持含有LibreHardwareMonitorLib.dll的TrafficMonitor
> 因为我也需要用。  
> 因为WinRT运行时和ARM64EC或原生不兼容，所以请ARM64用户暂时使用X64版本
> 请拷贝LibreHardwareMonitorLib.dll 替换原有的Traffic Monitor
>


2024-08-24
> 添加CPU和GPU监控模块。
>

2024-09-12
> Removed debug build.
> Attempted bug fix for setting's persistance.
> 

2024-09-14
> Fixed settings load caused internal sync failed.

2024-10-07

> Fixed string format issue.
> Added multiple new functions.
> Fixed battery time display, but futher improvements can be done.
> Added battery in sensor tree object.
> Recovered ARM64X compile config, but it's not been tested and does not have CPU and GPU monitor capibility. 
> Added digi length control.
> Added spacing control.

