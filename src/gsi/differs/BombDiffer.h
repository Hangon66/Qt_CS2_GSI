#ifndef BOMBDIFFER_H
#define BOMBDIFFER_H

#include "DifferBase.h"

namespace GSI {

/** @brief 炸弹差异检测 */
class BombDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

} // namespace GSI

#endif // BOMBDIFFER_H
