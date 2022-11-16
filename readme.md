# 图形学大作业

## 运行环境

- OS: Ubuntu 20.04
- lib: freeglut3-dev
- gcc: gcc version 9.4.0

## Quick run

```shell
sudo apt install make gcc freeglut3-dev
make main
./main nilu.obj
```

## 说明

本程序使用 OpenGL glut 库实现了一个简单的图形渲染引擎，支持：

- .obj 文件的加载
- mesh 对象的平移、旋转、放缩变换
- 平移和旋转的 Bézier 插值动画
- 正投影、透视投影的切换
- 自由视角、固定视角的切换
- 鼠标点击选中物体

## 实验报告

[实验报告](report/report.md)