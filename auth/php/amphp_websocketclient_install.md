## PHP安装composer及amphp/webscoket-client依赖

### 下载composer.phar

参考链接: [PHP安装composer](https://docs.phpcomposer.com/00-intro.html)

- 将composer.phar文件放到项目目录中
- 在composer.phar同级目录下新建文件composer.bat
-  在控制台执行echo @php "%~dp0composer.phar" %*>composer.bat
-  （sdk中已经包含了composer.phar和composer.bat，直接copy到项目目录中即可）

### 安装amphp/websocket-client依赖

- 执行命令composer config -g repo.packagist composer https://packagist.phpcomposer.com(执行这一步的原因是使用镜像加速下载安装过程, 参考链接[Packagist 镜像使用方法](https://pkg.phpcomposer.com/))

- 从 [amp/websocket-client](https://github.com/amphp/websocket-client)上下载composer.json到当前目录，执行命令 php composer.phar install

- 执行完毕，项目目录下会出现vender文件，继续执行命令composer require amphp/websocket-client

- 执行成功后，vender/amphp目录下会出现websocket-client文件夹

  ###### 附：websocket-client所需要的vender目录已经给出，可以直接解压到自己项目文件夹下使用



