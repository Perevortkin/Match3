#ifndef MODELLIST_H
#define MODELLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QPointer>

#include "item.h"
#include "gameconfig.h"

class ModelList: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GameConfig* config READ config NOTIFY configChanged)
    Q_PROPERTY(int elementToSwap1 READ elementToSwap1 NOTIFY elementToSwap1Changed)
    Q_PROPERTY(int elementToSwap2 READ elementToSwap2 NOTIFY elementToSwap2Changed)

public:

    ModelList(QObject* pobj = 0);
    ModelList(GameConfig& config, QObject *pobj = 0);
    QVariant data(const QModelIndex& index, int nRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int nRole);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames() const;
    enum Roles { Name = Qt::UserRole + 1, Path, Flag};

    Q_INVOKABLE int swapTwoElements(int from, int to);
    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE void newGame();
    Q_INVOKABLE void hint();

    void addItem(const Item & c);
    void setFirstSearchExecuted(bool firstSearchExecuted);
    bool searchForMatch();
    GameConfig* config();
    int elementToSwap1() const;
    int elementToSwap2() const;

signals:

    void configChanged(GameConfig config);
    void elementToSwap1Changed(int elementToSwap1);
    void elementToSwap2Changed(int elementToSwap2);

private:

    int getIndex(QPointer<Item> item);
    bool searchForPossibleMatch();
    bool verticalSearchMatch();
    bool horizontalSearchMatch();
    void setDataFlag(int index, QVariant color);
    void swapTwoElementsWithoutSearching(int from, int to);
    void remove();
    void removeHorizontalMatch();
    void removeVerticalMatch();

    int m_elementToSwap1;
    int m_elementToSwap2;
    bool m_firstSearchExecuted;
    GameConfig m_config;
    QList<Item> m_list;
    QVector <QVector<QPointer<Item> > > m_removeVerticalMatch;
    QVector <QVector<QPointer<Item> > > m_removeHorizontalMatch;


};

#endif // MODELLIST_H
