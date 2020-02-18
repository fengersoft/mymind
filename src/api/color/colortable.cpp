#include "colortable.h"
ColorTable* ColorTable::m_instance = nullptr;
int ColorTable::fontColorIndex = 0;
int ColorTable::backColorIndex = ColorTable::lastIndex();
QStack<int> ColorTable::colorIndexes;
ColorTable::ColorTable(QObject* parent)
    : QObject(parent)
{

    int n = 0;
    int values[5] = { 0, 64, 128, 192, 255 };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                m_colors << QColor(values[i], values[j], values[k]);
                n++;
            }
        }
    }
}

QList<QColor>& ColorTable::colors()
{
    return m_colors;
}

ColorTable* ColorTable::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new ColorTable();
    }
    return m_instance;
}

void ColorTable::freeColorTable()
{
    if (m_instance != nullptr) {
        delete m_instance;
    }
}

QColor ColorTable::getColor(int i)
{
    if (i == -1) {
        i = 0;
    }
    return getInstance()->colors().at(i);
}

void ColorTable::addColorIndex(int i)
{
    if (!colorIndexes.contains(i)) {
        colorIndexes.push(i);
    }
}

int ColorTable::lastIndex()
{
    static int ret = getInstance()->colors().count() - 1;
    return ret;
}
