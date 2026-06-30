#include "MapDiffers.h"
#include "GameState.h"
#include "GsiService.h"

namespace GSI {

// --- MapDiffer ---
void MapDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pm = prev.map;
    const auto &cm = curr.map;

    if (pm.name != cm.name)                   emit e->mapNameChanged(pm.name, cm.name);
    if (pm.phase != cm.phase)                 emit e->mapPhaseChanged(pm.phase, cm.phase);
    if (pm.round != cm.round)                 emit e->mapRoundChanged(pm.round, cm.round);
    if (pm.teamCT.score != cm.teamCT.score)   emit e->mapTeamCTScoreChanged(pm.teamCT.score, cm.teamCT.score);
    if (pm.teamT.score != cm.teamT.score)     emit e->mapTeamTScoreChanged(pm.teamT.score, cm.teamT.score);
    if (pm.currentSpectators != cm.currentSpectators) emit e->mapSpectatorsChanged(pm.currentSpectators, cm.currentSpectators);
    if (pm.souvenirsTotal != cm.souvenirsTotal)       emit e->mapSouvenirsChanged(pm.souvenirsTotal, cm.souvenirsTotal);
}

// --- RoundDiffer ---
void RoundDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pr = prev.round;
    const auto &cr = curr.round;

    if (pr.phase != cr.phase) {
        emit e->roundPhaseChanged(pr.phase, cr.phase);
        if (cr.phase == RoundPhase::Freezetime) {
            emit e->roundStarted();
        }
        if (pr.phase == RoundPhase::Live && cr.phase == RoundPhase::Over) {
            emit e->roundEnded(cr.winner);
        }
    }

    if (pr.winner != cr.winner && cr.winner != Team::Unknown) {
        emit e->roundWon(pr.winner, cr.winner);
        emit e->roundEnded(cr.winner);
    }
}

// --- RoundWinsDiffer ---
void RoundWinsDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &prw = prev.map.roundWins.list;
    const auto &crw = curr.map.roundWins.list;

    if (prw.size() != crw.size()) {
        // 新增回合胜利记录
        for (auto it = crw.begin(); it != crw.end(); ++it) {
            if (!prw.contains(it.key())) {
                emit e->roundWinAdded(it.key(), it.value());
            }
        }
    }

    if (prev.map.roundWins != curr.map.roundWins) {
        emit e->roundWinsChanged(prev.map.roundWins, curr.map.roundWins);
    }
}

// --- PhaseCountdownsDiffer ---
void PhaseCountdownsDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pp = prev.phaseCountdowns;
    const auto &cp = curr.phaseCountdowns;

    if (pp.phase != cp.phase)           emit e->phaseCountdownsPhaseChanged(pp.phase, cp.phase);
    if (pp.phaseEndsIn != cp.phaseEndsIn) emit e->phaseCountdownsTimeChanged(pp.phaseEndsIn, cp.phaseEndsIn);
}

// --- ProviderDiffer ---
void ProviderDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pp = prev.provider;
    const auto &cp = curr.provider;

    if (pp.name != cp.name)           emit e->providerNameChanged(pp.name, cp.name);
    if (pp.timestamp != cp.timestamp) emit e->providerTimestampChanged(pp.timestamp, cp.timestamp);
}

} // namespace GSI
