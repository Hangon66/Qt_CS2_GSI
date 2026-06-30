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

    // Team Info 补全 diff
    if (pm.teamCT.name != cm.teamCT.name)
        emit e->teamNameChanged(Team::CT, pm.teamCT.name, cm.teamCT.name);
    if (pm.teamT.name != cm.teamT.name)
        emit e->teamNameChanged(Team::T, pm.teamT.name, cm.teamT.name);
    if (pm.teamCT.consecutiveRoundLosses != cm.teamCT.consecutiveRoundLosses)
        emit e->teamConsecutiveRoundLossesChanged(Team::CT, pm.teamCT.consecutiveRoundLosses, cm.teamCT.consecutiveRoundLosses);
    if (pm.teamT.consecutiveRoundLosses != cm.teamT.consecutiveRoundLosses)
        emit e->teamConsecutiveRoundLossesChanged(Team::T, pm.teamT.consecutiveRoundLosses, cm.teamT.consecutiveRoundLosses);

    // 语义推理
    if (pm.mode != cm.mode)
        emit e->gameModeChanged(pm.mode, cm.mode);

    if (pm.teamCT.timeoutsRemaining != cm.teamCT.timeoutsRemaining)
        emit e->teamTimeoutsRemainingChanged(Team::CT, pm.teamCT.timeoutsRemaining, cm.teamCT.timeoutsRemaining);
    if (pm.teamT.timeoutsRemaining != cm.teamT.timeoutsRemaining)
        emit e->teamTimeoutsRemainingChanged(Team::T, pm.teamT.timeoutsRemaining, cm.teamT.timeoutsRemaining);

    if (pm.teamCT.matchesWonThisSeries != cm.teamCT.matchesWonThisSeries)
        emit e->teamMatchesWonChanged(Team::CT, pm.teamCT.matchesWonThisSeries, cm.teamCT.matchesWonThisSeries);
    if (pm.teamT.matchesWonThisSeries != cm.teamT.matchesWonThisSeries)
        emit e->teamMatchesWonChanged(Team::T, pm.teamT.matchesWonThisSeries, cm.teamT.matchesWonThisSeries);

    // MapPhase 语义事件
    if (pm.phase != cm.phase) {
        if (cm.phase == MapPhase::Warmup)
            emit e->warmupStarted();
        if (pm.phase == MapPhase::Warmup && cm.phase != MapPhase::Warmup)
            emit e->warmupOver();
        if (cm.phase == MapPhase::Intermission)
            emit e->intermissionStarted();
        if (pm.phase == MapPhase::Intermission && cm.phase != MapPhase::Intermission)
            emit e->intermissionOver();
        if (pm.phase == MapPhase::Warmup && cm.phase == MapPhase::Live)
            emit e->matchStarted();
        if (cm.phase == MapPhase::GameOver)
            emit e->gameOver();
    }
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
        // 语义推理：回合结论
        auto cond = curr.map.roundWins.list.value(curr.map.round, RoundWinCondition::Unknown);
        emit e->roundConcluded(cr.winner, cond);
    }

    if (pr.bomb != cr.bomb) {
        emit e->roundBombChanged(pr.bomb, cr.bomb);
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

    if (pp.phase != cp.phase) {
        emit e->phaseCountdownsPhaseChanged(pp.phase, cp.phase);
        // 语义推理
        if (cp.phase == CountdownPhase::Freezetime)
            emit e->freezetimeStarted();
        else if (pp.phase == CountdownPhase::Freezetime)
            emit e->freezetimeOver();
        if (cp.phase == CountdownPhase::Paused)
            emit e->pauseStarted();
        else if (pp.phase == CountdownPhase::Paused)
            emit e->pauseOver();
        if (cp.phase == CountdownPhase::Timeout)
            emit e->timeoutStarted();
        else if (pp.phase == CountdownPhase::Timeout)
            emit e->timeoutOver();
        // 炸弹/拆弹倒计时语义事件
        if (cp.phase == CountdownPhase::Bomb)
            emit e->bombCountdownStarted();
        else if (pp.phase == CountdownPhase::Bomb)
            emit e->bombCountdownOver();
        if (cp.phase == CountdownPhase::Defusing)
            emit e->defuseCountdownStarted();
        else if (pp.phase == CountdownPhase::Defusing)
            emit e->defuseCountdownOver();
    }
    if (pp.phaseEndsIn != cp.phaseEndsIn) emit e->phaseCountdownsTimeChanged(pp.phaseEndsIn, cp.phaseEndsIn);
}

// --- ProviderDiffer ---
void ProviderDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pp = prev.provider;
    const auto &cp = curr.provider;

    if (pp.name != cp.name)           emit e->providerNameChanged(pp.name, cp.name);
    if (pp.timestamp != cp.timestamp) emit e->providerTimestampChanged(pp.timestamp, cp.timestamp);
}

// --- AuthDiffer ---
void AuthDiffer::diff(const GameState &prev, const GameState &curr, GsiService *e) {
    const auto &pa = prev.auth;
    const auto &ca = curr.auth;

    if (pa.token != ca.token) emit e->authTokenChanged(pa.token, ca.token);
    if (pa.ip != ca.ip)       emit e->authIpChanged(pa.ip, ca.ip);
}

} // namespace GSI
