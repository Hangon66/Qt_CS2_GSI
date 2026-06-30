#include "GrenadesDiffer.h"
#include "GameState.h"
#include "GsiService.h"
#include "GrenadeModels.h"

namespace GSI {

void GrenadesDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &prevList = prev.grenades.list;
    const auto &currList = curr.grenades.list;

    // 检查新增的手雷
    for (auto it = currList.begin(); it != currList.end(); ++it) {
        if (!prevList.contains(it.key())) {
            emit e->grenadeThrown(it.key(), it.value());
        }
    }

    // 检查消失的手雷
    for (auto it = prevList.begin(); it != prevList.end(); ++it) {
        if (!currList.contains(it.key())) {
            emit e->grenadeGone(it.key(), it.value());
        }
    }

    // 检查已有手雷的属性变化
    for (auto it = currList.begin(); it != currList.end(); ++it) {
        auto prevIt = prevList.find(it.key());
        if (prevIt == prevList.end()) continue;

        const auto &pg = prevIt.value();
        const auto &cg = it.value();

        if (pg->position != cg->position)
            emit e->grenadePositionChanged(it.key(), pg->position, cg->position);
        if (pg->velocity != cg->velocity)
            emit e->grenadeVelocityChanged(it.key(), pg->velocity, cg->velocity);
        if (pg->lifetime != cg->lifetime)
            emit e->grenadeLifetimeChanged(it.key(), pg->lifetime, cg->lifetime);
        if (pg->effecttime != cg->effecttime)
            emit e->grenadeEffectTimeChanged(it.key(), pg->effecttime, cg->effecttime);
    }
}

} // namespace GSI
