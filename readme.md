## 报文通信

### Server <-> Client (进程间)

#### `200` OK

当 Client 连接到 Server 之后：

+ Server 向 Client 发送 `200` 信号，Client 接收后切换为 WAIT_PLAY_CONFIRMATION
+ 之后 Client 向 Server 发送 `200` 信号，Server 接收后切换为 WAIT_PLAY_CONFIRMATION

### Game <-> GameConnection (多线程间)

#### `以上进程间传递的所有带有*的信号`

#### `0 <Color> <Message>`

+ 说明：设置主界面的说明文字
+ 参数
  + Color: 报文的颜色(六位十六进制数，或是HTML支持的颜色标签)，如ff0000
  + Message: 说明文字.

## 类的设计

### Game

以 GameLogic 类为友元.

+ 棋盘 board: `Vector<ChessPiece*>`



### ChesePiece

棋子的 InitID 与其兵种的对应关系如下。其中奇数为蓝方，偶数为红方。

| InitID | 兵种 |
| ------ | ---- |
| 1~2    | 军旗 |
| 3~4    | 司令 |
| 5~6    | 军长 |
| 7~10   | 师长 |
| 11~14  | 旅长 |
| 15~18  | 团长 |
| 19~22  | 营长 |
| 23~26  | 炸弹 |
| 27~32  | 连长 |
| 33~38  | 排长 |
| 39~44  | 工兵 |
| 45~50  | 地雷 |



## TODO

+ 还有一个 GAME_STATUS == READY 的 cancel 逻辑没有写
+ 断线的业务逻辑
+ Game::onPressedBoard 开了调试项目
+ 通信
+ 先写本地：可达性 可吃子

+ 走游戏流程 计时器 判胜负
+ 军旗最后被吃
+ 每一个 Turn 的流程：