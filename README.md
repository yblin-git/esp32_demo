# 介绍
    此项目把esp-idf作为submodule添加到工程里的，所以需要以下步骤
1. 首先下载此工程
```
    git clone https://github.com/yblin-git/esp32_demo.git
```
2. 更新所有子项目
```
    git submodule update --init --recursive
```
3. 根据官方文档，搭建环境
   - 根目录下dep/esp-idf目录就是官方的sdk,可根据下面链接搭IDF的环境
    https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/linux-macos-setup.html
4. 先source setup.sh
5. 执行./build.sh 或者 idf.py build
    执行就会编译
6. 执行./flash.sh 或者 idf.py -p (PORT) flash
   下发镜像 默认/dev/ttyUSB0
