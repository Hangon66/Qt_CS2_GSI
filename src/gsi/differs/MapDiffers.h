#ifndef MAPDIFFERS_H
#define MAPDIFFERS_H

#include "DifferBase.h"

namespace GSI {

/** @brief 地图信息差异检测 */
class MapDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/** @brief 回合差异检测（阶段变化、开始/结束） */
class RoundDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/** @brief 回合胜利记录差异检测 */
class RoundWinsDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/** @brief 阶段倒计时差异检测 */
class PhaseCountdownsDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

/** @brief Provider 差异检测 */
class ProviderDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

} // namespace GSI

#endif // MAPDIFFERS_H
