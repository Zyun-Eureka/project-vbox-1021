# 开发内容
> DevBy: Eureka

## Date: Oct 20
1. 修改视频解码过程，重写 QAbstractVideoSurface 以获取视频流帧数据进行绘制
>  因原先VideoWidget使用的是GPU进行解码渲染，现在使用继承重写的myVideoSurface类，导致原先GPU内容变由CPU执行，致使CPU占用率微小提升，GPU占用率下降
2. 修改绘制函数,实现方框绘制，文字绘制等
3. 可双击 智能安检 标题栏填写参数进行测试 (效果见以下截图)
> 原先双击进入全屏失效，可在老版本测试体验
4. 视频和图片列表中间添加滑块可自行拖动调整画面大小 (见以下截图)
5. 图片和实时数据中间添加滑块可自行拖动调整画面大小 (见以下截图)
   
### 绘制效果
![截图](./screenshot/Screenshot10-20-1.png)
### 滑块效果
![截图](./screenshot/Screenshot10-20-02.png)
## Date: Oct 15
1. 添加播放MP4视频选项
2. 添加右侧列表弹出倒计时 

具体倒计时可自行在代码处调整
``` cpp
// in widget.h
bool Widget::eventFilter(QObject *watched, QEvent *event){
    if(watched == flist){
        if(event->type()==QEvent::Enter){
            // 鼠标放置后多少毫秒弹出
            timer->start(3000);
    }
}
```
![截图](./screenshot/Screenshot10-15.png)

## Date: 241012
1. 移除VLC-QT库，无需添加vlc动态链接库
2. 无限制编译器，mingw和msvc均可
3. 右键标题栏选择摄像头即可打开
4. 图片列表可下拉，点击可选中图片
5. 图片详细信息因未计划显示信息内容暂无显示
![截图](./screenshot/Screenshot10-12.png)