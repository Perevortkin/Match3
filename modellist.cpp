#include "modellist.h"
#include <QDebug>
#include <QTimer>
ModelList::ModelList(QObject *pobj)
    : QAbstractListModel(pobj){

}

QVariant ModelList::data(const QModelIndex &index, int nRole) const {
    if (index.row() < 0 || index.row() > m_list.count()) {
        return QVariant();
    }
    const Item &item = m_list[index.row()];
    if (nRole == Name) {
        return item.getName();
    }
    else if (nRole == Path)
        return item.getIcon();
    else if (nRole ==Flag)
        return item.getFlag();
    return QVariant();

}

bool ModelList::setData(const QModelIndex &index, const QVariant &value, int nRole) {
    if (index.isValid() && nRole == Qt::EditRole) {
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int ModelList::rowCount(const QModelIndex &parent) const {
    return m_list.size();
}

QVariant ModelList::headerData(int nSection, Qt::Orientation orientation, int nRole) const {
    if (nRole != Qt::DisplayRole) {
        return QVariant();
    }
    return (orientation == Qt::Horizontal) ? QString("Number"): QString::number(nSection);
}

Qt::ItemFlags ModelList::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QHash<int, QByteArray> ModelList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[Flag] = "flag";
    return roles;
}

void ModelList::addItem(const Item &c) {

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_list << c;
    endInsertRows();
}

bool ModelList::searchForMatch() {
    int rowCount = 8;
    int colCount = 8;
    int matches = 1;
    bool isMatch = false;
    //Horizontal search
    for (int i = 0; i < rowCount * colCount; i += rowCount) {
        matches = 1;
        for (int j = i; j < i + rowCount - 1; j++) {
            if (m_list[j].getName() == m_list[j + 1].getName()) {
                matches++;
            }
            else if (matches == 2) {
                matches --;
            }
            else if (matches >= 3) {
                int k = j;
                isMatch = true;
                qDebug()<< "hor matches :"<< matches;
                while (matches != 0) {
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k--;
                    matches--;
                }
                matches = 1;
            }

            if (j + 1 == i + rowCount - 1 && matches >= 3) {
                int k = j + 1;
                isMatch = true;
                qDebug()<< "hor matches :"<< matches;
                while (matches != 0) {
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k--;
                    matches--;
                }
                matches = 1;
            }
        }
    }

    //Vertical search
    for (int i =  0; i < colCount; i++) {
        matches = 1;
        for (int j = i; j < (colCount - 1) * rowCount + i; j += colCount) {
            if (m_list[j].getName() == m_list[j + colCount].getName()) {
                matches++;
            }
            else if (matches == 2) {
                matches --;
            }
            else if (matches >= 3) {
                isMatch = true;
                int k = j;
                qDebug()<< "ver matches :"<< matches;
                while (matches != 0) {
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k -= colCount;
                    matches--;
                }
                matches = 1;
            }
            if (j + colCount == (colCount - 1) * rowCount + i && matches >= 3) {
                isMatch = true;
                int k = j += colCount;
                qDebug()<< "ver matches :"<< matches;
                while (matches != 0) {
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k -= colCount;
                    matches--;
                }
                matches = 1;
            }

        }
    }


    if (isMatch) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                [=]()
        {
            removeItems();
        });
        timer->setSingleShot(true);
        timer->start(500);
    }

    return isMatch;
}

void ModelList::swapTwoElementsWithoutSearching(int from, int to) {
    if (from + 8 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to -1, to -1, QModelIndex(), to - 8);
        endMoveRows();
    }
    if (from - 8 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to + 1, to + 1, QModelIndex(), to + 9);
        endMoveRows();
    }
    if ( from - 1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        beginMoveRows(QModelIndex(), to, to, QModelIndex(), from + 1);
        endMoveRows();
        endMoveRows();
    }
    if ( from + 1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        beginMoveRows(QModelIndex(), to, to, QModelIndex(), from);
        endMoveRows();
        endMoveRows();
    }
}
void ModelList::swapTwoElements(int from, int to) {

    if (from + 8 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to -1, to -1, QModelIndex(), to - 8);
        endMoveRows();
    }
    if (from - 8 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to + 1, to + 1, QModelIndex(), to + 9);
        endMoveRows();
    }
    if ( from - 1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
    }
    if ( from +1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
    }
    if (!searchForMatch()) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                [=]()
        {
            swapTwoElementsWithoutSearching(to, from);
        });
        timer->setSingleShot(true);
        timer->start(500);
    }
}

void ModelList::removeItems()
{

    for (int i = 0; i < 64; i++) {
        if (m_list[i].getFlag()) {
            beginRemoveRows(QModelIndex(),i,i);
            while (i + 8 < 64 - 8) {
                swapTwoElementsWithoutSearching(i + 8, i);
                i += 8;
            }
            endRemoveRows();
        }
    }
}

