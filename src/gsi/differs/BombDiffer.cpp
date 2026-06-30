#include "BombDiffer.h"
#include "GameState.h"
#include "GsiService.h"

namespace GSI {

void BombDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pb = prev.bomb;
    const auto &cb = curr.bomb;

    if (pb.state != cb.state)       emit e->bombStateChanged(pb.state, cb.state);
    if (pb.position != cb.position) emit e->bombPositionChanged(pb.position, cb.position);
    if (pb.player != cb.player)     emit e->bombPlayerChanged(pb.player, cb.player);
}

} // namespace GSI
