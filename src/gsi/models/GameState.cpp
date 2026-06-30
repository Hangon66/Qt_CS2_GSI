#include "GameState.h"

namespace GSI {

GameState::GameState(const QJsonObject &obj)
    : auth(obj.value("auth").toObject())
    , provider(obj.value("provider").toObject())
    , map(obj.value("map").toObject())
    , round(obj.value("round").toObject())
    , player(obj.value("player").toObject())
    , phaseCountdowns(obj.value("phase_countdowns").toObject())
    , allPlayers(obj.value("allplayers").toObject())
    , bomb(obj.value("bomb").toObject())
    , grenades(obj.value("grenades").toObject())
    , previously(obj.value("previously").toObject())
{}

QJsonObject GameState::toJsonObject() const {
    QJsonObject o;
    o["auth"] = auth.toJsonObject();
    o["provider"] = provider.toJsonObject();
    o["map"] = map.toJsonObject();
    o["round"] = round.toJsonObject();
    o["player"] = player.toJsonObject();
    o["phase_countdowns"] = phaseCountdowns.toJsonObject();
    o["allplayers"] = allPlayers.toJsonObject();
    o["bomb"] = bomb.toJsonObject();
    o["grenades"] = grenades.toJsonObject();
    return o;
}

} // namespace GSI
