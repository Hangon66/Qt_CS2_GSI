#include "GrenadeModels.h"

namespace GSI {

// --- GrenadeBase ---
GrenadeBase::GrenadeBase(const QJsonObject &obj)
    : owner(isSteamId64(obj.value("owner").toString()) ? obj.value("owner").toString() : QStringLiteral("unknown"))
    , lifetime(obj.value("lifetime").toString())
    , type(stringToGrenadeType(obj.value("type").toString()))
    , velocity(Vector3D::fromString(obj.value("velocity").toString()))
    , position(Vector3D::fromString(obj.value("position").toString()))
    , effecttime(obj.value("effecttime").toString())
{}

QJsonObject GrenadeBase::toJsonObject() const {
    QJsonObject o;
    o["owner"] = owner;
    o["lifetime"] = lifetime;
    o["type"] = grenadeTypeToString(type);
    o["velocity"] = velocity.toString();
    o["position"] = position.toString();
    o["effecttime"] = effecttime;
    return o;
}

bool GrenadeBase::operator==(const GrenadeBase &other) const {
    return owner == other.owner && lifetime == other.lifetime &&
           type == other.type && velocity == other.velocity &&
           position == other.position && effecttime == other.effecttime;
}

bool GrenadeBase::isMoving() const {
    return velocity.x != 0.0 || velocity.y != 0.0 || velocity.z != 0.0;
}

// --- FlameGrenade ---
FlameGrenade::FlameGrenade(const QJsonObject &obj)
    : GrenadeBase(obj)
{
    const QJsonObject flamesObj = obj.value("flames").toObject();
    for (auto it = flamesObj.begin(); it != flamesObj.end(); ++it) {
        flames.append(Vector3D::fromString(it.value().toString()));
    }
}

QJsonObject FlameGrenade::toJsonObject() const {
    QJsonObject o = GrenadeBase::toJsonObject();
    QJsonObject flamesObj;
    for (int i = 0; i < flames.size(); ++i) {
        flamesObj[QStringLiteral("flame_%1").arg(i)] = flames[i].toString();
    }
    o["flames"] = flamesObj;
    return o;
}

// --- GrenadeList ---
GrenadeList::GrenadeList(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        bool ok = false;
        int id = it.key().toInt(&ok);
        if (!ok) continue;

        const QJsonObject grenadeObj = it.value().toObject();
        if (grenadeObj.contains("flames")) {
            list.insert(id, QSharedPointer<GrenadeBase>(new FlameGrenade(grenadeObj)));
        } else {
            list.insert(id, QSharedPointer<GrenadeBase>(new GrenadeBase(grenadeObj)));
        }
    }
}

QJsonObject GrenadeList::toJsonObject() const {
    QJsonObject o;
    for (auto it = list.begin(); it != list.end(); ++it) {
        o[QString::number(it.key())] = it.value()->toJsonObject();
    }
    return o;
}

bool GrenadeList::operator==(const GrenadeList &other) const {
    if (list.size() != other.list.size()) return false;
    for (auto it = list.begin(); it != list.end(); ++it) {
        auto otherIt = other.list.find(it.key());
        if (otherIt == other.list.end()) return false;
        if (*(it.value()) != *(otherIt.value())) return false;
    }
    return true;
}

QSharedPointer<GrenadeBase> GrenadeList::getById(int id) const {
    return list.value(id);
}

QList<QSharedPointer<GrenadeBase>> GrenadeList::getAllWithType(GrenadeType type) const {
    QList<QSharedPointer<GrenadeBase>> result;
    for (const auto &g : list) {
        if (g->type == type) result.append(g);
    }
    return result;
}

} // namespace GSI
