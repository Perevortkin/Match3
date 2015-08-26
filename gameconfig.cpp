#include "gameconfig.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

GameConfig::GameConfig()
{
    QFile file("config.json");
    file.open(QIODevice::ReadOnly);
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

    for(int i = 0; i < q.size(); i++)
    {
       m_types.push_back(q[i].toInt());
    }
    qDebug() << "columns" << m_columns;
    qDebug() << "rows" << m_rows;
    qDebug() << "elementScore" << m_elementScore;
    qDebug() << "minScore" << m_minScore;
    qDebug() << "maxMoves" << m_maxMoves;

}

GameConfig::GameConfig(const GameConfig & config)
{
    m_columns = config.columns();
    m_rows = config.rows();
    m_elementScore = config.elementScore();
    m_minScore = config.minScore();
    m_maxMoves = config.maxMoves();
    m_types = config.types();
}

int GameConfig::columns() const
{
    return m_columns;
}

int GameConfig::rows() const
{
    return m_rows;
}

int GameConfig::elementScore() const
{
    return m_elementScore;
}

int GameConfig::minScore() const
{
    return m_minScore;
}

int GameConfig::maxMoves() const
{
    return m_maxMoves;
}

void GameConfig::setcolumns(int columns)
{
    if (m_columns == columns)
        return;

    m_columns = columns;
    emit columnsChanged(columns);
}



void GameConfig::setRows(int rows)
{
    if (m_rows == rows)
        return;

    m_rows = rows;
    emit rowsChanged(rows);
}

void GameConfig::setElementScore(int elementScore)
{
    if (m_elementScore == elementScore)
        return;

    m_elementScore = elementScore;
    emit elementScoreChanged(elementScore);
}

void GameConfig::setMinScore(int minScore)
{
    if (m_minScore == minScore)
        return;

    m_minScore = minScore;
    emit minScoreChanged(minScore);
}

void GameConfig::setMaxMoves(int maxMoves)
{
    if (m_maxMoves == maxMoves)
        return;

    m_maxMoves = maxMoves;
    emit maxMovesChanged(maxMoves);
}
QVector<int> GameConfig::types() const
{
    return m_types;
}

void GameConfig::setTypes(const QVector<int> &types)
{
    m_types = types;
}

GameConfig &GameConfig::operator =(const GameConfig & config) {
    m_columns = config.columns();
    m_rows = config.rows();
    m_elementScore = config.elementScore();
    m_minScore = config.minScore();
    m_maxMoves = config.maxMoves();
    m_types = config.types();
    return *this;
}


