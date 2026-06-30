# QtGSI

CS2 (Counter-Strike 2) Game State Integration 客户端，基于 **Qt 6 / C++17** 构建。

接收 CS2 游戏通过 GSI 协议推送的实时状态数据（地图、回合、玩家、武器、炸弹、投掷物等），并通过 Qt 信号系统分发给 UI 面板进行可视化展示。

移植自 TypeScript 库 [cs2-gsi-z](https://github.com/nicholascwm/cs2-gsi-z)。

## 功能特性

- **实时 GSI 数据接收** — 内置 HTTP 服务器监听 CS2 推送（默认端口 3000）
- **差分检测引擎** — 仅在状态变化时发射信号，避免无效更新
- **完整数据模型** — 覆盖 Provider、Map、Round、Player、AllPlayers、Bomb、Grenades 全部 GSI 数据字段
- **Qt 信号系统** — 70+ 个细粒度信号，上层应用通过 `connect()` 即可消费
- **自动配置** — 启动时自动检测 CS2 安装目录并写入 GSI 配置文件
- **可视化面板** — 地图信息、玩家状态 (KDA/HP/武器)、事件日志三个面板

## 项目结构

```
QtGSI/
├── CMakeLists.txt              # 顶层 CMake
├── main.cpp                    # 入口
├── build.bat                   # 完整构建脚本 (配置+编译)
├── build_incremental.bat       # 增量编译
├── rebuild.bat                 # 全量重建
└── src/
    ├── gsi/                    # GSI 核心库
    │   ├── GsiService.h/cpp    # 核心服务 — 统一协调器和信号发射器
    │   ├── GsiUpdateHandler    # JSON 解析 → 状态更新 → Differ 调度
    │   ├── network/
    │   │   ├── GsiListener     # HTTP 服务器，接收 CS2 GSI POST
    │   │   ├── GsiServer       # WebSocket 服务器 (可选)
    │   │   └── GsiConfigWriter # CS2 GSI .cfg 配置文件生成
    │   ├── models/             # 数据模型 (GameState, Player, Bomb...)
    │   ├── differs/            # 差分检测器 (状态变化才发射信号)
    │   ├── enums/              # 枚举定义 (MapPhase, RoundPhase, BombState...)
    │   └── utils/              # JSON 解析工具
    └── ui/                     # Qt Widgets 界面
        ├── MainWindow          # 主窗口
        ├── MapWidget           # 地图/比分面板
        ├── PlayerWidget        # 玩家状态面板
        └── EventLogWidget      # 事件日志面板
```

## 环境要求

| 组件 | 版本 |
|------|------|
| Qt | 6.5+ (MSVC 2019 64-bit) |
| Visual Studio | 2019+ (需 C++ 桌面开发工作负载) |
| CMake | 3.19+ |
| Ninja | 推荐 (CMake 默认生成器) |

Qt 模块依赖: `Core` `Widgets` `Network` `WebSockets`

## 编译

### 使用构建脚本

```batch
REM 完整流程 (首次配置 + 编译)
build.bat

REM 增量编译 (日常开发)
build_incremental.bat

REM 全量重建 (clean + build)
rebuild.bat
```

### 使用 Qt Creator

在 Qt Creator 中打开 `CMakeLists.txt`，选择 MSVC 2019 64-bit Kit，点击构建即可。

### 手动命令行

```batch
REM 1. 初始化 MSVC 环境
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

REM 2. 配置
cmake -B build -G Ninja ^
      -DCMAKE_PREFIX_PATH=C:/Qt/6.7.3/msvc2019_64 ^
      -DCMAKE_CXX_COMPILER=cl

REM 3. 编译
cmake --build build --config Release

REM 4. (可选) 部署运行时 DLL
windeployqt6 --release build\QtGSI.exe
```

> **注意**: 如果系统 PATH 中存在 MinGW 工具链 (如 Strawberry Perl)，需要从 PATH 中移除以避免链接器冲突。

## 使用方法

### 1. 启动程序

```batch
QtGSI.exe
```

程序启动后会：
- 自动在 CS2 的 `cfg` 目录写入 `gamestate_integration_QtGSI.cfg`
- 在 `localhost:3000` 启动 HTTP 监听

### 2. 启动 CS2

正常启动游戏，CS2 会自动读取 cfg 目录下的 GSI 配置文件并开始推送数据。

### 3. 使用 GsiService 库

`GsiService` 可作为独立库集成到其他 Qt 项目中：

```cpp
#include "GsiService.h"

GSI::GsiService service;
service.start(3000);

// 监听特定事件
connect(&service, &GSI::GsiService::playerHpChanged,
        this, [](int prev, int curr) {
    qDebug() << "HP:" << prev << "->" << curr;
});

// 监听原始 JSON
connect(&service, &GSI::GsiService::rawUpdate,
        this, [](const QJsonObject &json) {
    qDebug() << json;
});
```

## GSI 配置文件

自动生成的 `gamestate_integration_QtGSI.cfg` 示例：

```
"QtGSI"
{
  "uri"          "http://localhost:3000"
  "timeout"      "3.0"
  "buffer"       "0.0"
  "throttle"     "0.0"
  "heartbeat"    "30.0"
  "data"
  {
    "provider"                  "1"
    "map"                       "1"
    "round"                     "1"
    "player_id"                 "1"
    "player_state"              "1"
    "player_weapons"            "1"
    "player_match_stats"        "1"
    "allplayers_id"             "1"
    "allplayers_state"          "1"
    "allplayers_match_stats"    "1"
    "allplayers_weapons"        "1"
    "allplayers_position"       "1"
    "allgrenades"               "1"
    "bomb"                      "1"
    "phase_countdowns"          "1"
    "map_round_wins"            "1"
    "player_position"           "1"
  }
}
```

手动安装：将此文件复制到 CS2 的 cfg 目录：
```
<Steam>\steamapps\common\Counter-Strike Global Offensive\game\csgo\cfg\
```

## 架构说明

```
CS2 Game ──HTTP POST──▶ GsiListener (HTTP Server)
                              │
                        QJsonObject
                              │
                        GsiUpdateHandler
                              │
                    JSON → GameState 解析
                              │
                        DifferManager
                              │
              ┌───────────────┼───────────────┐
              │               │               │
        MapDiffer      PlayerDiffers    BombDiffer ...
              │               │               │
              ▼               ▼               ▼
        GsiService 发射对应 Qt 信号
              │
    ┌─────────┼─────────┐
    ▼         ▼         ▼
MapWidget  PlayerWidget  EventLogWidget
```

- **GsiListener**: 内嵌 HTTP 服务器，接收 CS2 的 JSON POST 请求
- **GsiUpdateHandler**: 解析 JSON，更新 `GameState`，调度 DifferManager
- **DifferManager**: 管理多个 Differ，对比前后状态差异，仅在变化时发射信号
- **GsiService**: 对外暴露 70+ 个 Qt 信号，上层通过 `connect()` 消费

## 许可证

MIT License

## 致谢

- 移植自 [cs2-gsi-z](https://github.com/nicholascwm/cs2-gsi-z) (TypeScript)
- [Valve GSI 官方文档](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration)
