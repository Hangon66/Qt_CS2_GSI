#ifndef BOMBMODEL_H
#define BOMBMODEL_H

#include <QString>
#include <QJsonObject>
#include "GsiEnums.h"
#include "Vector3D.h"

namespace GSI {

/**
 * @brief 炸弹数据模型
 */
struct Bomb {
    /** @brief 炸弹状态 */
    BombState state = BombState::Unknown;
    /** @brief 炸弹位置 */
    Vector3D position;
    /** @brief 持有/安放炸弹的玩家 SteamID64 */
    QString player;

    Bomb() = default;
    explicit Bomb(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const Bomb &other) const;
    bool operator!=(const Bomb &other) const { return !(*this == other); }
};

} // namespace GSI

#endif // BOMBMODEL_H
