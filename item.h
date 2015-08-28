#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QObject>
#include <QPointer>
class Item: public QObject
{

    Q_OBJECT
    Q_PROPERTY(bool flag READ getFlag WRITE setFlag NOTIFY flagChanged)
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
signals:
  void  flagChanged();
private:

    QString m_Icon;
    int m_name;
    bool m_flag;
};

#endif // ITEM_H
