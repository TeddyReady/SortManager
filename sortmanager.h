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
#include <QElapsedTimer>


/**
 * @brief Type of sort
 */
enum class SortType
{
    BUBBLE  , ///< Bubble sort
    CHOICE  , ///< Sort with fast inserts
    PYRAMID , ///< Pyramid (heap) sort
};

/**
 * @brief Order of sort
 */
enum class SortOrder
{
    UP   , ///< From min to max (1, 2, 3...)
    DOWN , ///< From max to min (3, 2, 1...)
};

/**
 * @brief Types of data, which containing in Data class
 */
enum DataType
{
    FLY     , ///< 1th field
    DATE    , ///< 2nd field
    NAME    , ///< 3td field
    PLACE   , ///< 4th field
};

/**
 * @brief The Data class
 *
 * Wrapper for 4 fields.
 * Class for comparing in right order, between its fields
 */
class Data
{
public:

    explicit Data(int fly, const QString &sDate, const QString &name, int place);

    QVariant at(int index) const;
    bool operator== (const Data &other) const;
    bool operator!= (const Data &other) const;
    bool operator< (const Data &other)  const;
    bool operator<= (const Data &other) const;
    bool operator> (const Data &other)  const;
    bool operator>= (const Data &other) const;

private:
    QDate date(const QString &dateString) const;

    int fly;        ///< number of flight
    int place;      ///< number of place in airplane
    QString sDate;  ///< date in format 'dd/mm/yyyy'
    QString name;   ///< name in format 'Some Person'
};

/**
 * @brief The SortManager class
 *
 * Class for sorting and generating data (Data)
 */
class SortManager
{
public:
    explicit SortManager(int count, SortOrder order = SortOrder::UP);

private:
    void uploadData(int count);
    void sort(SortType type, SortOrder order);

    void makeHeap(int size, int root, SortOrder order);
    void printData(const QString &fileName, SortType type, SortOrder order);
    QString getRandomName(int i) const;

    QVector<Data> data;
    QElapsedTimer delay;
};

#endif // SORTMANAGER_H
