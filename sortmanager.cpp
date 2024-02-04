#include "sortmanager.h"

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

int SortManager::sort(SortType type, SortOrder order)
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
                    if (compare(j+1, j))
                        qSwap(data[j], data[j+1]);
            }
            break;

        case SortOrder::DOWN:
            for (int i = 1; i < data.size(); ++i)
            {
                for(int j = 0; j < data.size() - i; ++j)
                    if (compare(j, j+1))
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
                QDate curDate = date(data.at(i).at(DATE).toString());
                QDate nextDate = date(data.at(i - 1).at(DATE).toString());
                int curFly = data.at(i).at(FLY).toInt();
                int nextFly = data.at(i - 1).at(FLY).toInt();
                QString curName = data.at(i).at(NAME).toString();
                QString nextName = data.at(i - 1).at(NAME).toString();
                int curPlace = data.at(i).at(PLACE).toInt();
                int nextPlace = data.at(i - 1).at(PLACE).toInt();

                bool flag = nextDate > curDate || (nextDate == curDate && nextFly > curFly) || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) < 0)
                            || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) == 0 && nextPlace > curPlace);

                for (int j = i - 1; j >= 0 && flag; j--)
                {
                    qSwap(data[j], data[j+1]);

                    if (j - 1 >= 0)
                    {
                        nextDate = date(data.at(j - 1).at(DATE).toString());
                        nextFly = data.at(j - 1).at(FLY).toInt();
                        nextName = data.at(j - 1).at(NAME).toString();
                        nextPlace = data.at(j - 1).at(PLACE).toInt();
                    }

                    flag = nextDate > curDate || (nextDate == curDate && nextFly > curFly) || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) < 0)
                           || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) == 0 && nextPlace > curPlace);
                }
            }
            break;

        case SortOrder::DOWN:
            for (int i = 1; i < data.size(); ++i)
            {
                QDate curDate = date(data.at(i).at(DATE).toString());
                QDate nextDate = date(data.at(i - 1).at(DATE).toString());
                int curFly = data.at(i).at(FLY).toInt();
                int nextFly = data.at(i - 1).at(FLY).toInt();
                QString curName = data.at(i).at(NAME).toString();
                QString nextName = data.at(i - 1).at(NAME).toString();
                int curPlace = data.at(i).at(PLACE).toInt();
                int nextPlace = data.at(i - 1).at(PLACE).toInt();

                bool flag = nextDate < curDate || (nextDate == curDate && nextFly < curFly) || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) > 0)
                            || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) == 0 && nextPlace < curPlace);

                for (int j = i - 1; j >= 0 && flag; j--)
                {
                    qSwap(data[j], data[j+1]);

                    if (j - 1 >= 0)
                    {
                        nextDate = date(data.at(j - 1).at(DATE).toString());
                        nextFly = data.at(j - 1).at(FLY).toInt();
                        nextName = data.at(j - 1).at(NAME).toString();
                        nextPlace = data.at(j - 1).at(PLACE).toInt();
                    }

                    flag = nextDate < curDate || (nextDate == curDate && nextFly < curFly) || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) > 0)
                           || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) == 0 && nextPlace < curPlace);
                }
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
    return 0;
}

void SortManager::uploadData(int count)
{
    qDebug() << "Begin data generating...";
    data.clear();

    for (int i = count; i >=0; --i)
        data.append(QVariantList({i%3, QString("11/%1/2023").arg(i%8), getRandomName(i), i%4}));

    qDebug() << "Data uploaded!";
}

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

/* Equiv data[i] < data[j] */
bool SortManager::compare(int i, int j) const
{
    QDate curDate = date(data.at(i).at(DATE).toString());
    QDate nextDate = date(data.at(j).at(DATE).toString());
    int curFly = data.at(i).at(FLY).toInt();
    int nextFly = data.at(j).at(FLY).toInt();
    QString curName = data.at(i).at(NAME).toString();
    QString nextName = data.at(j).at(NAME).toString();
    int curPlace = data.at(i).at(PLACE).toInt();
    int nextPlace = data.at(j).at(PLACE).toInt();

    if (curDate < nextDate)
        return true;

    else if (curDate == nextDate && curFly < nextFly)
        return true;

    else if (curDate == nextDate && curFly == nextFly && curName.compare(nextName) < 0)
        return true;

    else if (curDate == nextDate && curFly == nextFly && curName.compare(nextName) == 0 && curPlace < nextPlace)
        return true;

    return false;
}

void SortManager::makeHeap(int size, int root, SortOrder order)
{
    int maxIndex = root;
    int leftIndex = 2 * root + 1;
    int rightIndex = 2 * root + 2;

    switch (order)
    {
    case SortOrder::UP:
        if (leftIndex < size && compare(maxIndex, leftIndex))
            maxIndex = leftIndex;

        if (rightIndex < size && compare(maxIndex, rightIndex))
            maxIndex = rightIndex;
        break;
    case SortOrder::DOWN:
        if (leftIndex < size && compare(leftIndex, maxIndex))
            maxIndex = leftIndex;

        if (rightIndex < size && compare(rightIndex, maxIndex))
            maxIndex = rightIndex;
        break;
    }

    if (maxIndex != root)
    {
        qSwap(data[root], data[maxIndex]);

        makeHeap(size, maxIndex, order);
    }
}

QDate SortManager::date(const QString &dateString) const
{
    QStringList dateList = dateString.split("/");
    return QDate(dateList.at(2).toInt(), dateList.at(1).toInt(), dateList.at(0).toInt());
}

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
