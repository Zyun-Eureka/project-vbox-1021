# 开发内容
> DevBy: Eureka
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