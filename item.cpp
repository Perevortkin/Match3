#include "item.h"
#include <QDebug>

Item::Item():QObject(0) {
    getRandomIcon();
}

Item::Item(const Item & item):QObject(0)
{
    m_name = item.getName();
    m_flag = item.getFlag();
    m_Icon = item.getIcon();
}


void Item::refresh()
{
    getRandomIcon();
}

int Item::getName() const {
    return m_name;
}

void Item::setName(int name) {
    m_name = name;
}

QString Item::getIcon() const {
    return m_Icon;
}

bool Item::getFlag() const {
    return m_flag;
}

void Item::setFlag(bool flag) {
    m_flag = flag;
    emit flagChanged();
}

Item &Item::operator =(const Item & item)
{
    m_name = item.getName();
    m_flag = item.getFlag();
    m_Icon = item.getIcon();
    return *this;
}

    void Item::getRandomIcon() {

        int n = rand()% 5 + 1;
        m_name = n;
        m_flag = false;
        switch (m_name) {
        case 1:
            m_Icon = "qrc:/icons/icons/apple.png";
            break;
        case 2:
            m_Icon = "qrc:/icons/icons/pepper.png";
            break;
        case 3:
            m_Icon = "qrc:/icons/icons/pear.png";
            break;
        case 4:
            m_Icon = "qrc:/icons/icons/carrot.png";
            break;
        case 5:
            m_Icon = "qrc:/icons/icons/watermelon.png";
            break;
        }

    }




