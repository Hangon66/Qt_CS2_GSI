#include "AllPlayers.h"

namespace GSI {

AllPlayers::AllPlayers(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        const QString &steamid = it.key();
        if (isSteamId64(steamid)) {
            QJsonObject playerObj = it.value().toObject();
            playerObj["steamid"] = steamid;
            list.insert(steamid, Player(playerObj));
        }
    }
}

QJsonObject AllPlayers::toJsonObject() const {
    QJsonObject o;
    for (auto it = list.begin(); it != list.end(); ++it) {
        o[it.key()] = it.value().toJsonObject();
    }
    return o;
}

bool AllPlayers::operator==(const AllPlayers &other) const {
    return list == other.list;
}

const Player *AllPlayers::getBySteamid(const QString &steamid) const {
    auto it = list.find(steamid);
    return (it != list.end()) ? &it.value() : nullptr;
}

QList<QString> AllPlayers::getAllSteamids() const {
    return list.keys();
}

} // namespace GSI
