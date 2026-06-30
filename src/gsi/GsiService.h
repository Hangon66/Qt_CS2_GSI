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
class LocalPlayerService;

/**
 * @brief GSI 核心服务，统一信号发射器和协调器
 * @details 持有 GsiListener、GsiUpdateHandler、DifferManager，
 *          接收 CS2 GSI 推送并通过 Qt 信号通知上层
 */
class GsiService : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造 GSI 核心服务。
     * @param parent Qt 父对象，用于内存管理。
     */
    explicit GsiService(QObject *parent = nullptr);
    ~GsiService() override;

    /**
     * @brief 启动 GSI 服务。
     * @param httpPort HTTP 监听端口，默认 3000。
     */
    void start(quint16 httpPort = 3000);

    /**
     * @brief 停止 GSI 服务。
     */
    void stop();

    /**
     * @brief 服务是否正在运行。
     * @return true 表示服务正在运行。
     */
    bool isRunning() const;

    /**
     * @brief 获取本地玩家服务。
     * @return LocalPlayerService 指针。
     */
    LocalPlayerService *localPlayer() const;

    /**
     * @brief 获取最后一次游戏状态。
     * @return 最近一次解析的 GameState 引用。
     */
    const GameState &lastState() const;

    /**
     * @brief 获取本地玩家的 Steam ID。
     * @return 本地玩家 SteamID64，未获取到时返回空字符串。
     */
    QString localSteamid() const;

    /**
     * @brief 判断 steamid 是否属于本地玩家。
     * @param steamid 待判断的 SteamID64。
     * @return true 表示是本地玩家。
     */
    bool isLocal(const QString &steamid) const;

signals:
    // ====== Auth — GSI 认证信息 ======
    /**
     * @brief 认证令牌变化。
     * @param previous 旧令牌。
     * @param current 新令牌。
     */
    void authTokenChanged(const QString &previous, const QString &current);
    /**
     * @brief 认证 IP 地址变化。
     * @param previous 旧 IP。
     * @param current 新 IP。
     */
    void authIpChanged(const QString &previous, const QString &current);

    // ====== Provider — 客户端基本信息 (游戏名、appid、steamid) ======
    /**
     * @brief 提供者名称变化（如 "Counter-Strike: Global Offensive"）。
     * @param previous 旧名称。
     * @param current 新名称。
     */
    void providerNameChanged(const QString &previous, const QString &current);
    /**
     * @brief 推送时间戳变化。
     * @param previous 旧时间戳。
     * @param current 新时间戳。
     */
    void providerTimestampChanged(int previous, int current);

    // ====== Map — 地图、模式、比分 ======
    /**
     * @brief 当前地图名变化（如 "de_dust2"）。
     * @param previous 旧地图名。
     * @param current 新地图名。
     */
    void mapNameChanged(const QString &previous, const QString &current);
    /**
     * @brief 比赛阶段变化（warmup / intermission / gameover / live）。
     * @param previous 旧阶段。
     * @param current 新阶段。
     */
    void mapPhaseChanged(MapPhase previous, MapPhase current);
    /**
     * @brief 当前回合数变化。
     * @param previous 旧回合数。
     * @param current 新回合数。
     */
    void mapRoundChanged(int previous, int current);
    /**
     * @brief CT 方比分变化。
     * @param previous 旧比分。
     * @param current 新比分。
     */
    void mapTeamCTScoreChanged(int previous, int current);
    /**
     * @brief T 方比分变化。
     * @param previous 旧比分。
     * @param current 新比分。
     */
    void mapTeamTScoreChanged(int previous, int current);
    /**
     * @brief 观战人数变化。
     * @param previous 旧人数。
     * @param current 新人数。
     */
    void mapSpectatorsChanged(int previous, int current);
    /**
     * @brief 纪念品数量变化。
     * @param previous 旧数量。
     * @param current 新数量。
     */
    void mapSouvenirsChanged(int previous, int current);

    // ====== Team Info — 队伍详细信息 ======
    /**
     * @brief 队伍名称变化。
     * @param team 队伍（CT / T）。
     * @param previous 旧名称。
     * @param current 新名称。
     */
    void teamNameChanged(Team team, const QString &previous, const QString &current);
    /**
     * @brief 队伍连续失败回合数变化。
     * @param team 队伍（CT / T）。
     * @param previous 旧数值。
     * @param current 新数值。
     */
    void teamConsecutiveRoundLossesChanged(Team team, int previous, int current);

    // ====== Round — 回合阶段、胜负、炸弹状态 ======
    /**
     * @brief 回合阶段变化（freezetime / over / live）。
     * @param previous 旧阶段。
     * @param current 新阶段。
     */
    void roundPhaseChanged(RoundPhase previous, RoundPhase current);
    /**
     * @brief 回合开始。
     */
    void roundStarted();
    /**
     * @brief 回合结束。
     * @param winner 获胜队伍。
     */
    void roundEnded(Team winner);
    /**
     * @brief 回合获胜方变化。
     * @param previous 旧获胜方。
     * @param current 新获胜方。
     */
    void roundWon(Team previous, Team current);
    /**
     * @brief 回合内炸弹状态变化（planted / exploded / defused）。
     * @param previous 旧状态。
     * @param current 新状态。
     */
    void roundBombChanged(StableBombState previous, StableBombState current);
    /**
     * @brief 新增一条回合胜利记录。
     * @param roundNum 回合编号。
     * @param condition 胜利条件。
     */
    void roundWinAdded(int roundNum, RoundWinCondition condition);
    /**
     * @brief 回合胜利列表整体变化。
     * @param previous 旧列表。
     * @param current 新列表。
     */
    void roundWinsChanged(const RoundWins &previous, const RoundWins &current);

    // ====== Phase Countdowns — 各阶段倒计时 (仅 GOTV/观察者可用) ======
    /**
     * @brief 倒计时阶段变化（live / over / bomb / defuse / warmup / paused / timeout）。
     * @param previous 旧阶段。
     * @param current 新阶段。
     */
    void phaseCountdownsPhaseChanged(CountdownPhase previous, CountdownPhase current);
    /**
     * @brief 倒计时剩余秒数变化。
     * @param previous 旧秒数。
     * @param current 新秒数。
     */
    void phaseCountdownsTimeChanged(const QString &previous, const QString &current);

    // ====== Player — 本地玩家基础信息 ======
    /**
     * @brief 玩家名称变化。
     * @param previous 旧名称。
     * @param current 新名称。
     */
    void playerNameChanged(const QString &previous, const QString &current);
    /**
     * @brief 战队标签变化。
     * @param previous 旧标签。
     * @param current 新标签。
     */
    void playerClanChanged(const QString &previous, const QString &current);
    /**
     * @brief Steam ID 变化。
     * @param previous 旧 SteamID64。
     * @param current 新 SteamID64。
     */
    void playerSteamidChanged(const QString &previous, const QString &current);
    /**
     * @brief 所属队伍变化（CT / T / Spectator）。
     * @param previous 旧队伍。
     * @param current 新队伍。
     */
    void playerTeamChanged(Team previous, Team current);
    /**
     * @brief 活动状态变化（playing / textinput / menu）。
     * @param previous 旧状态。
     * @param current 新状态。
     */
    void playerActivityChanged(Activity previous, Activity current);
    /**
     * @brief 观察者槽位变化。
     * @param previous 旧槽位。
     * @param current 新槽位。
     */
    void playerObserverSlotChanged(int previous, int current);
    /**
     * @brief 观战目标玩家变化。
     * @param previous 旧目标 SteamID64。
     * @param current 新目标 SteamID64。
     */
    void playerSpecTargetChanged(const QString &previous, const QString &current);
    /**
     * @brief 玩家世界坐标变化（仅观察者可用）。
     * @param previous 旧坐标。
     * @param current 新坐标。
     */
    void playerPositionChanged(const Vector3D &previous, const Vector3D &current);
    /**
     * @brief 玩家朝向向量变化（仅观察者可用）。
     * @param previous 旧朝向。
     * @param current 新朝向。
     */
    void playerForwardChanged(const Vector3D &previous, const Vector3D &current);

    // ====== Player State — 本地玩家实时状态 ======
    /**
     * @brief 生命值变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerHpChanged(int previous, int current);
    /**
     * @brief 护甲值变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerArmorChanged(int previous, int current);
    /**
     * @brief 头盔状态变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerHelmetChanged(bool previous, bool current);
    /**
     * @brief 当前金钱变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerMoneyChanged(int previous, int current);
    /**
     * @brief 闪光效果强度变化 (0-255)。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerFlashedChanged(int previous, int current);
    /**
     * @brief 烟雾效果强度变化 (0-255)。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerSmokedChanged(int previous, int current);
    /**
     * @brief 燃烧效果强度变化 (0-255)。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerBurningChanged(int previous, int current);
    /**
     * @brief 装备总价值变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerEquipmentValueChanged(int previous, int current);
    /**
     * @brief 本回合击杀数变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerRoundKillsChanged(int previous, int current);
    /**
     * @brief 本回合爆头击杀变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerRoundHeadshotKillsChanged(int previous, int current);
    /**
     * @brief 拆弹器有无变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerDefusekitChanged(bool previous, bool current);
    /**
     * @brief 本回合总伤害变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerRoundTotalDamageChanged(int previous, int current);

    // ====== Player MatchStats — 本地玩家本局统计 ======
    /**
     * @brief 总击杀变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerKillsChanged(int previous, int current);
    /**
     * @brief 总死亡变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerDeathsChanged(int previous, int current);
    /**
     * @brief 总助攻变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerAssistsChanged(int previous, int current);
    /**
     * @brief 总得分变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerScoreChanged(int previous, int current);
    /**
     * @brief MVP 次数变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerMvpsChanged(int previous, int current);

    // ====== Weapon — 本地玩家武器信息 ======
    /**
     * @brief 当前持武器变化。
     * @param previous 旧武器。
     * @param current 新武器。
     */
    void playerWeaponChanged(const Weapon &previous, const Weapon &current);
    /**
     * @brief 弹匣内弹药变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerAmmoClipChanged(int previous, int current);
    /**
     * @brief 备弹量变化。
     * @param previous 旧值。
     * @param current 新值。
     */
    void playerAmmoReserveChanged(int previous, int current);
    /**
     * @brief 武器栏整体变化。
     * @param previous 旧集合。
     * @param current 新集合。
     */
    void playerWeaponsChanged(const WeaponsCollection &previous, const WeaponsCollection &current);
    /**
     * @brief 活跃武器切换（玩家切换当前持枪）。
     * @param previous 旧活跃武器。
     * @param current 新活跃武器。
     */
    void playerActiveWeaponChanged(const Weapon &previous, const Weapon &current);

    // ====== Player 语义事件（二级推理） ======
    /**
     * @brief 玩家受到伤害（HP 减少）。
     * @param amount 伤害量（> 0）。
     */
    void playerTookDamage(int amount);
    /**
     * @brief 玩家死亡（HP 从 >0 → 0）。
     */
    void playerDied();
    /**
     * @brief 玩家重生（HP 从 0 → >0）。
     */
    void playerRespawned();
    /**
     * @brief 玩家获得击杀（kills +1）。
     * @param weaponName 击杀使用的武器名称。
     */
    void playerGotKill(const QString &weaponName);
    /**
     * @brief 玩家武器列表新增（拾取武器）。
     * @param newWeapons 新增的武器列表。
     */
    void playerWeaponsPickedUp(const QList<Weapon> &newWeapons);
    /**
     * @brief 玩家武器列表减少（丢弃武器）。
     * @param droppedWeapons 丢弃的武器列表。
     */
    void playerWeaponsDropped(const QList<Weapon> &droppedWeapons);

    // ====== AllPlayers — 所有玩家信息 (仅 HLTV / 观察者可用) ======
    /**
     * @brief 玩家加入。
     * @param steamId 玩家 SteamID64。
     * @param player 玩家数据快照。
     */
    void allPlayersJoined(const QString &steamId, const Player &player);
    /**
     * @brief 玩家离开。
     * @param steamId 玩家 SteamID64。
     * @param player 最后的玩家数据快照。
     */
    void allPlayersLeft(const QString &steamId, const Player &player);
    /**
     * @brief 玩家生命值变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersHpChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家护甲变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersArmorChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家头盔状态变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersHelmetChanged(const QString &steamId, bool previous, bool current);
    /**
     * @brief 玩家金钱变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersMoneyChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家闪光效果变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersFlashedChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家烟雾效果变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersSmokedChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家燃烧效果变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersBurningChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家击杀数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersKillsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家死亡数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersDeathsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家助攻数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersAssistsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家得分变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersScoreChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家本回合击杀数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersRoundKillsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家本回合爆头数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersRoundHeadshotKillsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家装备价值变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersEquipmentValueChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家 MVP 次数变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersMvpsChanged(const QString &steamId, int previous, int current);
    /**
     * @brief 玩家拆弹器有无变化。
     * @param steamId 玩家 SteamID64。
     * @param previous 旧值。
     * @param current 新值。
     */
    void allPlayersDefusekitChanged(const QString &steamId, bool previous, bool current);

    // ====== Bomb — 炸弹状态 (仅观察者可用) ======
    /**
     * @brief 炸弹状态变化（carried / dropped / planted / defused / exploded）。
     * @param previous 旧状态。
     * @param current 新状态。
     */
    void bombStateChanged(BombState previous, BombState current);
    /**
     * @brief 炸弹世界坐标变化。
     * @param previous 旧坐标。
     * @param current 新坐标。
     */
    void bombPositionChanged(const Vector3D &previous, const Vector3D &current);
    /**
     * @brief 持炸弹玩家 Steam ID 变化。
     * @param previous 旧 SteamID64。
     * @param current 新 SteamID64。
     */
    void bombPlayerChanged(const QString &previous, const QString &current);

    // ====== Bomb 语义事件（状态转换推理） ======
    /**
     * @brief 炸弹开始安装。
     * @param playerSteamId 安装者 SteamID64。
     */
    void bombPlanting(const QString &playerSteamId);
    /**
     * @brief 炸弹安装完成。
     * @param playerSteamId 安装者 SteamID64。
     */
    void bombPlanted(const QString &playerSteamId);
    /**
     * @brief 炸弹开始拆除。
     * @param playerSteamId 拆除者 SteamID64。
     */
    void bombDefusing(const QString &playerSteamId);
    /**
     * @brief 炸弹拆除完成。
     */
    void bombDefused();
    /**
     * @brief 炸弹掉落。
     * @param playerSteamId 掉落时持有者 SteamID64。
     */
    void bombDropped(const QString &playerSteamId);
    /**
     * @brief 炸弹被捡起。
     * @param playerSteamId 捡起者 SteamID64。
     */
    void bombPickedUp(const QString &playerSteamId);
    /**
     * @brief 炸弹爆炸。
     */
    void bombExploded();

    // ====== Grenades — 投掷物信息 (仅观察者可用) ======
    /**
     * @brief 投掷物被投出。
     * @param id 投掷物 ID。
     * @param grenade 投掷物数据指针。
     */
    void grenadeThrown(int id, const QSharedPointer<GrenadeBase> &grenade);
    /**
     * @brief 投掷物消失。
     * @param id 投掷物 ID。
     * @param grenade 投掷物数据指针。
     */
    void grenadeGone(int id, const QSharedPointer<GrenadeBase> &grenade);
    /**
     * @brief 投掷物位置变化。
     * @param id 投掷物 ID。
     * @param previous 旧位置。
     * @param current 新位置。
     */
    void grenadePositionChanged(int id, const Vector3D &previous, const Vector3D &current);
    /**
     * @brief 投掷物速度变化。
     * @param id 投掷物 ID。
     * @param previous 旧速度。
     * @param current 新速度。
     */
    void grenadeVelocityChanged(int id, const Vector3D &previous, const Vector3D &current);
    /**
     * @brief 投掷物存在时间变化。
     * @param id 投掷物 ID。
     * @param previous 旧时间。
     * @param current 新时间。
     */
    void grenadeLifetimeChanged(int id, const QString &previous, const QString &current);
    /**
     * @brief 投掷物效果持续时间变化。
     * @param id 投掷物 ID。
     * @param previous 旧时间。
     * @param current 新时间。
     */
    void grenadeEffectTimeChanged(int id, const QString &previous, const QString &current);

    // ====== Map 语义事件 ======
    /**
     * @brief 游戏模式变化。
     * @param previous 旧模式。
     * @param current 新模式。
     */
    void gameModeChanged(GameMode previous, GameMode current);
    /**
     * @brief 队伍剩余暂停次数变化。
     * @param team 队伍（CT / T）。
     * @param previous 旧次数。
     * @param current 新次数。
     */
    void teamTimeoutsRemainingChanged(Team team, int previous, int current);
    /**
     * @brief 队伍系列赛胜场变化。
     * @param team 队伍（CT / T）。
     * @param previous 旧胜场。
     * @param current 新胜场。
     */
    void teamMatchesWonChanged(Team team, int previous, int current);
    /**
     * @brief 热身开始。
     */
    void warmupStarted();
    /**
     * @brief 热身结束。
     */
    void warmupOver();
    /**
     * @brief 休息开始。
     */
    void intermissionStarted();
    /**
     * @brief 休息结束。
     */
    void intermissionOver();
    /**
     * @brief 比赛正式开始。
     */
    void matchStarted();
    /**
     * @brief 比赛结束。
     */
    void gameOver();

    // ====== Round 语义事件 ======
    /**
     * @brief 回合结束（含胜方和胜利条件）。
     * @param winner 获胜队伍。
     * @param condition 胜利条件。
     */
    void roundConcluded(Team winner, RoundWinCondition condition);

    // ====== PhaseCountdowns 语义事件 ======
    /**
     * @brief 冻结时间开始。
     */
    void freezetimeStarted();
    /**
     * @brief 冻结时间结束。
     */
    void freezetimeOver();
    /**
     * @brief 暂停开始。
     */
    void pauseStarted();
    /**
     * @brief 暂停结束。
     */
    void pauseOver();
    /**
     * @brief 战术暂停开始。
     */
    void timeoutStarted();
    /**
     * @brief 战术暂停结束。
     */
    void timeoutOver();
    /**
     * @brief 炸弹爆炸倒计时开始。
     */
    void bombCountdownStarted();
    /**
     * @brief 炸弹爆炸倒计时结束（爆炸或被拆除中断）。
     */
    void bombCountdownOver();
    /**
     * @brief 拆弹倒计时开始。
     */
    void defuseCountdownStarted();
    /**
     * @brief 拆弹倒计时结束（拆除完成或被中断）。
     */
    void defuseCountdownOver();

    // ====== 通用调试信号 ======
    /**
     * @brief 任意事件触发（调试用）。
     * @param eventName 事件名称。
     * @param args 事件参数列表。
     */
    void anyEvent(const QString &eventName, const QVariantList &args);
    /**
     * @brief 原始 JSON 推送（调试用）。
     * @param json 原始 JSON 对象。
     */
    void rawUpdate(const QJsonObject &json);

    // ====== 状态更新完成通知 ======
    /**
     * @brief 所有 Differ 执行完毕后发射，供包装层使用。
     */
    void stateUpdated();

private:
    /**
     * @brief 确保 CS2 GSI 配置文件已安装，返回配置文件路径。
     * @param port HTTP 监听端口，用于写入 GSI 配置。
     * @return 配置文件路径，空字符串表示未找到 CS2。
     */
    QString ensureGsiConfig(quint16 port);

    /**
     * @brief 搜索 CS2 cfg 目录。
     * @return 找到的 cfg 目录路径列表。
     */
    static QStringList findCs2CfgDirs();

    /**
     * @brief HTTP 监听器，接收 CS2 GSI POST 推送。
     */
    GsiListener *m_listener = nullptr;

    /**
     * @brief 更新处理器，负责解析 JSON 并调度 DifferManager 进行差异对比。
     */
    GsiUpdateHandler *m_updateHandler = nullptr;

    /**
     * @brief WebSocket 服务器，将 GSI 数据转发给前端可视化面板。
     */
    GsiServer *m_wsServer = nullptr;

    /**
     * @brief 本地玩家数据包装服务，提供过滤后的本地玩家专用信号。
     */
    LocalPlayerService *m_localPlayer = nullptr;

    /**
     * @brief 服务是否正在运行。
     */
    bool m_running = false;
};

} // namespace GSI

#endif // GSISERVICE_H
