## 报文通信

### Server <-> Client (进程间)

#### `200` OK

当 Client 连接到 Server 之后：

+ Server 向 Client 发送 `200` 信号，Client 接收后切换为 Ready
+ 之后 Client 向 Server 发送 `200` 信号，Server 接收后切换为 Ready

### Game <-> GameConnection (多线程间)

#### `以上进程间传递的所有带有*的信号`

#### `0 <Color> <Message>`

+ 说明：设置主界面的说明文字
+ 参数
  + Color: 报文的颜色(六位十六进制数，或是HTML支持的颜色标签)，如ff0000
  + Message: 说明文字.



## TODO

+ 还有一个 GAME_STATUS == READY 的 cancel 逻辑没有写
+ 断线的业务逻辑
+ edit constants