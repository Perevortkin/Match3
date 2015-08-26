#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {

public:
    Item();
    void getRandomIcon();
    int getName() const;
    QString getIcon() const;
     bool getFlag() const;

    void setName(int name);
    void setFlag(bool flag);

private:

    QString m_Icon;
    int m_name;
    bool m_flag;
};

#endif // ITEM_H
