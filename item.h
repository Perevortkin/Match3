#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {

public:
    Item();
    void getRandomIcon();
    int getName() const;
    void setName(int name);

    QString getIcon() const;
    void setIcon(const QString &Icon);

    bool getFlag() const;
    void setFlag(bool flag);

private:

    QString m_Icon;
    int m_name;
    bool m_flag;
};

#endif // ITEM_H
