#ifndef PLAYERDIFFERS_H
#define PLAYERDIFFERS_H

#include "DifferBase.h"

namespace GSI {

/**
 * @brief 玩家基本信息差异检测（名字、队伍、活动、位置等）
 */
class PlayerDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/**
 * @brief 玩家状态差异检测（HP、护甲、金钱等）
 * @details 同时检测当前玩家和 allPlayers 中的状态变化
 */
class PlayerStateDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/**
 * @brief 玩家比赛统计差异检测（KDA、得分等）
 */
class PlayerMatchStatsDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/**
 * @brief 武器差异检测（当前武器、弹药等）
 */
class WeaponDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

} // namespace GSI

#endif // PLAYERDIFFERS_H
