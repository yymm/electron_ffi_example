# electron_ffi_example

[Electron(旧:Atom-Shell)勉強会 #1](http://atom-shell.connpass.com/event/13304/ "Electron(旧:Atom-Shell)勉強会 #1 - connpass")のLTの資料です。

[LTのスライド](http://yymm.github.io/electron_ffi_example/#13 "Slide")

# Screenshot
![](https://github.com/yymm/electron_ffi_example/blob/master/screenshot/screenshot1.png?raw=true)

# Overview
C言語で書いたdelaunay三角形分割のアルゴリズムを、node-ffiを介して使用しています。

具体的には、JS側で作成したランダムな点列(構造体の配列)をCで書いたライブラリの関数に渡して、その点に対して得られるドロネー三角形群を参照渡しの引数で受け取って描画しています。

詳しくは、[electron_ffi_example/app.coffee](https://github.com/yymm/electron_ffi_example/blob/master/src/app.coffee "electron_ffi_example/app.coffee at master · yymm/electron_ffi_example")を見るといいです。

# for dev

nodejs関係

```bash
$ nvm install iojs-v1.6.3
$ npm install
$ npm install -g gulp electron-prebuilt
(アプリの起動)
$ electron index.js
```

c関係

```bash
$ cd native
$ make
($ make test)
($ make clean)
```
