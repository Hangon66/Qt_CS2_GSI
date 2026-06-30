#ifndef GSISERVICE_H
#define GSISERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>
#include <QVariant>
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
    // ====== Provider 信号 ======
    void providerNameChanged(const QString &previous, const QString &current);
    void providerTimestampChanged(int previous, int current);

    // ====== Map 信号 ======
    void mapNameChanged(const QString &previous, const QString &current);
    void mapPhaseChanged(MapPhase previous, MapPhase current);
    void mapRoundChanged(int previous, int current);
    void mapTeamCTScoreChanged(int previous, int current);
    void mapTeamTScoreChanged(int previous, int current);
    void mapSpectatorsChanged(int previous, int current);
    void mapSouvenirsChanged(int previous, int current);

    // ====== Round 信号 ======
    void roundPhaseChanged(RoundPhase previous, RoundPhase current);
    void roundStarted();
    void roundEnded(Team winner);
    void roundWon(Team previous, Team current);
    void roundWinAdded(int roundNum, RoundWinCondition condition);
    void roundWinsChanged(const RoundWins &previous, const RoundWins &current);

    // ====== Phase Countdowns 信号 ======
    void phaseCountdownsPhaseChanged(CountdownPhase previous, CountdownPhase current);
    void phaseCountdownsTimeChanged(const QString &previous, const QString &current);

    // ====== Player 信号 ======
    void playerNameChanged(const QString &previous, const QString &current);
    void playerClanChanged(const QString &previous, const QString &current);
    void playerSteamidChanged(const QString &previous, const QString &current);
    void playerTeamChanged(Team previous, Team current);
    void playerActivityChanged(Activity previous, Activity current);
    void playerObserverSlotChanged(int previous, int current);
    void playerSpecTargetChanged(const QString &previous, const QString &current);
    void playerPositionChanged(const Vector3D &previous, const Vector3D &current);
    void playerForwardChanged(const Vector3D &previous, const Vector3D &current);

    // ====== Player State 信号 ======
    void playerHpChanged(int previous, int current);
    void playerArmorChanged(int previous, int current);
    void playerHelmetChanged(bool previous, bool current);
    void playerMoneyChanged(int previous, int current);
    void playerFlashedChanged(int previous, int current);
    void playerSmokedChanged(int previous, int current);
    void playerBurningChanged(int previous, int current);
    void playerEquipmentValueChanged(int previous, int current);
    void playerRoundKillsChanged(int previous, int current);

    // ====== Player MatchStats 信号 ======
    void playerKillsChanged(int previous, int current);
    void playerDeathsChanged(int previous, int current);
    void playerAssistsChanged(int previous, int current);
    void playerScoreChanged(int previous, int current);
    void playerMvpsChanged(int previous, int current);

    // ====== Weapon 信号 ======
    void playerWeaponChanged(const Weapon &previous, const Weapon &current);
    void playerAmmoClipChanged(int previous, int current);
    void playerAmmoReserveChanged(int previous, int current);
    void playerWeaponsChanged(const WeaponsCollection &previous, const WeaponsCollection &current);

    // ====== AllPlayers 信号 ======
    void allPlayersJoined(const QString &steamId, const Player &player);
    void allPlayersLeft(const QString &steamId, const Player &player);
    void allPlayersHpChanged(const QString &steamId, int previous, int current);
    void allPlayersArmorChanged(const QString &steamId, int previous, int current);
    void allPlayersHelmetChanged(const QString &steamId, bool previous, bool current);
    void allPlayersMoneyChanged(const QString &steamId, int previous, int current);
    void allPlayersFlashedChanged(const QString &steamId, int previous, int current);
    void allPlayersSmokedChanged(const QString &steamId, int previous, int current);
    void allPlayersBurningChanged(const QString &steamId, int previous, int current);
    void allPlayersKillsChanged(const QString &steamId, int previous, int current);
    void allPlayersDeathsChanged(const QString &steamId, int previous, int current);
    void allPlayersAssistsChanged(const QString &steamId, int previous, int current);
    void allPlayersScoreChanged(const QString &steamId, int previous, int current);

    // ====== Bomb 信号 ======
    void bombStateChanged(BombState previous, BombState current);
    void bombPositionChanged(const Vector3D &previous, const Vector3D &current);
    void bombPlayerChanged(const QString &previous, const QString &current);

    // ====== Grenades 信号 ======
    void grenadeThrown(int id, const QSharedPointer<GrenadeBase> &grenade);
    void grenadeGone(int id, const QSharedPointer<GrenadeBase> &grenade);
    void grenadePositionChanged(int id, const Vector3D &previous, const Vector3D &current);
    void grenadeVelocityChanged(int id, const Vector3D &previous, const Vector3D &current);
    void grenadeLifetimeChanged(int id, const QString &previous, const QString &current);
    void grenadeEffectTimeChanged(int id, const QString &previous, const QString &current);

    // ====== 通用调试信号 ======
    void anyEvent(const QString &eventName, const QVariantList &args);
    void rawUpdate(const QJsonObject &json);

private:
    GsiListener *m_listener = nullptr;
    GsiUpdateHandler *m_updateHandler = nullptr;
    GsiServer *m_wsServer = nullptr;
    bool m_running = false;
};

} // namespace GSI

#endif // GSISERVICE_H
