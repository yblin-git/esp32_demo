# 介绍
    此项目把esp-idf作为submodule添加到工程里的，所以需要以下步骤
1. 首先下载此工程
```
    git clone https://gitee.com/xuhongv/esp-idf.git
```
2. 更新所有子项目
```
    git submodule update --init --recursive
```
3. 根据官方文档，搭建环境
    https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/linux-macos-setup.html
4. 执行./build.sh
    执行后会生成build_esp32
5. 进入build_esp32 编译
```
    cd build_esp32
    make all -j4
```