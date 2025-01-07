class Shape {
     constructor(type, x, y, angle, attribute) {
          this.type = type;    // 物体类型 (圆形、正三角形等)
          this.x = x;          // X 坐标
          this.y = y;          // Y 坐标
          this.angle = angle;  // 旋转角度 (弧度)
          this.attribute = attribute;  // 物体的属性（半径、边长等）
     }

     updateFromBuffer(dataView, offset) {
          // 从 dataView 解包几何体数据，并更新 Shape 实例
          this.type = dataView.getUint8(offset.value);
          offset.value += 1;

          this.x = dataView.getFloat32(offset.value, true); // 小端序
          offset.value += 4;

          this.y = dataView.getFloat32(offset.value, true);
          offset.value += 4;

          this.angle = dataView.getFloat32(offset.value, true);
          offset.value += 4;

          switch (this.type) {
               case 0: // 圆形
                    this.attribute = dataView.getFloat32(offset.value, true);
                    offset.value += 4; // 半径
                    break;
               case 1: // 正三角形
                    this.attribute = dataView.getFloat32(offset.value, true);
                    offset.value += 4; // 边长
                    break;
               case 2: // 正方形
                    this.attribute = dataView.getFloat32(offset.value, true);
                    offset.value += 4; // 边长
                    break;
               case 3: // 长方形
                    this.attribute = {
                         width: dataView.getFloat32(offset.value, true),
                         height: dataView.getFloat32(offset.value + 4, true)
                    };
                    offset.value += 8; // 宽度 + 高度
                    break;
               case 4: // 正五边形
                    this.attribute = dataView.getFloat32(offset.value, true);
                    offset.value += 4; // 内切圆半径
                    break;
               default:
                    throw new Error('Unknown shape type');
          }
     }

     draw(camera, fillStyle) {
          let ctx = camera.ctx;
          ctx.save();
          ctx.beginPath();
          ctx.translate((this.x - camera.x) * camera.scale, (this.y - camera.y) * camera.scale);
          ctx.rotate(this.angle);

          //路径start
          if (this.type == 0) {//圆                         
               ctx.arc(0, 0, this.attribute * camera.scale, 0, Math.PI * 2);
          } else if (this.type == 1) {//正三角形
          } else if (this.type == 2) {//正方形
          } else if (this.type == 3) {//长方形
          } else if (this.type == 4) {//正五边形
          } else {
          }
          //路径end

          ctx.fillStyle = fillStyle;
          ctx.strokeStyle = "#555555";
          ctx.lineWidth = 3;
          ctx.fill();
          ctx.stroke();
          ctx.restore();
     }
}

class Barrel {
     constructor(width, length, angle) {
          this.width = width;
          this.length = length;
          this.angle = angle;
          this.updateAngleable = true;//是否从服务器数据包解包角度数据
     }

     updateFromBuffer(dataView, offset) {
          // 从 dataView 解包炮管数据，并更新 炮管 实例
          // this.type = dataView.getUint8(offset.value);
          // offset.value += 1;

          this.width = dataView.getFloat32(offset.value, true); // 小端序
          offset.value += 4;

          this.length = dataView.getFloat32(offset.value, true);
          offset.value += 4;

          if (this.updateAngleable) { this.angle = dataView.getFloat32(offset.value, true); }
          offset.value += 4;
     }

     draw(player, camera) {
          let ctx = camera.ctx;

          ctx.save();
          ctx.beginPath();
          ctx.fillStyle = "#b7b7b7";
          ctx.strokeStyle = "#727272";
          ctx.lineJoin = "round";
          ctx.lineWidth = 3;
          ctx.translate((player.shape.x - camera.x) * camera.scale, (player.shape.y - camera.y) * camera.scale);
          ctx.rotate(this.angle);
          ctx.rect(0, -this.width * camera.scale / 2, this.length * camera.scale, this.width * camera.scale);
          ctx.fill();
          ctx.stroke();
          ctx.restore();
     }
}

class Player {
     // 当前操作的玩家的ID
     static nowPlayerID = null;

     static nowPlayer = null;
     // 玩家列表
     static playerMap = new Map();

     constructor(ID, groupIndex, health, maxHealth, gold, score, shape = null, barrel = null) {
          this.ID = ID;
          this.groupIndex = groupIndex;
          this.health = health;
          this.maxHealth = maxHealth;
          this.gold = gold;
          this.score = score;
          this.shape = shape || new Shape();  // 包含一个 Shape 对象
          this.barrel = barrel || new Barrel();
     }

     static updateFromBuffer(dataView, offset) {
          // 解包玩家数据
          let ID = dataView.getBigUint64(offset.value, true);
          offset.value += 8;

          let tmpPlayer;
          let addMap = false;
          if (Player.playerMap.has(ID))
               tmpPlayer = Player.playerMap.get(ID);
          else { tmpPlayer = new Player(); addMap = true; }

          tmpPlayer.ID = ID;
          tmpPlayer.groupIndex = dataView.getInt32(offset.value, true);
          offset.value += 4;
          tmpPlayer.health = dataView.getInt32(offset.value, true);
          offset.value += 4;
          tmpPlayer.maxHealth = dataView.getInt32(offset.value, true);
          offset.value += 4;
          tmpPlayer.gold = dataView.getInt32(offset.value, true);
          offset.value += 4;
          tmpPlayer.score = dataView.getInt32(offset.value, true);
          offset.value += 4;

          // 解包几何体
          tmpPlayer.shape.updateFromBuffer(dataView, offset);

          if (ID == Player.nowPlayerID) {
               Player.nowPlayer = tmpPlayer;
               Player.nowPlayer.barrel.updateAngleable = false;
          }

          //解包炮管
          tmpPlayer.barrel.updateFromBuffer(dataView, offset);


          if (addMap)
               Player.playerMap.set(ID, tmpPlayer);
          return tmpPlayer;
     }

     draw(camera) {

          //炮管
          this.barrel.draw(this, camera);
          //身体
          if (this.ID == Player.nowPlayerID) {
               this.shape.draw(camera, "#66ccff");
               //金币/得分/血量
               camera.ctx.save();
               camera.ctx.scale(1, -1);
               camera.ctx.font = "20px Arial"; // 设置字体
               camera.ctx.fillStyle = "black";
               camera.ctx.textAlign = 'center';
               camera.ctx.fillText("￥:"+this.gold + " XP:" + this.score + " HP:" + this.health, 0, camera.canvas.height / 2 - 30);
               camera.ctx.restore();
          }
          else
               this.shape.draw(camera, "#ff5151");
          //血条
          this.drawHealthBar(camera);
     }

     drawHealthBar(camera) {
          let ctx = camera.ctx;
          ctx.save();
          ctx.translate((this.shape.x - camera.x) * camera.scale, (this.shape.y - camera.y) * camera.scale);

          ctx.beginPath();
          ctx.fillStyle = "#006400";
          ctx.fillRect(-this.maxHealth / 2, -this.shape.attribute * camera.scale - 15, this.maxHealth, 10);

          ctx.beginPath();
          ctx.fillStyle = "#90EE90";
          ctx.fillRect(-this.maxHealth / 2, -this.shape.attribute * camera.scale - 15, this.health, 10);


          ctx.strokeStyle = "#000000";
          ctx.lineWidth = 3;
          ctx.strokeRect(-this.maxHealth / 2, -this.shape.attribute * camera.scale - 15, this.maxHealth, 10);

          ctx.restore();
     }
}

class Camera {
     constructor(offscreenCanvas, canvas, ctx) {
          //用于显示的坐标
          this.x = 0;
          this.y = 0;
          this.alpha = 0.1;//[0,1]插值因子
          //玩家的坐标
          this.targetX = 0;
          this.targetY = 0;
          this.scale = 500;

          //前景
          this.canvas = canvas;
          this.ctx = ctx;
          this.ctx.translate(canvas.width / 2, canvas.height / 2);
          this.ctx.scale(1, -1); // 反转 y 轴ctx

          // 离屏画布，用来绘制背景网格
          this.offscreenCanvas = offscreenCanvas;
          this.initBackground(offscreenCanvas, 25, "#e3e3e3", "#cfcfcf");
     }
     updateFromBuffer(dataView, offset) {
          this.targetX = dataView.getFloat32(offset.value, true);
          offset.value += 4;
          this.targetY = dataView.getFloat32(offset.value, true);
          offset.value += 4;
          this.scale = dataView.getUint16(offset.value, true);
          offset.value += 2;

          //插值更新直接显示的坐标
          let addX = this.alpha * (this.targetX - this.x);
          let addY = this.alpha * (this.targetY - this.y);
          this.x += addX;
          this.y += addY;
     }
     //渲染背景
     drawBackground() {
          this.ctx.save()
          this.ctx.scale(1, -1); // 反转 y 轴
          this.ctx.drawImage(this.offscreenCanvas, this.x * this.scale - this.canvas.width / 2 + this.offscreenCanvas.width / 2,
               -(this.y * this.scale + this.canvas.height / 2) + this.offscreenCanvas.height / 2, this.canvas.width, this.canvas.height,
               -this.canvas.width / 2, -this.canvas.height / 2, this.canvas.width, this.canvas.height);
          this.ctx.restore();
     }
     // 初始化背景
     initBackground(canvas, gridSize, gridColor, lineColor) {
          const ctx = canvas.getContext('2d');

          // 填充背景色(无效区域)
          ctx.fillStyle = "#808080";
          ctx.fillRect(0, 0, canvas.width, canvas.height);

          // 填充背景色(有效区域)
          ctx.fillStyle = gridColor;
          ctx.fillRect(1.5 * 500, 1.5 * 500, canvas.width - 3 * 500, canvas.height - 3 * 500);

          // 设置网格线样式
          ctx.strokeStyle = lineColor;
          ctx.lineWidth = 1;

          // 绘制网格线
          ctx.beginPath();
          for (let x = 0; x < canvas.width; x += gridSize) {
               ctx.moveTo(x, 0);
               ctx.lineTo(x, canvas.height);
          }
          for (let y = 0; y < canvas.height; y += gridSize) {
               ctx.moveTo(0, y);
               ctx.lineTo(canvas.width, y);
          }
          ctx.stroke(); // 一次性绘制所有线条
     }

}

class WebFrameStats {
     constructor(websocket) {
          this.lastTime = Date.now(); // 上一帧的时间戳
          this.lastTimeFPS = this.lastTime; // 上一次更新FPS的时间
          this.frameTime = 0;  // 帧间隔
          this.FPS = 0;  // 帧率
          this.FPSCounter = 0;  // 计算每秒钟的帧数
          this.ping = 0; //延迟
          this.lastSendTime = this.lastTime; //上一次发送ping的时间
          this.websocket = websocket;
     }

     update() {
          const currentTime = Date.now();  // 获取当前时间戳
          this.frameTime = currentTime - this.lastTime;  // 计算帧间隔
          this.lastTime = currentTime;  // 更新上一帧时间戳

          this.FPSCounter++;  // 增加每一帧的计数

          // 每秒更新一次FPS 和 ping
          if (currentTime - this.lastTimeFPS >= 1000) {
               this.FPS = this.FPSCounter;  // 当前秒的帧数
               this.FPSCounter = 0;  // 重置帧数计数
               this.lastTimeFPS = currentTime;
               if (this.websocket.readyState === WebSocket.OPEN) {
                    this.lastSendTime = currentTime;
                    this.websocket.send(new Uint8Array([0x03]));
               }
          }
     }

     updatePing(recvTime) {
          this.ping = recvTime - this.lastSendTime;
     }
}

class Minimap {
     constructor(canvas, worldWidth, worldHeight) {
          this.canvas = canvas; // 小地图的 canvas
          this.ctx = canvas.getContext('2d');
          this.worldWidth = worldWidth; // 游戏世界的宽度
          this.worldHeight = worldHeight; // 游戏世界的高度
     }

     draw(player, webFrameStats) {
          // 清空画布
          this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

          // 绘制背景
          this.drawBackground();

          // 绘制玩家位置和方向
          this.drawPlayer(player.shape.x, player.shape.y);

          //绘制顶部的状态信息（FPS, 帧间隔, ping）
          this.drawStats(webFrameStats);
     }

     // 绘制小地图的背景
     drawBackground() {
          // 设置背景颜色
          this.ctx.fillStyle = "#b0e0e6"; // 浅蓝色背景
          this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

          // 绘制边框
          this.ctx.strokeStyle = "#000000"; // 黑色边框
          this.ctx.lineWidth = 2;
          this.ctx.strokeRect(0, 0, this.canvas.width, this.canvas.height);
     }

     // 绘制玩家位置及朝向
     drawPlayer(playerX, playerY) {
          this.ctx.save();

          this.ctx.translate(this.canvas.width / 2, this.canvas.height / 2);
          this.ctx.scale(1, -1); // 反转 y 轴ctx

          const scale = 20; // 1m : 500px (x * 500 / (10 * 500) * 200)

          // 玩家在小地图上的坐标
          const xOnMinimap = playerX * scale;
          const yOnMinimap = playerY * scale;

          // 绘制玩家的位置：红色圆点
          this.ctx.fillStyle = "red"; // 玩家为红色
          this.ctx.beginPath();
          this.ctx.arc(xOnMinimap, yOnMinimap, 3, 0, Math.PI * 2); // 绘制玩家位置
          this.ctx.fill();

          this.ctx.restore();
     }

     // 绘制状态信息（FPS, 帧间隔, ping）
     drawStats(webFrameStats) {
          // 设置文字样式
          this.ctx.font = "14px Arial"; // 设置字体
          // 绘制FPS
          this.ctx.fillStyle = webFrameStats.FPS <= 30 ? "red" : "black";
          this.ctx.fillText("FPS: " + webFrameStats.FPS, 10, 20);

          // 绘制帧间隔
          this.ctx.fillStyle = webFrameStats.frameTime > 33 ? "red" : "black";
          this.ctx.fillText("FrameTime: " + webFrameStats.frameTime + " ms", 10, 40);

          // 绘制Ping
          this.ctx.fillStyle = webFrameStats.ping > 100 ? "red" : "black";
          this.ctx.fillText("Ping: " + webFrameStats.ping + " ms", 10, 60);
     }
}

class PlayerOperation {
     static keyMapping = {
          'left': 0,    // 左键
          'right': 1,   // 右键
          'w': 2,       // W
          'a': 3,       // A
          's': 4,       // S
          'd': 5        // D
     };
     constructor() {
          this.mouseX = 0.0;
          this.mouseY = 0.0;
          this.keyStatus = new Array(64).fill(false);
     }

     onBlur() // 窗口失焦时
     {
          this.keyStatus.fill(false);
     }

     setKeyStatus(key, status) {
          let n = PlayerOperation.keyMapping[key];
          if (n !== undefined)
               this.keyStatus[n] = status;
     }

     packData() {
          let message = new Uint8Array(2 + 8 + 8);
          let offset = 0;
          message[offset++] = 2;
          message[offset++] = 0;
          // 打包鼠标位置（x 和 y）
          let view = new DataView(message.buffer);
          view.setFloat32(offset, this.mouseX, true);  // 小端字节序
          offset += 4;
          view.setFloat32(offset, this.mouseY, true);
          offset += 4;
          //打包按键
          let keyStatusBigInt = BigInt(0);  // 初始化为 0
          for (let i = 0; i < Object.keys(PlayerOperation.keyMapping).length; i++) {
               if (this.keyStatus[i]) {
                    keyStatusBigInt |= (1n << BigInt(i));  // 设置相应的位为 1
               }
          }
          view.setBigUint64(offset, keyStatusBigInt, true);
          return message;
     }
}

class Bullet {
     constructor() { }
     static drawBullet(dataView, offset, camera) {
          let groupIndex = dataView.getInt32(offset.value, true);
          offset.value += 4;
          let tmpShape = new Shape();
          tmpShape.updateFromBuffer(dataView, offset);
          if (Player.nowPlayer !== undefined && Player.nowPlayer !== null && groupIndex == Player.nowPlayer.groupIndex)
               tmpShape.draw(camera, "#66ccff");
          else
               tmpShape.draw(camera, "#ff5151");
     }
}