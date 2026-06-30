#include "DifferManager.h"
#include "GameState.h"
#include "GsiService.h"

namespace GSI {

void DifferManager::registerDiffer(std::unique_ptr<DifferBase> differ) {
    m_differs.push_back(std::move(differ));
}

void DifferManager::diff(const GameState &prev, const GameState &curr, GsiService *emitter) {
    for (auto &differ : m_differs) {
        differ->diff(prev, curr, emitter);
    }
}

} // namespace GSI
