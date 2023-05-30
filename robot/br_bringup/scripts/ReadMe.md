### 文件
- `br` : 服务`br` 放到`/etc/init.d/`目录下, 默认启动, 也可通过`sudo service br stop/start`命令运行与停止
- `BrManager.sh` : 进程`BrManager` 拷贝到`/home/cti/`下 服务`br`开启/关闭, 一直运行, 用于`navi/map`模式的切换
- `start_br.sh` : 启动`navi`模式的脚本 拷贝到`/home/cti/`下
- `start_map.sh` : 启动`map`模式的脚本 拷贝到`/home/cti/`下

- `navi` : 导航模式
- `map` : 建图模式
