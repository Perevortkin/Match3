#ifndef MODELLIST_H
#define MODELLIST_H

#include <QObject>
#include <QAbstractListModel>
#include "item.h"


class ModelList: public QAbstractListModel
{
    Q_OBJECT
public:
    ModelList(QObject* pobj = 0);
    QVariant data(const QModelIndex& index, int nRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int nRole);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames() const;

    enum Roles { Name = Qt::UserRole + 1, Path, Flag};

    void addItem(const Item &c);
    bool searchForMatch();
    Q_INVOKABLE void swapTwoElements(int from, int to);

    void removeItems();

public slots:
    void swapTwoElementsWithoutSearching(int from, int to);
private:
    QList<Item> m_list;
};

#endif // MODELLIST_H
