#ifndef GRENADEMODELS_H
#define GRENADEMODELS_H

#include <QString>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include "GsiEnums.h"
#include "Vector3D.h"

namespace GSI {

/**
 * @brief 手雷基础模型
 */
struct GrenadeBase {
    /** @brief 投掷者 SteamID64 */
    QString owner;
    /** @brief 存在时间 */
    QString lifetime;
    /** @brief 手雷类型 */
    GrenadeType type = GrenadeType::Unknown;
    /** @brief 速度向量 */
    Vector3D velocity;
    /** @brief 位置向量 */
    Vector3D position;
    /** @brief 效果时间 */
    QString effecttime;

    GrenadeBase() = default;
    explicit GrenadeBase(const QJsonObject &obj);
    virtual ~GrenadeBase() = default;
    virtual QJsonObject toJsonObject() const;
    bool operator==(const GrenadeBase &other) const;
    bool operator!=(const GrenadeBase &other) const { return !(*this == other); }
    bool isMoving() const;
    virtual bool isFlameGrenade() const { return false; }
};

/**
 * @brief 燃烧弹模型（包含火焰位置列表）
 */
struct FlameGrenade : public GrenadeBase {
    /** @brief 火焰位置列表 */
    QList<Vector3D> flames;

    FlameGrenade() = default;
    explicit FlameGrenade(const QJsonObject &obj);
    QJsonObject toJsonObject() const override;
    bool isFlameGrenade() const override { return true; }
};

/**
 * @brief 手雷列表，管理所有活动手雷
 */
struct GrenadeList {
    /** @brief 手雷映射表（ID → 手雷对象） */
    QMap<int, QSharedPointer<GrenadeBase>> list;

    GrenadeList() = default;
    explicit GrenadeList(const QJsonObject &obj);
    QJsonObject toJsonObject() const;
    bool operator==(const GrenadeList &other) const;
    bool operator!=(const GrenadeList &other) const { return !(*this == other); }

    /**
     * @brief 根据 ID 获取手雷
     * @param id 手雷 ID
     * @return 手雷指针，未找到返回 nullptr
     */
    QSharedPointer<GrenadeBase> getById(int id) const;

    /**
     * @brief 获取指定类型的所有手雷
     * @param type 手雷类型
     * @return 符合条件的手雷列表
     */
    QList<QSharedPointer<GrenadeBase>> getAllWithType(GrenadeType type) const;
};

} // namespace GSI

#endif // GRENADEMODELS_H
