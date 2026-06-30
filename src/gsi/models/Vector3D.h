#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <QString>
#include <QJsonObject>
#include <QRegularExpression>

namespace GSI {

/**
 * @brief 三维向量，用于表示游戏中的位置和方向
 */
struct Vector3D {
    /** @brief X 坐标 */
    double x = 0.0;
    /** @brief Y 坐标 */
    double y = 0.0;
    /** @brief Z 坐标 */
    double z = 0.0;

    Vector3D() = default;
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    /**
     * @brief 从 GSI 字符串解析向量
     * @param str 格式如 "1.0, 2.0, 3.0"
     * @return 解析后的 Vector3D
     */
    static Vector3D fromString(const QString &str);

    /**
     * @brief 从 QJsonObject 构建
     * @param obj JSON 对象，包含 x, y, z 字段
     * @return Vector3D 实例
     */
    static Vector3D fromJsonObject(const QJsonObject &obj);

    /**
     * @brief 转换为字符串表示
     * @return 格式如 "1.00, 2.00, 3.00"
     */
    QString toString() const;

    /**
     * @brief 序列化为 JSON 对象
     * @return QJsonObject
     */
    QJsonObject toJsonObject() const;

    bool operator==(const Vector3D &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3D &other) const {
        return !(*this == other);
    }
};

} // namespace GSI

#endif // VECTOR3D_H
