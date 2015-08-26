#include "modellist.h"
#include <QDebug>
#include <QTimer>
ModelList::ModelList(QObject *pobj)
    : QAbstractListModel(pobj){

}

ModelList::ModelList(GameConfig & config, QObject *pobj)
    :QAbstractListModel(pobj) {
    m_config = config;
}

QVariant ModelList::data(const QModelIndex &index, int nRole) const {
    if (index.row() < 0 || index.row() > m_list.count()) {
        return QVariant();
    }
    Item item = m_list[index.row()];
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
    int rowCount = m_config.rows();
    int colCount = m_config.columns();
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
                QVector<int> match;
                while (matches != 0) {
                    match.push_front(k);
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k--;
                    matches--;
                }
                matches = 1;
                removeHorizontalMatch.push_back(match);
            }

            if (j + 1 == i + rowCount - 1 && matches >= 3) {
                int k = j + 1;
                isMatch = true;
                QVector<int> match;
                qDebug()<< "hor matches :"<< matches;
                while (matches != 0) {
                    match.push_front(k);
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k--;
                    matches--;
                }
                matches = 1;
                removeHorizontalMatch.push_back(match);
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
                QVector<int> match;
                while (matches != 0) {
                        match.push_front(k);
                        m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k -= colCount;
                    matches--;
                }
                removeVerticalMatch.push_back(match);
                matches = 1;
            }
            if (j + colCount == (colCount - 1) * rowCount + i && matches >= 3) {
                isMatch = true;
                int k = j += colCount;
                qDebug()<< "ver matches :"<< matches;
                QVector<int> match;
                while (matches != 0) {
                    match.push_front(k);
                    m_list[k].setFlag(true);
                    qDebug() <<" index :" << k;
                    k -= colCount;
                    matches--;
                }
                removeVerticalMatch.push_back(match);
                matches = 1;
            }

        }
    }


    if (isMatch) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                [=]()
        {
            remove();
        });
        timer->setSingleShot(true);
        timer->start(500);
    }

    return isMatch;
}

void ModelList::swapTwoElementsWithoutSearching(int from, int to) {

    int columns = m_config.columns();

    if (from + columns == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to -1, to -1, QModelIndex(), to - columns);
        endMoveRows();
    }
    if (from - columns == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
        beginMoveRows(QModelIndex(), to + 1, to + 1, QModelIndex(), to + columns + 1);
        endMoveRows();
    }
    if ( from - 1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();

    }
    if ( from + 1 == to) {
        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
        qSwap(m_list[from], m_list[to]);
        endMoveRows();
    }
}
void ModelList::swapTwoElements(int from, int to) {

    int columns = m_config.columns();

    if ( from + columns == to || from - columns == to || from + 1 == to || from - 1 == to) {
        swapTwoElementsWithoutSearching(from, to);
        if (!searchForMatch()) {
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout,
                    [=]() {
                swapTwoElementsWithoutSearching(to, from); });
            timer->setSingleShot(true);
            timer->start(500);
        }
    }
}

void ModelList::remove() {
    int rows = m_config.rows();
    int columns = m_config.columns();
    int index;
    int k = 0;

    //if item is in horizontal and vertical match

    //remove Horizontal Match
    for (int i = 0; i < removeHorizontalMatch.size(); i++) {
        for (int j = 0; j < removeHorizontalMatch[i].size(); j++) {
            index = removeHorizontalMatch[i][j];
            while (  k < columns ) {
                swapTwoElementsWithoutSearching(index,index + columns);
                k++;
                index += columns;
            }

            beginInsertRows(QModelIndex(), index, index);
            m_list.insert(index, Item());
            endInsertRows();
            beginRemoveRows(QModelIndex(), index + 1, index + 1 );
            m_list.removeAt(index + 1);
            endRemoveRows();
            k = 0;
        }
    }
    k = 0;

    //remove Vertical Match
    for (int i = 0; i < removeVerticalMatch.size(); i++) {
        for (int j = 0; j < removeVerticalMatch[i].size(); j++) {
            index = removeVerticalMatch[i][0];
            //if element doesn`t removed yet
            if (m_list[index].getFlag()) {
            while ( k < rows) {
                swapTwoElementsWithoutSearching(index, index + columns);
                k++;
                index += columns;
            }
            beginInsertRows(QModelIndex(), index, index);
            m_list.insert(index, Item());
            endInsertRows();
            beginRemoveRows(QModelIndex(), index + 1, index + 1 );
            m_list.removeAt(index + 1);
            endRemoveRows();
            k = 0;
        }
    }
    }

    removeVerticalMatch.clear();
    removeHorizontalMatch.clear();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [=]() {
        searchForMatch(); });
    timer->setSingleShot(true);
    timer->start(700);
}

int ModelList::getName(int index)
{
    return m_list[index].getName();
}

void ModelList::removeItems()
{

    for (int i = 0; i < 64; i++) {
        if (m_list[i].getFlag()) {
            beginRemoveColumns(QModelIndex(),i,i);
            //  beginRemoveRows(QModelIndex(),i,64);
            int j = i;
            m_list.removeAt(i);

            qDebug() << "remove :" << i;
            //            while (j + 8 < 56  ) {
            swapTwoElementsWithoutSearching(j + 8, j);
            //                j += 8;
            //            }
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_list.insert(j,Item());
            endInsertRows();
            //   endRemoveRows();
            endRemoveColumns();
            break;
        }

    }
}

