#include "BombModel.h"

namespace GSI {

Bomb::Bomb(const QJsonObject &obj)
    : state(stringToBombState(obj.value("state").toString()))
    , position(Vector3D::fromString(obj.value("position").toString()))
    , player(isSteamId64(obj.value("player").toString()) ? obj.value("player").toString() : QStringLiteral("unknown"))
{}

QJsonObject Bomb::toJsonObject() const {
    QJsonObject o;
    o["state"] = bombStateToString(state);
    o["position"] = position.toString();
    o["player"] = player;
    return o;
}

bool Bomb::operator==(const Bomb &other) const {
    return state == other.state && position == other.position && player == other.player;
}

} // namespace GSI
