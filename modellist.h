#ifndef MODELLIST_H
#define MODELLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QPointer>

#include "item.h"
#include "gameconfig.h"

class ModelList: public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(GameConfig* config READ config NOTIFY configChanged)
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
    void addItem(const Item &c);
    Q_INVOKABLE int swapTwoElements(int from, int to);
    void remove();
    void removeHorizontalMatch();
    void removeVerticalMatch();
    bool searchForMatch();
    bool verticalSearchMatch();
    bool horizontalSearchMatch();
    int getIndex(QPointer<Item> item);
    GameConfig* config();
    void swapTwoElementsWithoutSearching(int from, int to);
    void setFirstSearchExecuted(bool firstSearchExecuted);
    void setDataFlag(int index, QVariant color);
    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE void newGame();
    void refresh(int index);

signals:
    void configChanged(GameConfig config);

private:
    bool m_firstSearchExecuted;
    GameConfig m_config;
    QList<Item> m_list;
    QVector <QVector<QPointer<Item> > > m_removeVerticalMatch;
    QVector <QVector<QPointer<Item> > > m_removeHorizontalMatch;
};

#endif // MODELLIST_H
