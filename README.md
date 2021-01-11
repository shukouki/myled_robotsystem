# myled_robotsystem
## ロボットシステム学　課題１　デバイスドライバ

##　内容
- 信号機を模したLEDの制御を主にします
- デバイスファイルに'1'を書き込むと信号の様にLEDが点灯します
  - 青色LED点灯時はブザー音が鳴ります
  - 信号モード終了時に信号機部分のLEDがすべて点灯します
  - (今回は青→黄→赤の周期を3回繰り返します)
- '0'を書き込むとすべてのLEDが消灯、ブザー音が止まります
- 'a'を書き込むと緑色のLEDが点滅し応じたブザー音も鳴ります
  - (こちらは警報をイメージしたものです)
- 信号機部分に使用しているLEDは各々点灯可能です
  - 'b'を書き込むと青色LEDが点灯します
  - 'y'を書き込むと黄色LEDが点灯します
  - 'r'を書き込むと赤色LEDが点灯します

## 実験器具
- Raspberry Pi4 ModelB
- LED(緑、青、黄、赤)各々1個
- 抵抗
- ブザー
- 配線コード
- ジャンパーワイヤ

## 回路

## 実行方法

    $ git clone https://github.com/shukouki/myled_robotsystem.git
    $ cd myled_robotsystem
    $ make
    $ sudo insmod myled.ko
    $ sudo chmod 666 /dev/myled0

## 操作方法

- 警報機モード(緑色LED点滅、ブザー音あり)

        $ echo a > /dev/myled0
        
- すべて消灯

        $ echo 0 > /dev/myled0
        
- 青色LED点灯

        $ echo b > /dev/myled0
        
- 黄色LED点灯

        $ echo y > /dev/myled0
        
- 赤色LED点灯

        $ echo r > /dev/myled0
        
- 信号機モード

        $ echo 1 > /dev/myled0
        
## 実演動画

[![](http://img.youtube.com/vi/bFy2kft-Sw4/0.jpg)](http://www.youtube.com/watch?v=bFy2kft-Sw4 "実演動画")

↑の画像をクリックするとYouTubeで再生されます

## 参考

https://github.com/ryuichiueda/robosys_device_drivers

https://github.com/GakuKuwano/robosys2020_led

## ライセンス

[GNU General Public License v3.0](https://github.com/shukouki/myled_robotsystem//blob//COPYING)
