#include "GsiModels.h"

namespace GSI {

// --- Provider ---
Provider::Provider(const QJsonObject &obj)
    : name(obj.value("name").toString())
    , appid(obj.value("appid").toInt())
    , version(obj.value("version").toInt())
    , steamid(obj.value("steamid").toString())
    , timestamp(obj.value("timestamp").toInt())
{}

QJsonObject Provider::toJsonObject() const {
    QJsonObject o;
    o["name"] = name;
    o["appid"] = appid;
    o["version"] = version;
    o["steamid"] = steamid;
    o["timestamp"] = timestamp;
    return o;
}

bool Provider::operator==(const Provider &other) const {
    return name == other.name && appid == other.appid &&
           version == other.version && steamid == other.steamid &&
           timestamp == other.timestamp;
}

// --- Round ---
Round::Round(const QJsonObject &obj)
    : phase(stringToRoundPhase(obj.value("phase").toString()))
    , bomb(obj.contains("bomb") ? stringToStableBombState(obj.value("bomb").toString()) : StableBombState::Unknown)
    , winner(obj.contains("win_team") ? stringToTeam(obj.value("win_team").toString()) : Team::Unknown)
{}

QJsonObject Round::toJsonObject() const {
    QJsonObject o;
    o["phase"] = roundPhaseToString(phase);
    if (bomb != StableBombState::Unknown)
        o["bomb"] = stableBombStateToString(bomb);
    if (winner != Team::Unknown)
        o["win_team"] = teamToString(winner);
    return o;
}

bool Round::operator==(const Round &other) const {
    return phase == other.phase && bomb == other.bomb && winner == other.winner;
}

// --- PhaseCountdowns ---
PhaseCountdowns::PhaseCountdowns(const QJsonObject &obj)
    : phase(stringToCountdownPhase(obj.value("phase").toString()))
    , phaseEndsIn(obj.value("phase_ends_in").toString())
{}

QJsonObject PhaseCountdowns::toJsonObject() const {
    QJsonObject o;
    o["phase"] = countdownPhaseToString(phase);
    if (!phaseEndsIn.isEmpty())
        o["phase_ends_in"] = phaseEndsIn;
    return o;
}

bool PhaseCountdowns::operator==(const PhaseCountdowns &other) const {
    return phase == other.phase && phaseEndsIn == other.phaseEndsIn;
}

// --- TeamInfo ---
TeamInfo::TeamInfo(const QJsonObject &obj)
    : score(obj.value("score").toInt())
    , name(obj.value("name").toString())
    , consecutiveRoundLosses(obj.value("consecutive_round_losses").toInt())
    , timeoutsRemaining(obj.value("timeouts_remaining").toInt())
    , matchesWonThisSeries(obj.value("matches_won_this_series").toInt())
{}

QJsonObject TeamInfo::toJsonObject() const {
    QJsonObject o;
    o["score"] = score;
    o["name"] = name;
    o["consecutive_round_losses"] = consecutiveRoundLosses;
    o["timeouts_remaining"] = timeoutsRemaining;
    o["matches_won_this_series"] = matchesWonThisSeries;
    return o;
}

bool TeamInfo::operator==(const TeamInfo &other) const {
    return score == other.score && name == other.name &&
           consecutiveRoundLosses == other.consecutiveRoundLosses &&
           timeoutsRemaining == other.timeoutsRemaining &&
           matchesWonThisSeries == other.matchesWonThisSeries;
}

// --- RoundWins ---
RoundWins::RoundWins(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        bool ok = false;
        int roundNum = it.key().toInt(&ok);
        if (ok) {
            list.insert(roundNum, stringToRoundWinCondition(it.value().toString()));
        }
    }
}

QJsonObject RoundWins::toJsonObject() const {
    QJsonObject o;
    for (auto it = list.begin(); it != list.end(); ++it) {
        o[QString::number(it.key())] = roundWinConditionToString(it.value());
    }
    return o;
}

bool RoundWins::operator==(const RoundWins &other) const {
    return list == other.list;
}

// --- MapModel ---
MapModel::MapModel(const QJsonObject &obj)
    : mode(stringToGameMode(obj.value("mode").toString()))
    , name(obj.value("name").toString())
    , phase(stringToMapPhase(obj.value("phase").toString()))
    , round(obj.value("round").toInt())
    , numMatchesToWinSeries(obj.value("num_matches_to_win_series").toInt())
    , teamCT(obj.value("team_ct").toObject())
    , teamT(obj.value("team_t").toObject())
    , roundWins(obj.value("round_wins").toObject())
    , currentSpectators(obj.value("current_spectators").toInt())
    , souvenirsTotal(obj.value("souvenirs_total").toInt())
{}

QJsonObject MapModel::toJsonObject() const {
    QJsonObject o;
    o["mode"] = gameModeToString(mode);
    o["name"] = name;
    o["phase"] = mapPhaseToString(phase);
    o["round"] = round;
    o["num_matches_to_win_series"] = numMatchesToWinSeries;
    o["team_ct"] = teamCT.toJsonObject();
    o["team_t"] = teamT.toJsonObject();
    o["round_wins"] = roundWins.toJsonObject();
    o["current_spectators"] = currentSpectators;
    o["souvenirs_total"] = souvenirsTotal;
    return o;
}

bool MapModel::operator==(const MapModel &other) const {
    return mode == other.mode && name == other.name &&
           phase == other.phase && round == other.round &&
           teamCT == other.teamCT && teamT == other.teamT &&
           currentSpectators == other.currentSpectators &&
           souvenirsTotal == other.souvenirsTotal;
}

} // namespace GSI
