#include "modellist.h"
#include <QDebug>
#include <QTimer>

ModelList::ModelList(QObject *pobj)
    : QAbstractListModel(pobj) {

}

ModelList::ModelList(GameConfig & config, QObject *pobj)
    :QAbstractListModel(pobj) {
    m_firstSearchExecuted = false;
    m_config = config;
    m_elementToSwap1 = -1;
    m_elementToSwap2 = -1;
}

QVariant ModelList::data(const QModelIndex &index, int nRole) const {
    if (index.row() < 0 || index.row() > m_list.count()) {
        return QVariant();
    }
    Item item = m_list[index.row()];
    if (nRole == Name) {
        return item.getType();
    }
    else if (nRole == Path) {
        return item.getIcon();
    }
    else if (nRole == Flag) {
        return item.getFlag();
    }
    return QVariant();

}

bool ModelList::setData(const QModelIndex &index, const QVariant &value, int nRole) {
    Item *item = &m_list[index.row()];
    if (nRole == Flag) {
        item->setFlag(value.toBool());
    }
    else if (nRole == Name) {
        item->setType(value.toInt());
    }
    else if (nRole == Path) {
    }
    emit dataChanged(index, index);
    return true;
}

int ModelList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
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
    bool isMatch = false;
    if (horizontalSearchMatch()) {
        isMatch = true;
    }
    if (verticalSearchMatch()) {
        isMatch = true;
    }
    if (!m_firstSearchExecuted && isMatch) {
        remove();
    }
    else if (isMatch) {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                [=]() {
            remove();});
        timer->setSingleShot(true);
        timer->start(500);
    }
    else m_config.isLevelCompleted();
    return isMatch;
}

bool ModelList::verticalSearchMatch() {
    int rowCount = m_config.rows();
    int colCount = m_config.columns();
    int matches = 1;
    bool isMatch = false;
    for (int i =  0; i < colCount; i++) {
        matches = 1;
        for (int j = i; j < (rowCount - 1) * colCount + i; j += colCount) {
            if (m_list[j].getType() == m_list[j + colCount].getType()) {
                matches++;
            }
            else if (matches == 2) {
                matches --;
            }
            else if (matches >= 3) {
                isMatch = true;
                int k = j;
                QVector<QPointer<Item> > match;

                while (matches != 0) {
                    QPointer<Item> p = &m_list[k];
                    match.push_back(p);
                    setDataFlag(k,true);
                    k -= colCount;
                    matches--;
                }
                m_removeVerticalMatch.push_front(match);
                matches = 1;
            }
            if (j + colCount == (rowCount - 1) * colCount + i && matches >= 3) {
                isMatch = true;
                int k = j += colCount;
                QVector<QPointer<Item> > match;
                while (matches != 0) {
                    QPointer<Item> p = &m_list[k];
                    match.push_back(p);
                    setDataFlag(k,true);
                    k -= colCount;
                    matches--;
                }
                m_removeVerticalMatch.push_front(match);
                matches = 1;
            }
        }
    }
    return isMatch;
}

bool ModelList::horizontalSearchMatch() {
    int rowCount = m_config.rows();
    int colCount = m_config.columns();
    int matches = 1;
    bool isMatch = false;
    for (int i = 0; i < rowCount * colCount; i += colCount) {
        matches = 1;
        for (int j = i; j < i + colCount - 1; j++) {
            if (m_list[j].getType() == m_list[j + 1].getType()) {
                matches++;
            }
            else if (matches == 2) {
                matches --;
            }
            else if (matches >= 3) {
                int k = j;
                isMatch = true;
                QVector<QPointer<Item> > match;
                while (matches != 0) {
                    QPointer<Item> p = &m_list[k];
                    match.push_back(p);
                    setDataFlag(k,true);
                    k--;
                    matches--;
                }
                matches = 1;
                m_removeHorizontalMatch.push_front(match);
            }
            if (j + 1 == i + colCount - 1 && matches >= 3) {
                int k = j + 1;
                isMatch = true;
                QVector<QPointer<Item> > match;
                while (matches != 0) {
                    QPointer<Item> p = &m_list[k];
                    match.push_back(p);
                    setDataFlag(k,true);
                    k--;
                    matches--;
                }
                matches = 1;
                m_removeHorizontalMatch.push_front(match);
            }
        }
    }
    return isMatch;
}

int ModelList::getIndex(QPointer<Item> item) {
    int rows = m_config.rows();
    int columns = m_config.columns();

    for (int i = rows * columns; i >= 0; i--) {
        if ( &m_list[i] == item) {
            return i;
        }
    }
    qWarning() << "Index not found";
    return 0;
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

void ModelList::setFirstSearchExecuted(bool firstSearchExecuted) {
    m_firstSearchExecuted = firstSearchExecuted;
}

void ModelList::setDataFlag(int index, QVariant flag) {
    QModelIndex modelIndex = createIndex(index, 0);
    setData(modelIndex, flag, Flag);
}

QVariantMap ModelList::get(int row) {

    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;

    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return res;
}

void ModelList::newGame() {
    m_firstSearchExecuted = false;
    m_config.setMoves(0);
    m_config.setScore(0);

    for (int i = 0; i < m_config.rows() * m_config.columns(); i++) {
        QModelIndex modelIndex = createIndex(i, 0);
        m_list[i].refresh();
        emit dataChanged(modelIndex, modelIndex);
    }
    searchForMatch();
    m_firstSearchExecuted = true;
}

void ModelList::hint() {

    int rows = m_config.rows();
    int columns = m_config.columns();
    int index;

    if (!searchForPossibleMatch()) {
        for (int i = 0; i < rows * columns; i++) {
            index = i;

            if ((index - 3) % columns != columns - 1 && index >= 3) {

                if ( m_list[index - 3].getType() == m_list[index - 2].getType()) {

                    if (m_list[index].getType() == m_list[index - 3].getType()) {
                        m_elementToSwap1 = index;
                        m_elementToSwap2 = index - 1;
                        emit elementToSwap1Changed(index);
                        emit elementToSwap2Changed(index - 1);
                        return;
                    }
                }
            }
            if ((index + 3) % columns != 0 && index + 3 < rows * columns) {

                if (m_list[index + 3].getType() == m_list[index + 2].getType()) {

                    if (m_list[index].getType() == m_list[index + 3].getType()) {
                        m_elementToSwap1 = index;
                        m_elementToSwap2 = index + 1;
                        emit elementToSwap1Changed(index);
                        emit elementToSwap2Changed(index + 1);
                        return;
                    }
                }
            }
            if (index - 3 * columns >= 0) {

                if (m_list[index - 3 * columns].getType() == m_list[index - 2 * columns].getType()) {

                    if (m_list[index].getType() == m_list[index - 3 * columns].getType()) {
                        m_elementToSwap1 = index;
                        m_elementToSwap2 = index - columns;
                        emit elementToSwap1Changed(index);
                        emit elementToSwap2Changed(index - columns);
                        return;
                    }
                }
            }
            if (index + 3 * columns < rows * columns) {

                if (m_list[index + 3 * columns].getType() == m_list[index + 2 * columns].getType()) {

                    if (m_list[index].getType() == m_list[index + 3 * columns].getType()) {
                        m_elementToSwap1 = index;
                        m_elementToSwap2 = index + columns;
                        emit elementToSwap1Changed(index);
                        emit elementToSwap2Changed(index + columns);
                        return;
                    }
                }
            }
        }
    }
}

bool ModelList::searchForPossibleMatch() {

    int rows = m_config.rows();
    int columns = m_config.columns();
    int index;

    bool leftTop = false;
    bool leftBottom = false;
    bool rightTop = false;
    bool rightBottom = false;

    for (int i = 0; i < rows * columns; i++) {
        index = i;
        //If this element aren`t first or last in row
        if ( (index % columns != 0) && (index % columns != columns - 1) ) {

            if (index + columns - 1 < rows * columns ) {

                if (m_list[index].getType() == m_list[index + columns - 1].getType()) {
                    leftTop = true;
                }
            }
            if (index - columns + 1 > 0) {

                if (m_list[index].getType() == m_list[index - columns + 1].getType()) {
                    rightBottom = true;
                }
            }

            if (leftTop || rightBottom) {

                if (index - columns - 1 > 0) {

                    if (m_list[index].getType() == m_list[index - columns - 1].getType()) {
                        leftBottom = true;
                    }
                }
                if (index + columns + 1 < rows * columns) {

                    if (m_list[index].getType() == m_list[index + columns + 1].getType()) {
                        rightTop = true;
                    }
                }
            }
            if (leftTop) {

                if (leftTop && rightTop) {
                    m_elementToSwap1 = index;
                    m_elementToSwap2 = index + columns;
                    emit elementToSwap1Changed(index);
                    emit elementToSwap2Changed(index + columns);
                    return true;
                }
                if (leftTop && leftBottom) {
                    m_elementToSwap1 = index;
                    m_elementToSwap2 = index - 1;
                    emit elementToSwap1Changed(index);
                    emit elementToSwap2Changed(index - 1);
                    return true;
                }
            }
            if (rightBottom) {

                if (rightBottom && rightTop) {
                    m_elementToSwap1 = index;
                    m_elementToSwap2 = index + 1;
                    emit elementToSwap1Changed(index);
                    emit elementToSwap2Changed(index + 1);
                    return true;
                }
                if (rightBottom && leftBottom) {
                    m_elementToSwap1 = index;
                    m_elementToSwap2 = index - columns;
                    emit elementToSwap1Changed(index);
                    emit elementToSwap2Changed(index - columns);
                    return true;
                }
            }
        }

        leftTop = false;
        leftBottom = false;
        rightTop = false;
        rightBottom = false;
    }
    return false;
}

int ModelList::elementToSwap1() const {
    return m_elementToSwap1;
}

int ModelList::elementToSwap2() const {
    return m_elementToSwap2;
}
int ModelList::swapTwoElements(int from, int to) {
    int columns = m_config.columns();
    m_elementToSwap1 = - 1;
    m_elementToSwap2 = - 1;
    emit elementToSwap1Changed(-1);
    emit elementToSwap2Changed(-1);

    if ( from + columns == to || from - columns == to || from + 1 == to || from - 1 == to) {
        swapTwoElementsWithoutSearching(from, to);

        if (!searchForMatch()) {
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout,
                    [=]() {
                swapTwoElementsWithoutSearching(to, from); });
            timer->setSingleShot(true);
            timer->start(500);
            return -1;
        }
        else {
            static int moves = 0;
            if (m_config.moves() == 0) {
                moves = 0;
            }
            moves++;
            m_config.setMoves(moves);
        }
        return -2;
    }
    return to;
}

void ModelList::remove() {

    if (m_firstSearchExecuted) {
        int removedItems = 0;

        for (int i = 0; i < 2 * m_config.rows() * m_config.columns(); i++) {

            if (m_list[i].getFlag()) {
                removedItems++;
            }
        }
        m_config.setScore(removedItems);
    }
    removeHorizontalMatch();
    removeVerticalMatch();

    for (int i = 0; i < m_config.rows() * m_config.columns(); i++) {

        if (m_list[i].getFlag()) {
            int index = i;
            int k = 0;

            while (  k < m_config.columns() ) {
                swapTwoElementsWithoutSearching(index,index + m_config.columns());
                k++;
                index += m_config.columns();
            }

            beginInsertRows(QModelIndex(), index, index);
            m_list.insert(index, Item());
            endInsertRows();
            beginRemoveRows(QModelIndex(), index + 1, index + 1 );
            m_list.removeAt(index + 1);
            endRemoveRows();
        }
    }

    if (!m_firstSearchExecuted) {
        searchForMatch();
    }
    else {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                [=]() {
            searchForMatch(); });
        timer->setSingleShot(true);
        timer->start(700);
    }
}

void ModelList::removeHorizontalMatch() {
    int columns = m_config.columns();
    int index;
    int k = 0;
    QVector<int> itemsIndex;

    for (int i = 0; i < m_removeHorizontalMatch.size(); i++) {

        for (int j = 0; j < m_removeHorizontalMatch[i].size(); j++) {
            itemsIndex.push_back(getIndex(m_removeHorizontalMatch[i][j]));
        }
    }
    for (int i = 0; i < itemsIndex.size(); i++) {
        index = itemsIndex[i];

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
    m_removeHorizontalMatch.clear();
}

void ModelList::removeVerticalMatch() {
    int rows = m_config.rows();
    int columns = m_config.columns();
    int index;
    int k = 0;
    QVector<int> itemsIndex;

    for (int i = 0; i < m_removeVerticalMatch.size(); i++) {

        for (int j = 0; j < m_removeVerticalMatch[i].size(); j++) {

            if (!m_removeVerticalMatch[i][j]->getFlag()) {
                continue;
            }
            itemsIndex.push_back(getIndex(m_removeVerticalMatch[i][j]));
        }
    }
    for (int i = 0; i < itemsIndex.size(); i++) {
        k = 0;
        index = itemsIndex[i];

        while ( k < rows ) {
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
    }
    m_removeVerticalMatch.clear();
}

GameConfig* ModelList::config() {
    return &m_config;
}


