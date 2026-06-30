#ifndef GSISERVICE_H
#define GSISERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>
#include <QVariant>
#include <QStringList>
#include "GsiEnums.h"
#include "Vector3D.h"
#include "GsiModels.h"
#include "PlayerModels.h"
#include "BombModel.h"
#include "GrenadeModels.h"
#include "GameState.h"

namespace GSI {

class GsiListener;
class GsiUpdateHandler;
class GsiServer;

/**
 * @brief GSI 核心服务，统一信号发射器和协调器
 * @details 持有 GsiListener、GsiUpdateHandler、DifferManager，
 *          接收 CS2 GSI 推送并通过 Qt 信号通知上层
 */
class GsiService : public QObject {
    Q_OBJECT

public:
    explicit GsiService(QObject *parent = nullptr);
    ~GsiService() override;

    /** @brief 启动 GSI 服务 */
    void start(quint16 httpPort = 3000);

    /** @brief 停止 GSI 服务 */
    void stop();

    /** @brief 是否正在运行 */
    bool isRunning() const;

    /** @brief 获取最后一次游戏状态 */
    const GameState &lastState() const;

signals:
    // ====== Provider — 客户端基本信息 (游戏名、appid、steamid) ======
    void providerNameChanged(const QString &previous, const QString &current);   ///< 提供者名称 (如 "Counter-Strike: Global Offensive")
    void providerTimestampChanged(int previous, int current);                    ///< 推送时间戳

    // ====== Map — 地图、模式、比分 ======
    void mapNameChanged(const QString &previous, const QString &current);        ///< 当前地图名 (如 "de_dust2")
    void mapPhaseChanged(MapPhase previous, MapPhase current);                   ///< 比赛阶段 (warmup / intermission / gameover / live)
    void mapRoundChanged(int previous, int current);                             ///< 当前回合数
    void mapTeamCTScoreChanged(int previous, int current);                       ///< CT 方比分
    void mapTeamTScoreChanged(int previous, int current);                        ///< T 方比分
    void mapSpectatorsChanged(int previous, int current);                        ///< 观战人数
    void mapSouvenirsChanged(int previous, int current);                         ///< 纪念品数量

    // ====== Round — 回合阶段、胜负、炸弹状态 ======
    void roundPhaseChanged(RoundPhase previous, RoundPhase current);             ///< 回合阶段 (freezetime / over / live)
    void roundStarted();                                                         ///< 回合开始
    void roundEnded(Team winner);                                                ///< 回合结束
    void roundWon(Team previous, Team current);                                  ///< 回合获胜方变化
    void roundWinAdded(int roundNum, RoundWinCondition condition);               ///< 新增一条回合胜利记录
    void roundWinsChanged(const RoundWins &previous, const RoundWins &current);  ///< 回合胜利列表整体变化

    // ====== Phase Countdowns — 各阶段倒计时 (仅 GOTV/观察者可用) ======
    void phaseCountdownsPhaseChanged(CountdownPhase previous, CountdownPhase current); ///< 倒计时阶段 (live / over / bomb / defuse / warmup / paused / timeout)
    void phaseCountdownsTimeChanged(const QString &previous, const QString &current);   ///< 倒计时剩余秒数

    // ====== Player — 本地玩家基础信息 ======
    void playerNameChanged(const QString &previous, const QString &current);     ///< 玩家名称
    void playerClanChanged(const QString &previous, const QString &current);     ///< 战队标签
    void playerSteamidChanged(const QString &previous, const QString &current);  ///< Steam ID
    void playerTeamChanged(Team previous, Team current);                         ///< 所属队伍 (CT / T / Spectator)
    void playerActivityChanged(Activity previous, Activity current);             ///< 活动状态 (playing / textinput / menu)
    void playerObserverSlotChanged(int previous, int current);                   ///< 观察者槽位
    void playerSpecTargetChanged(const QString &previous, const QString &current); ///< 观战目标玩家
    void playerPositionChanged(const Vector3D &previous, const Vector3D &current); ///< 玩家世界坐标 (仅观察者)
    void playerForwardChanged(const Vector3D &previous, const Vector3D &current);  ///< 玩家朝向向量 (仅观察者)

    // ====== Player State — 本地玩家实时状态 ======
    void playerHpChanged(int previous, int current);              ///< 生命值
    void playerArmorChanged(int previous, int current);           ///< 护甲值
    void playerHelmetChanged(bool previous, bool current);        ///< 是否有头盔
    void playerMoneyChanged(int previous, int current);           ///< 当前金钱
    void playerFlashedChanged(int previous, int current);         ///< 闪光效果强度 (0-255)
    void playerSmokedChanged(int previous, int current);          ///< 烟雾效果强度 (0-255)
    void playerBurningChanged(int previous, int current);         ///< 燃烧效果强度 (0-255)
    void playerEquipmentValueChanged(int previous, int current);  ///< 装备总价值
    void playerRoundKillsChanged(int previous, int current);      ///< 本回合击杀数

    // ====== Player MatchStats — 本地玩家本局统计 ======
    void playerKillsChanged(int previous, int current);           ///< 总击杀
    void playerDeathsChanged(int previous, int current);          ///< 总死亡
    void playerAssistsChanged(int previous, int current);         ///< 总助攻
    void playerScoreChanged(int previous, int current);           ///< 总得分
    void playerMvpsChanged(int previous, int current);            ///< MVP 次数

    // ====== Weapon — 本地玩家武器信息 ======
    void playerWeaponChanged(const Weapon &previous, const Weapon &current);                     ///< 当前持武器变化
    void playerAmmoClipChanged(int previous, int current);                                       ///< 弹匣内弹药
    void playerAmmoReserveChanged(int previous, int current);                                    ///< 备弹量
    void playerWeaponsChanged(const WeaponsCollection &previous, const WeaponsCollection &current); ///< 武器栏整体变化

    // ====== AllPlayers — 所有玩家信息 (仅 HLTV / 观察者可用) ======
    void allPlayersJoined(const QString &steamId, const Player &player);   ///< 玩家加入
    void allPlayersLeft(const QString &steamId, const Player &player);     ///< 玩家离开
    void allPlayersHpChanged(const QString &steamId, int previous, int current);      ///< 玩家生命值
    void allPlayersArmorChanged(const QString &steamId, int previous, int current);   ///< 玩家护甲
    void allPlayersHelmetChanged(const QString &steamId, bool previous, bool current); ///< 玩家头盔
    void allPlayersMoneyChanged(const QString &steamId, int previous, int current);   ///< 玩家金钱
    void allPlayersFlashedChanged(const QString &steamId, int previous, int current);  ///< 玩家闪光
    void allPlayersSmokedChanged(const QString &steamId, int previous, int current);   ///< 玩家烟雾
    void allPlayersBurningChanged(const QString &steamId, int previous, int current);  ///< 玩家燃烧
    void allPlayersKillsChanged(const QString &steamId, int previous, int current);    ///< 玩家击杀
    void allPlayersDeathsChanged(const QString &steamId, int previous, int current);   ///< 玩家死亡
    void allPlayersAssistsChanged(const QString &steamId, int previous, int current);  ///< 玩家助攻
    void allPlayersScoreChanged(const QString &steamId, int previous, int current);    ///< 玩家得分

    // ====== Bomb — 炸弹状态 (仅观察者可用) ======
    void bombStateChanged(BombState previous, BombState current);                          ///< 炸弹状态 (carried / dropped / planted / defused / exploded)
    void bombPositionChanged(const Vector3D &previous, const Vector3D &current);           ///< 炸弹世界坐标
    void bombPlayerChanged(const QString &previous, const QString &current);               ///< 持炸弹玩家 Steam ID

    // ====== Grenades — 投掷物信息 (仅观察者可用) ======
    void grenadeThrown(int id, const QSharedPointer<GrenadeBase> &grenade);                ///< 投掷物被投出
    void grenadeGone(int id, const QSharedPointer<GrenadeBase> &grenade);                  ///< 投掷物消失
    void grenadePositionChanged(int id, const Vector3D &previous, const Vector3D &current); ///< 投掷物位置
    void grenadeVelocityChanged(int id, const Vector3D &previous, const Vector3D &current); ///< 投掷物速度
    void grenadeLifetimeChanged(int id, const QString &previous, const QString &current);   ///< 投掷物存在时间
    void grenadeEffectTimeChanged(int id, const QString &previous, const QString &current); ///< 投掷物效果持续时间

    // ====== 通用调试信号 ======
    void anyEvent(const QString &eventName, const QVariantList &args);  ///< 任意事件触发 (调试用)
    void rawUpdate(const QJsonObject &json);                            ///< 原始 JSON 推送 (调试用)

private:
    /** @brief 确保 CS2 GSI 配置文件已安装，返回配置文件路径（空表示未找到 CS2） */
    QString ensureGsiConfig(quint16 port);

    /** @brief 搜索 CS2 cfg 目录 */
    static QStringList findCs2CfgDirs();

    GsiListener *m_listener = nullptr;
    GsiUpdateHandler *m_updateHandler = nullptr;
    GsiServer *m_wsServer = nullptr;
    bool m_running = false;
};

} // namespace GSI

#endif // GSISERVICE_H
