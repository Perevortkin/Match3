#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QObject>
#include <QPointer>
class Item: public QObject
{

    Q_OBJECT
public:

    Item();
    Item(const Item &);
    void refresh();
    void getRandomIcon();
    int getName() const;
    QString getIcon() const;
    bool getFlag() const;

    void setName(int name);
    void setFlag(bool flag);
    Item& operator = (const Item &);

private:

    QString m_Icon;
    int m_name;
    bool m_flag;
};

#endif // ITEM_H
