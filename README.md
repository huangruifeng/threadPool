# threadPool

## 依赖环境

- cmake > 3.8.x
- c++ 
- c++11

## 生成项目
在项目根目录创建build文件夹：
```sh
mkdir build
cd build
```
使用cmake在build目录下生成项目
```sh
cmake ..
```
## 构建
可以使用cmake构建
```sh
cmake --build .
```
生成库位置在 build/lib 下

### windows 使用vs构建
生成项目后在build目录下找到项目文件threadpool.sln并使用vs打开(双击)
直接使用vs构建sh

### linux 使用make构建
在 build 目录下直接使用make

## 生成测试项目
在build 目录下使用cmake 生成测试项目sh
```sh
cmake .. -DENABLE_TEST=ON
```
### 构建运行测试项目
执行
```sh
cmake --build.
```
在build/bin/Debug 目录下找到程序test然后运行


