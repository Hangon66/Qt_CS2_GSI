#include "BombDiffer.h"
#include "GameState.h"
#include "GsiService.h"

namespace GSI {

void BombDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pb = prev.bomb;
    const auto &cb = curr.bomb;

    if (pb.state != cb.state) {
        emit e->bombStateChanged(pb.state, cb.state);
        // 语义推理：根据状态转换发射对应事件
        switch (cb.state) {
            case BombState::Planting:  emit e->bombPlanting(cb.player); break;
            case BombState::Planted:   emit e->bombPlanted(cb.player); break;
            case BombState::Defusing:  emit e->bombDefusing(cb.player); break;
            case BombState::Defused:   emit e->bombDefused(); break;
            case BombState::Dropped:   emit e->bombDropped(cb.player); break;
            case BombState::Exploded:  emit e->bombExploded(); break;
            case BombState::Carried:
                if (pb.state == BombState::Dropped) emit e->bombPickedUp(cb.player);
                break;
            default: break;
        }
    }
    if (pb.position != cb.position) emit e->bombPositionChanged(pb.position, cb.position);
    if (pb.player != cb.player)     emit e->bombPlayerChanged(pb.player, cb.player);
}

} // namespace GSI
