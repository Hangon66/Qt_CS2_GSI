#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QJsonObject>
#include "GsiModels.h"
#include "PlayerModels.h"
#include "BombModel.h"
#include "GrenadeModels.h"
#include "AllPlayers.h"

namespace GSI {

/**
 * @brief 完整游戏状态，聚合所有子模型
 */
struct GameState {
    Provider provider;
    MapModel map;
    Round round;
    Player player;
    PhaseCountdowns phaseCountdowns;
    AllPlayers allPlayers;
    Bomb bomb;
    GrenadeList grenades;
    /** @brief GSI 原始 previously 字段（增量数据） */
    QJsonObject previously;

    GameState() = default;
    explicit GameState(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
};

} // namespace GSI

#endif // GAMESTATE_H
