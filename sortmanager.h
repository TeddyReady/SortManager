#ifndef SORTMANAGER_H
#define SORTMANAGER_H
#include <QVariantList>
#include <QString>
#include <QVector>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>

enum class SortType {
    BUBBLE,
    CHOICE,
    PYRAMID,
};

class SortManager
{
public:
    SortManager();

    int sort(SortType type);

    void uploadData();

private:
    enum {FLY, DATE, NAME, PLACE};
    void printData(const QString &fileName);

    QVector<QVariantList> data;
};

#endif // SORTMANAGER_H
