#ifndef ALLPLAYERS_H
#define ALLPLAYERS_H

#include <QString>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include "PlayerModels.h"

namespace GSI {

/**
 * @brief 所有玩家集合（SteamID64 → Player）
 */
struct AllPlayers {
    /** @brief 玩家映射表 */
    QMap<QString, Player> list;

    AllPlayers() = default;
    explicit AllPlayers(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const AllPlayers &other) const;
    bool operator!=(const AllPlayers &other) const { return !(*this == other); }

    /**
     * @brief 根据 SteamID 获取玩家
     * @param steamid SteamID64
     * @return 玩家指针，未找到返回 nullptr
     */
    const Player *getBySteamid(const QString &steamid) const;

    /**
     * @brief 获取所有玩家的 SteamID 列表
     * @return SteamID 列表
     */
    QList<QString> getAllSteamids() const;
};

} // namespace GSI

#endif // ALLPLAYERS_H
