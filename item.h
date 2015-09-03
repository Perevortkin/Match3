#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QObject>

class Item: public QObject
{

    Q_OBJECT
    Q_PROPERTY(bool flag READ getFlag WRITE setFlag NOTIFY flagChanged)

public:

    Item();
    Item(const Item &);
    Item& operator = (const Item &);

    void getRandomIcon();
    QString getIcon() const;
    void refresh();

    int getType() const;
    void setType(int name);

    bool getFlag() const;
    void setFlag(bool flag);

signals:
    void flagChanged();

private:
    QString m_Icon;
    int m_type;
    bool m_flag;
};

#endif // ITEM_H
