#include "GsiUpdateHandler.h"
#include "utils/JsonUtils.h"
#include "GsiService.h"
#include <QDebug>

namespace GSI {

GsiUpdateHandler::GsiUpdateHandler(DifferManager &differManager, GsiService *emitter)
    : m_differManager(differManager)
    , m_emitter(emitter)
{}

void GsiUpdateHandler::handle(const QJsonObject &rawUpdate) {
    if (rawUpdate.isEmpty()) {
        qWarning() << "GsiUpdateHandler: Empty GSI payload received.";
        return;
    }

    // 发射原始更新信号
    if (m_emitter) {
        emit m_emitter->rawUpdate(rawUpdate);
    }

    try {
        // 1. 获取上次完整状态的 JSON 表示
        QJsonObject previousObj = m_lastState.toJsonObject();

        // 2. 构建合并后的完整 JSON（将 previously 增量应用到 rawUpdate）
        QJsonObject mergedUpdate = rawUpdate;
        QJsonObject previouslyData = rawUpdate.value("previously").toObject();
        if (!previouslyData.isEmpty()) {
            JsonUtils::applyDelta(mergedUpdate, previouslyData);
        }

        // 3. 构建新的 GameState
        GameState newState(mergedUpdate);

        // 4. 通过 DifferManager 比较差异并发射信号
        m_differManager.diff(m_lastState, newState, m_emitter);

        // 5. 更新最后状态
        m_lastState = newState;

        // 6. 通知包装层（如 LocalPlayerService）状态已更新
        if (m_emitter) {
            emit m_emitter->stateUpdated();
        }
    } catch (...) {
        qWarning() << "GsiUpdateHandler: Error processing GSI update.";
    }
}

} // namespace GSI
