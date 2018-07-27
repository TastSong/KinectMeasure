KinectMeasure
==========================
环境配置
-------------------------
* VS2013
* [KinectV2 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=44561)
* [OpenCv2.4.9](https://www.opencv.org/releases.html)
#### `注：别忘将Kinect20.dll加入到项目→属性→链接器→输入→附加依赖项`

使用方法
==========================
* 摆放Kinect，用户应该距离Kinect 2.5米到3.5米左右的距离，高度应该在用户的膝盖以上腰部以下
* 进入程序运行界面
* 并通过彩色画面确定用户的身体完全被Kinect所采集
* 当用户准备好以后，右手握拳即可启动Kinect进行测量，测量时应保持身体直立，正面测量结束时，程序界面会出现相应数据
