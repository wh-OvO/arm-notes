首先要理解为什么用cmake不用makefile：
{
CMake是一个“元构建系统”（Meta - Build - System）：
 {
        不直接编译代码，而是根据一份手写的或IDE生成的跨平台文件（CMakeLists.txt）生成本地平台的构建文件（Makefile、Visual Studio工程文件等）
        ，然后再用本地平台的构建工具去编译代码,完美兼容。
 }
    Cmake的工作流严格分为三步：
 {
       （1）配置阶段（configure）：
       {
            读取CMakeList.txt文件，检查系统环境，生成本地平台的构建文件（Makefile、Visual Studio工程文件等）
            还会再生成一份CMakeCache.txt文件，里面记录了配置阶段产生的缓存变量和选项，例如编译器路径、构建类型等，方便下次生成直接读缓存。
       }
 }     （2）生成阶段（generate）：
 {     
            根据配置阶段生成的本地平台构建文件，生成最终的编译命令。
            不同平台对应的构建文件不同，但actually都被 ninja 大人一脚踢死了,所以学长让你用ninja
       
 }     （3）构建阶段（build）：
 {
            调用ninja->调用编译器进行编译、链接
            产出可执行文件
 }
}
--------规范做法-- >> > 源码外构建（Out - of - Source - Build）：
{
    1.在源码目录外根目录里新建一个build目录
    2.进入build目录，执行cmake .. / src
    3.执行ninja
}   这也是为什么你会有有一个build/独立目录，能保持源码目录干净，重编直接“rm -rf build”不烧心