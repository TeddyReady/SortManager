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

enum class SortOrder {
    UP   ,
    DOWN ,
};

class SortManager
{
public:
    explicit SortManager(SortOrder order = SortOrder::UP);

    int sort(SortType type, SortOrder order);

    void uploadData();

private:
    enum {FLY, DATE, NAME, PLACE};

    void makeHeap(int size, int root, SortOrder order);
    void printData(const QString &fileName, SortType type, SortOrder order);
    QDate date(const QString &dateString) const;
    bool compare(int i, int j) const;
    QString getRandomName(int i) const;

    QVector<QVariantList> data;
};

#endif // SORTMANAGER_H
