#include "GsiConfigWriter.h"
#include <QFile>
#include <QTextStream>

namespace GSI {

QString GsiConfigWriter::generate(const QString &name, const QString &uri) {
    return QStringLiteral(
        "\"%1\"\n"
        "{\n"
        "  \"uri\"          \"%2\"\n"
        "  \"timeout\"      \"3.0\"\n"
        "  \"buffer\"       \"0.0\"\n"
        "  \"throttle\"     \"0.0\"\n"
        "  \"heartbeat\"    \"30.0\"\n"
        "  \"data\"\n"
        "  {\n"
        "    \"provider\"                  \"1\"\n"
        "    \"map\"                       \"1\"\n"
        "    \"map_round_wins\"            \"1\"\n"
        "    \"round\"                     \"1\"\n"
        "    \"player_id\"                 \"1\"\n"
        "    \"player_state\"              \"1\"\n"
        "    \"player_weapons\"            \"1\"\n"
        "    \"player_match_stats\"        \"1\"\n"
        "    \"player_position\"           \"1\"\n"
        "    \"phase_countdowns\"          \"1\"\n"
        "    \"allplayers_id\"             \"1\"\n"
        "    \"allplayers_state\"          \"1\"\n"
        "    \"allplayers_match_stats\"    \"1\"\n"
        "    \"allplayers_weapons\"        \"1\"\n"
        "    \"allplayers_position\"       \"1\"\n"
        "    \"allgrenades\"               \"1\"\n"
        "    \"bomb\"                      \"1\"\n"
        "    \"auth\"                      \"1\"\n"
        "  }\n"
        "}"
    ).arg(name, uri);
}

bool GsiConfigWriter::writeToFile(const QString &filePath, const QString &name, const QString &uri) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream << generate(name, uri);
    file.close();
    return true;
}

} // namespace GSI
