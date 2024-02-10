#include "sortmanager.h"

/**
 * @brief Data::Data
 * @param fly - number of flight
 * @param sDate - date in format 'dd/mm/yyyy'
 * @param name - name in format 'Some Person'
 * @param place - number of place in airplane
 */
Data::Data(int fly, const QString &sDate, const QString &name, int place)
    : fly(fly), place(place), sDate(sDate), name(name) {}

/**
 * @brief Data::at
 * @param index - number of field (see DataType)
 * @return One of date fields in QVariant
 */
QVariant Data::at(int index) const
{
    QVariant item;
    switch (index)
    {
    case 0:
        item = QVariant(fly);
        break;
    case 1:
        item = QVariant(sDate);
        break;
    case 2:
        item = QVariant(name);
        break;
    case 3:
        item = QVariant(place);
        break;
    }
    return item;
}

/**
 * @brief Data::date
 * @param dateString - string in format 'dd/mm/yyyy'
 * @return data string converted in QDate
 */
QDate Data::date(const QString &dateString) const
{
    QStringList dateList = dateString.split("/");
    return QDate(dateList.at(2).toInt(), dateList.at(1).toInt(), dateList.at(0).toInt());
}

/**
 * @brief Data::operator ==
 * @param other
 * @return Result of compare
 */
bool Data::operator== (const Data &other) const
{
    return date(sDate) == date(other.sDate) && fly == other.fly && name.compare(other.name) == 0 && place == other.place;
}

/**
 * @brief Data::operator !=
 * @param other
 * @return Result of compare
 */
bool Data::operator!= (const Data &other) const
{
    return !(*this == other);
}

/**
 * @brief Data::operator <
 * @param other
 * @return Result of compare
 */
bool Data::operator< (const Data &other) const
{
    if (date(sDate) < date(other.sDate))
        return true;

    else if (date(sDate) == date(other.sDate) && fly < other.fly)
        return true;

    else if (date(sDate) == date(other.sDate) && fly == other.fly && name.compare(other.name) < 0)
        return true;

    else if (date(sDate) == date(other.sDate) && fly == other.fly && name.compare(other.name) == 0 && place < other.place)
        return true;

    return false;
}

/**
 * @brief Data::operator <=
 * @param other
 * @return Result of compare
 */
bool Data::operator<= (const Data &other) const
{
    return (*this < other) || (*this == other);
}

/**
 * @brief Data::operator >
 * @param other
 * @return Result of compare
 */
bool Data::operator> (const Data &other) const
{
    return !(*this <= other);
}

/**
 * @brief Data::operator >=
 * @param other
 * @return Result of compare
 */
bool Data::operator>= (const Data &other) const
{
    return !(*this < other);
}

/**
 * @brief SortManager::SortManager
 * @param count - number of generated data
 * @param order - SortOrder::UP or SortOrder::DOWN
 *
 * Generate and sort data, check delay
 */
SortManager::SortManager(int count, SortOrder order)
{
    double time = 0;
    const char* ext;

    uploadData(count);
    qDebug() << "Begin bubble sorting...";
    delay.start();
    sort(SortType::BUBBLE, order);
    time = delay.elapsed();
    ext = (time < 1000) ? "ms" : "sec";
    time = (time < 1000) ? time : static_cast<double>(time / 1000);
    qDebug() << "Finished bubble sorting for " << time << ext;
    printData("./bubble.md", SortType::BUBBLE, order);

    uploadData(count);
    qDebug() << "Begin inserts sorting...";
    delay.restart();
    sort(SortType::CHOICE, order);
    time = delay.elapsed();
    ext = (time < 1000) ? "ms" : "sec";
    time = (time < 1000) ? time : static_cast<double>(time / 1000);
    qDebug() << "Finished inserts sorting for " << time << ext;
    printData("./insert.md", SortType::CHOICE, order);


    uploadData(count);
    qDebug() << "Begin pyramid sorting...";
    delay.restart();
    sort(SortType::PYRAMID, order);
    time = delay.elapsed();
    ext = (time < 1000) ? "ms" : "sec";
    time = (time < 1000) ? time : static_cast<double>(time / 1000);
    qDebug() << "Finished pyramid sorting for " << time << ext;
    printData("./pyramid.md", SortType::PYRAMID, order);
}

/**
 * @brief SortManager::sort
 * @param type - type of sort (check SortType)
 * @param order - order of sort (check SortOrder)
 *
 * Sorting data in correct order with chosen algorithm
 */
void SortManager::sort(SortType type, SortOrder order)
{
    switch (type)
    {
    case SortType::BUBBLE:
        switch (order)
        {
        case SortOrder::UP:
            for (int i = 1; i < data.size(); ++i)
            {
                for(int j = 0; j < data.size() - i; ++j)
                    if (data[j+1] < data[j])
                        qSwap(data[j], data[j+1]);
            }
            break;

        case SortOrder::DOWN:
            for (int i = 1; i < data.size(); ++i)
            {
                for(int j = 0; j < data.size() - i; ++j)
                    if (data[j] < data[j+1])
                        qSwap(data[j], data[j+1]);
            }
            break;
        }
        break;

    case SortType::CHOICE:
        switch (order)
        {
        case SortOrder::UP:
            for (int i = 1; i < data.size(); ++i)
            {
                Data curEl = data[i];
                for (int j = i - 1; j >= 0 && data[j] > curEl; j--)
                    qSwap(data[j], data[j+1]);
            }
            break;

        case SortOrder::DOWN:
            for (int i = 1; i < data.size(); ++i)
            {
                Data curEl = data[i];
                for (int j = i - 1; j >= 0 && data[j] < curEl; j--)
                    qSwap(data[j], data[j+1]);
            }
            break;
        }
        break;

    case SortType::PYRAMID:
        for (int i = data.size() / 2 - 1; i >= 0; --i)
            makeHeap(data.size(), i, order);

        for (int i = data.size() - 1; i >= 0; --i)
        {
            qSwap(data[0], data[i]);

            makeHeap(i, 0, order);
        }
        break;
    }
}

/**
 * @brief SortManager::uploadData
 * @param count - number of generating data
 *
 * Generate data for sort
 */
void SortManager::uploadData(int count)
{
    qDebug() << "Begin data generating...";
    data.clear();

    for (int i = count; i >=0; --i)
        data.append(Data(i%3, QString("11/%1/2023").arg(i%8), getRandomName(i), i%4));

    qDebug() << "Data uploaded!";
}

/**
 * @brief SortManager::getRandomName
 * @param i - magic number for random
 * @return Random name for data
 */
QString SortManager::getRandomName(int i) const
{
    QString result;
    switch (i % 6) {
    case 0:
        result.append("Ilya ");
        break;
    case 1:
        result.append("Roman ");
        break;
    case 2:
        result.append("Vasiliy ");
        break;
    case 3:
        result.append("George ");
        break;
    case 4:
        result.append("Petr ");
        break;
    case 5:
        result.append("Gleb ");
        break;
    }

    switch (i % 4) {
    case 0:
        result.append("Kataev");
        break;
    case 1:
        result.append("Garkin");
        break;
    case 2:
        result.append("Ivanov");
        break;
    case 3:
        result.append("Samoilov");
        break;
    }
    return result;
}

/**
 * @brief SortManager::makeHeap
 * @param size - size of current heap
 * @param root - index of current root element
 * @param order - order of sort
 *
 * Create a heap with current size and root. This func is part of Pyramid sort
 */
void SortManager::makeHeap(int size, int root, SortOrder order)
{
    int maxIndex = root;
    int leftIndex = 2 * root + 1;
    int rightIndex = 2 * root + 2;

    switch (order)
    {
    case SortOrder::UP:
        if (leftIndex < size && data[maxIndex] < data[leftIndex])
            maxIndex = leftIndex;

        if (rightIndex < size && data[maxIndex] < data[rightIndex])
            maxIndex = rightIndex;
        break;
    case SortOrder::DOWN:
        if (leftIndex < size && data[leftIndex] < data[maxIndex])
            maxIndex = leftIndex;

        if (rightIndex < size && data[rightIndex] < data[maxIndex])
            maxIndex = rightIndex;
        break;
    }

    if (maxIndex != root)
    {
        qSwap(data[root], data[maxIndex]);

        makeHeap(size, maxIndex, order);
    }
}

/**
 * @brief SortManager::printData
 * @param fileName - fullPath of file, where sorted data will be saved
 * @param type - type of sort
 * @param order - type of order
 *
 * Print all sorted data in file in MarkDown format
 */
void SortManager::printData(const QString &fileName, SortType type, SortOrder order)
{
    qDebug() << "Begin data printing...";
    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) )
    {
        qDebug() << "Print file not exists";
        return;
    }

    QTextStream out(&file);

    out << "# ";
    switch(type)
    {
    case SortType::BUBBLE:
        out << "BUBBLE SORT";
        break;
    case SortType::CHOICE:
        out << "CHOICE SORT";
        break;
    case SortType::PYRAMID:
        out << "PYRAMID SORT";
        break;
    }

    out << "\n## ";
    switch(order)
    {
    case SortOrder::UP:
        out << "Sorted in upper order";
        break;
    case SortOrder::DOWN:
        out << "Sorted in lower order";
        break;
    }

    out << "\n\nIndex" << "\t|" << "Fly" << "\t|" << "Date" << "|\t\t"  << "Name" << "|\t\t"  << "Place";
    out << "\n---|" << "---|" << "---|---" << "|---";
    int i = 1;
    foreach (auto list, data)
    {
        out << "\n";
        out << i << "|\t" << list.at(FLY).toInt() << "|\t" << list.at(DATE).toString() << "|\t"  << list.at(NAME).toString() << "|\t"  << list.at(PLACE).toInt();
        ++i;
    }

    qDebug() << "Data printed!";
}
