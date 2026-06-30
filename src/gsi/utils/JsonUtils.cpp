#include "JsonUtils.h"

namespace GSI {

void JsonUtils::applyDelta(QJsonObject &target, const QJsonObject &source) {
    if (source.isEmpty()) return;
    deepMerge(target, source);
}

void JsonUtils::deepMerge(QJsonObject &target, const QJsonObject &source) {
    for (auto it = source.begin(); it != source.end(); ++it) {
        const QString &key = it.key();
        const QJsonValue &sourceValue = it.value();

        if (sourceValue.isObject() && target.value(key).isObject()) {
            // 两个都是对象，递归合并
            QJsonObject targetObj = target.value(key).toObject();
            QJsonObject sourceObj = sourceValue.toObject();
            deepMerge(targetObj, sourceObj);
            target[key] = targetObj;
        } else {
            // 叶子节点或类型不匹配，用 source 覆盖
            target[key] = sourceValue;
        }
    }
}

} // namespace GSI
