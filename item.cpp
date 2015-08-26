#include "item.h"
#include <QDebug>

Item::Item() {
    getRandomIcon();
}
int Item::getName() const {
    return m_name;
}

void Item::setName(int name) {
    m_name = name;
}
QString Item::getIcon() const
{
    return m_Icon;
}

//void Item::setIcon(const QString &Icon)
//{
//    m_Icon = Icon;
//}
bool Item::getFlag() const
{
    return m_flag;
}

void Item::setFlag(bool flag)
{
    m_flag = flag;
}

QString Item::getIconSource() {
    switch (m_name) {
    case 1:
        m_Icon = "qrc:/icons/icons/cloudy65.png";
        break;
    case 2:
        m_Icon = "qrc:/icons/icons/lightning31.png";
        break;
    case 3:
        m_Icon = "qrc:/icons/icons/rain14.png";
        break;
    case 4:
        m_Icon = "qrc:/icons/icons/snowflake3.png";
        break;
    case 5:
        m_Icon = "qrc:/icons/icons/sunny62.png";
        break;
    }
    return m_Icon;
}

void Item::getRandomIcon() {

    int n = rand()% 5 + 1;
    m_name = n;
    m_flag = false;
    switch (m_name) {
    case 1:
        m_Icon = "qrc:/icons/icons/cloudy65.png";
        break;
    case 2:
        m_Icon = "qrc:/icons/icons/lightning31.png";
        break;
    case 3:
        m_Icon = "qrc:/icons/icons/rain14.png";
        break;
    case 4:
        m_Icon = "qrc:/icons/icons/snowflake3.png";
        break;
    case 5:
        m_Icon = "qrc:/icons/icons/sunny62.png";
        break;
    }

}




