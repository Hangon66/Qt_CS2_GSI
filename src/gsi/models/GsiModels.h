#ifndef GSIMODELS_H
#define GSIMODELS_H

#include <QString>
#include <QJsonObject>
#include <QMap>
#include "GsiEnums.h"

namespace GSI {

/**
 * @brief GSI 提供者信息（游戏客户端）
 */
struct Provider {
    QString name;
    int appid = 0;
    int version = 0;
    QString steamid;
    int timestamp = 0;

    Provider() = default;
    explicit Provider(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Provider &other) const;
    bool operator!=(const Provider &other) const { return !(*this == other); }
};

/**
 * @brief 回合数据
 */
struct Round {
    RoundPhase phase = RoundPhase::Unknown;
    StableBombState bomb = StableBombState::Unknown;
    Team winner = Team::Unknown;

    Round() = default;
    explicit Round(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Round &other) const;
    bool operator!=(const Round &other) const { return !(*this == other); }
};

/**
 * @brief 阶段倒计时
 */
struct PhaseCountdowns {
    CountdownPhase phase = CountdownPhase::Unknown;
    QString phaseEndsIn;

    PhaseCountdowns() = default;
    explicit PhaseCountdowns(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const PhaseCountdowns &other) const;
    bool operator!=(const PhaseCountdowns &other) const { return !(*this == other); }
};

/**
 * @brief 队伍信息
 */
struct TeamInfo {
    int score = 0;
    QString name;
    int consecutiveRoundLosses = 0;
    int timeoutsRemaining = 0;
    int matchesWonThisSeries = 0;

    TeamInfo() = default;
    explicit TeamInfo(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const TeamInfo &other) const;
    bool operator!=(const TeamInfo &other) const { return !(*this == other); }
};

/**
 * @brief 回合胜利记录
 */
struct RoundWins {
    QMap<int, RoundWinCondition> list;

    RoundWins() = default;
    explicit RoundWins(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const RoundWins &other) const;
    bool operator!=(const RoundWins &other) const { return !(*this == other); }
};

/**
 * @brief 地图信息
 */
struct MapModel {
    GameMode mode = GameMode::Unknown;
    QString name;
    MapPhase phase = MapPhase::Unknown;
    int round = 0;
    int numMatchesToWinSeries = 0;
    TeamInfo teamCT;
    TeamInfo teamT;
    RoundWins roundWins;
    int currentSpectators = 0;
    int souvenirsTotal = 0;

    MapModel() = default;
    explicit MapModel(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const MapModel &other) const;
    bool operator!=(const MapModel &other) const { return !(*this == other); }
};

} // namespace GSI

#endif // GSIMODELS_H
