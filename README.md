# RootPlanner
程序开发环境：win10, vistual studio 2019        
语言:C++        
**语言标准：C++17**     
(若语言标准不为C\+\+17程序无法运行,下有vsitual studio 2019调整语言标准为C\+\+17的方法,若使用其他IDE,请自行更改)     
图形库:io2d (https://github.com/cristianadam/io2d)     
OSM数据解析:pugixml (https://pugixml.org)       
## 1. 安装io2d      
### 安装vcpkg     
官方教程:        
https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md     

在C盘根目录下创建一个文件夹src      
win+R输入cmd打开命令行界面   
输入`cd C:\src`     
即进入刚刚创建的文件夹      
输入`git clone https://github.com/microsoft/vcpkg`      
(若显示'git'不是内部或外部命令，也不是可运行的程序
或批处理文件。说明没有安装git,请自行搜索如何安装git)        
输入`.\vcpkg\bootstrap-vcpkg.bat`       
### 安装io2d
输入 `.\vcpkg\vcpkg install io2d`       
**这是最关键的一步,下载较慢且不显示进度条,耐心等待吧.还有可能遇到下载失败的情况,建议多次尝试且在睡觉的时候下载**       
**Emmmm......GoodLuck!**        
安装结束后输入`.\vcpkg\vcpkg integrate install`     
### 设置C++语言标准为C\+\+17
在VS2019中点击**项目**->**属性**->**C/C++**->**语言**->**C++语言标准**,在下拉框中改成**ISO C++17标准(/std:C\+\+17)**        
#### **<font color=red>如何检验安装成功？</font>**       
在Vs2019的项目中`#include <io2d.h>`     
如果程序能够正常运行,那么恭喜,安装成功！
## 2. 获取程序源代码
还是在命令行界面中,`cd`到一个你喜欢的地方,建议在其他地方新建一个文件夹      
输入`git clone https://github.com/WhereIsSr/RoutePlanner.git`       
成功之后打开VS2019,在刚才的目录下新建一个项目,点击添加现有项,将刚才克隆下来的文件全部导入       
运行程序        
## 3. 在程序中添加自己的算法
在运行程序时,你会发现:**你输入起始点和终点的坐标,按回车键,弹出的地图图像中会标出起始点和终点的位置,并且将他们之前的最短路径已经用橘色的粗线标了出来,这是这个程序给的示例算法迪杰斯特拉算法的运行结果**       

为了让你更好的理解这个程序是如何工作的,下面将介绍它的原理:    

**1. 加载地图数据**       
我们的数据来源是OpenStreetMap(https://www.openstreetmap.org), 相信你在新生课或者GIS原理课上已经接触过这个网站,在网站的上方有一个导出按钮,可以将目前屏幕显示的区域导出为OSM文件,OSM文件采用的是XML语言,我们利用pugi将其解析出来      
**解析出来的数据的储存结构(data.h)如下(伪代码)：  
点：struct Node {x, y}   
线：struct Way { Node 的集合 }
路：struct Road {way 的索引, type }
面：struct Multipolygon { 内边界:点的集合, 外边界:点的集合}
若干不同种类的面继承面的结构
以上是基本的数据类型,除了了解他们之外,你还需要了解你在实现算法时能够访问的数据:     
在Algorithm类中地图数据都储存在a_data中,比如我们可以用a_data->Nodes()[index]访问索引为index的点,用a_data->Ways()[index]访问索引为index的路,以此类推......**

**2. 获取起始点和终点坐标**     
在输出了提示了信息之后,在用户的输入中获取两点坐标,要特别注意:**OSM原始坐标是经纬度,在第一步中程序已经经过运算将其转为平面xy坐标,且坐标是经过了归一化的,因此输入的坐标需要在0-100之前,左下角为(0,0)点,右上角为(100,100)点**   

在正确获取了两点坐标之后,我们遍历所有**在路上的点**,寻找距离我们的起始点和终点最近的点      
(此处解释一下,因为我们要做的是路径规划,所以点选在建筑点或其他地物点是没有意义的,其他不在路上的点在本程序中仅仅起画图作用)       

**<要获取在路上的点的集合,我们就要对点到路创建一个映射,即每个路上点属于哪条路,此处只需知道这个工作已经完成,并且这个映射储存在index_node_to_road中,它的类型是unordered_map,比如说我要获取索引为381的点在哪条路上,那么访问index_node_to_road[381]就可以获取路的索引,因为一个点可能不仅仅属于一条路,所以返回的是一个集合,集合中每一个元素都是路的索引,通过a_data->Roads()[index]即可访问**

**3. 添加自己的算法**
添加算法的方法和位置都已在程序中以注释的形式标出:
1. 首先注释掉示例的迪杰斯特拉算法,只需要注释掉调用即可,不用注释掉函数体和声明(在此之前建议看懂示例算法,明白它干了什么)
2. 在algorithm.h文件中添加你自己函数的声明,在cpp文件中添加函数体,最后在main函数中调用

即:     
**<font color=red>给出的数据:a_data(Nodes, Ways, Roads),点到路的映射index_node_to_road  --->>  经过你的算法  --->> vector\<Nodes\>path</font>**

当然你也可以根据算法的需要对数据进行进一步的处理等等......
