#ifndef GRENADEDIFFER_H
#define GRENADEDIFFER_H

#include "DifferBase.h"

namespace GSI {

/** @brief 手雷差异检测（存在、位置、速度等） */
class GrenadesDiffer : public DifferBase {
public:
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter) override;
};

} // namespace GSI

#endif // GRENADEDIFFER_H
