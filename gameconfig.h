#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include <QObject>
#include <QVector>
#include <QDebug>
class GameConfig: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int columns READ columns WRITE setcolumns NOTIFY columnsChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int  elementScore READ elementScore WRITE setElementScore NOTIFY elementScoreChanged)
    Q_PROPERTY(int minScore READ minScore WRITE setMinScore NOTIFY minScoreChanged)
    Q_PROPERTY(int maxMoves READ maxMoves WRITE setMaxMoves NOTIFY maxMovesChanged)
public:
    GameConfig();
    GameConfig(const GameConfig &);
    int columns() const;
    int rows() const;
    int elementScore() const;
    int minScore() const;
    int maxMoves() const;

    QVector<int> types() const;
    void setTypes(const QVector<int> &types);
    GameConfig & operator =(const GameConfig &);

public slots:
    void setcolumns(int columns);
    void setRows(int rows);
    void setElementScore(int elementScore);
    void setMinScore(int minScore);
    void setMaxMoves(int maxMoves);

signals:
    void columnsChanged(int columns);
    void rowsChanged(int rows);
    void elementScoreChanged(int elementScore);
    void minScoreChanged(int minScore);
    void maxMovesChanged(int maxMoves);

private:
    int m_columns;
    int m_rows;
    int m_elementScore;
    int m_minScore;
    int m_maxMoves;
    QVector<int> m_types;
};

#endif // GAMECONFIG_H
