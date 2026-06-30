#include "Vector3D.h"

namespace GSI {

static const QRegularExpression s_vectorRegex(
    QStringLiteral("([+-]?\\d*\\.?\\d+),\\s*([+-]?\\d*\\.?\\d+),\\s*([+-]?\\d*\\.?\\d+)")
);

Vector3D Vector3D::fromString(const QString &str)
{
    if (str.isEmpty())
        return Vector3D();

    const auto match = s_vectorRegex.match(str);
    if (!match.hasMatch())
        return Vector3D();

    return Vector3D(
        match.captured(1).toDouble(),
        match.captured(2).toDouble(),
        match.captured(3).toDouble()
    );
}

Vector3D Vector3D::fromJsonObject(const QJsonObject &obj)
{
    return Vector3D(
        obj.value("x").toDouble(0.0),
        obj.value("y").toDouble(0.0),
        obj.value("z").toDouble(0.0)
    );
}

QString Vector3D::toString() const
{
    return QString::number(x, 'f', 2) + ", " +
           QString::number(y, 'f', 2) + ", " +
           QString::number(z, 'f', 2);
}

QJsonObject Vector3D::toJsonObject() const
{
    QJsonObject obj;
    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;
    return obj;
}

} // namespace GSI
