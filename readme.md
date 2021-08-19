## 报文通信

### Server <-> Client (进程间)

#### `200` Connection Established

当 Client 连接到 Server 之后：

+ Server 向 Client 发送 `200` 信号，Client 接收后返回 `200` 信号，完成第一次通信
+ Server 在收到 `200` 信号后，会发送 `201` 信号

#### `201` InitChessboard



#### `300` Unreveal Chess Piece



#### `301` Move Chess Piece



#### `302` Eat Chess Piece



#### `303` Eat Chess Piece and Destroy Self

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

棋子的 InitID 与其兵种的对应关系如下。其中偶数为蓝方，奇数为红方。

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
+ 通信
+ 走游戏流程 计时器 判胜负