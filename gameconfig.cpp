#include "gameconfig.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

GameConfig::GameConfig():QObject(0) {
    m_isVictory = false;
    m_moves = 0;
    m_score = 0;
    QFile file("../match3/config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() <<"Doesn`t opened";
        m_columns = 10;
        m_rows = 10;
        m_elementScore = 60;
        m_minScore = 10000;
        m_maxMoves = 20;
        for(int i = 0; i < 5; i++) {
            m_types.push_back(i);
        }
    }
    QByteArray rawData = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(rawData));
    QJsonObject json = doc.object();
    QJsonArray q = json["types"].toArray();
    m_columns = json["width"].toInt();
    m_rows = json["height"].toInt();
    m_elementScore = json["elementScore"].toInt();
    m_minScore = json["minScore"].toInt();
    m_maxMoves = json["maxMoves"].toInt();
    for(int i = 0; i < q.size(); i++) {
        m_types.push_back(q[i].toInt());
    }
}

GameConfig::GameConfig(const GameConfig & config):QObject(0) {
    m_columns = config.columns();
    m_rows = config.rows();
    m_elementScore = config.elementScore();
    m_minScore = config.minScore();
    m_maxMoves = config.maxMoves();
    m_types = config.types();
    m_score = config.score();
    m_moves = config.moves();
}

int GameConfig::columns() const {
    return m_columns;
}

int GameConfig::rows() const {
    return m_rows;
}

int GameConfig::elementScore() const {
    return m_elementScore;
}

int GameConfig::minScore() const {
    return m_minScore;
}

int GameConfig::maxMoves() const {
    return m_maxMoves;
}

void GameConfig::setcolumns(int columns) {
    if (m_columns == columns)
        return;
    m_columns = columns;
    emit columnsChanged(columns);
}

void GameConfig::setRows(int rows) {
    m_rows = rows;
    emit rowsChanged(rows);
}

void GameConfig::setElementScore(int elementScore) {
    m_elementScore = elementScore;
    emit elementScoreChanged(elementScore);
}

void GameConfig::setMinScore(int minScore) {
    m_minScore = minScore;
    emit minScoreChanged(minScore);
}

void GameConfig::setMaxMoves(int maxMoves) {
    m_maxMoves = maxMoves;
    emit maxMovesChanged(maxMoves);
}

void GameConfig::setMoves(int moves) {
    m_moves = moves;
    emit movesChanged(moves);
}

void GameConfig::setScore(int score) {
     static int someScore = 0;
    if (!score) {
        someScore = 0;
    }
    someScore += (score * m_elementScore);
    m_score = someScore;
    emit scoreChanged(someScore);
}

void GameConfig::setIsVictory(bool isVictory) {
    m_isVictory = isVictory;
    emit isVictoryChanged(isVictory);
}

QVector<int> GameConfig::types() const {
    return m_types;
}

void GameConfig::setTypes(const QVector<int> &types) {
    m_types = types;
}

GameConfig &GameConfig::operator =(const GameConfig & config) {
    m_columns = config.columns();
    m_rows = config.rows();
    m_elementScore = config.elementScore();
    m_minScore = config.minScore();
    m_maxMoves = config.maxMoves();
    m_types = config.types();
    m_score = config.score();
    m_moves = config.moves();
    return *this;
}

bool GameConfig::isVictory() const {
    return m_isVictory;
}

int GameConfig::moves() const {
    return m_moves;
}

int GameConfig::score()const {
    return m_score;
}

void GameConfig::isLevelCompleted() {
    if (m_score >= m_minScore && m_moves <= m_maxMoves) {
        setIsVictory(true);
    }
}


