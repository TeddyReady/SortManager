#include "sortmanager.h"

SortManager::SortManager()
{
    uploadData();
    sort(SortType::BUBBLE);

    uploadData();
    sort(SortType::CHOICE);
}

int SortManager::sort(SortType type)
{
    switch (type)
    {
    case SortType::BUBBLE:
        /* Отсортируем массив по убыванию */
        for (int i = 1; i < data.size(); ++i)
        {
            for(int j = 0; j < data.size() - i; ++j)
            {
                QStringList sCurDate = data.at(j).at(DATE).toString().split("/");
                QStringList sNextDate = data.at(j+1).at(DATE).toString().split("/");
                QDate curDate(sCurDate.at(2).toInt(), sCurDate.at(1).toInt(), sCurDate.at(0).toInt());
                QDate nextDate(sNextDate.at(2).toInt(), sNextDate.at(1).toInt(), sNextDate.at(0).toInt());
                int curFly = data.at(j).at(FLY).toInt();
                int nextFly = data.at(j+1).at(FLY).toInt();
                QString curName = data.at(j).at(NAME).toString();
                QString nextName = data.at(j+1).at(NAME).toString();
                int curPlace = data.at(j).at(PLACE).toInt();
                int nextPlace = data.at(j+1).at(PLACE).toInt();

                if (curDate < nextDate)
                    qSwap(data[j], data[j+1]);

                if (curDate == nextDate && curFly < nextFly)
                    qSwap(data[j], data[j+1]);

                if (curDate == nextDate && curFly == nextFly && curName.compare(nextName) < 0)
                    qSwap(data[j], data[j+1]);

                if (curDate == nextDate && curFly == nextFly && curName.compare(nextName) == 0 && curPlace < nextPlace)
                    qSwap(data[j], data[j+1]);
            }
        }
        break;

    case SortType::CHOICE:

        for (int i = 1; i < data.size(); ++i)
        {
            auto tmp = data.at(i);
            QStringList sCurDate = data.at(i).at(DATE).toString().split("/");
            QStringList sNextDate = data.at(i - 1).at(DATE).toString().split("/");
            QDate curDate(sCurDate.at(2).toInt(), sCurDate.at(1).toInt(), sCurDate.at(0).toInt());
            QDate nextDate(sNextDate.at(2).toInt(), sNextDate.at(1).toInt(), sNextDate.at(0).toInt());
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
                    sNextDate = data.at(j - 1).at(DATE).toString().split("/");
                    nextDate = QDate(sNextDate.at(2).toInt(), sNextDate.at(1).toInt(), sNextDate.at(0).toInt());
                    nextFly = data.at(j - 1).at(FLY).toInt();
                    nextName = data.at(j - 1).at(NAME).toString();
                    nextPlace = data.at(j - 1).at(PLACE).toInt();
                }

                flag = nextDate < curDate || (nextDate == curDate && nextFly < curFly) || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) > 0)
                       || (nextDate == curDate && nextFly == curFly && curName.compare(nextName) == 0 && nextPlace < curPlace);
            }
        }
        break;

    case SortType::PYRAMID:
        break;
    }

    printData("./example.txt");
    return 0;
}

void SortManager::uploadData()
{
    data.clear();

    for (int i = 20; i >=0; --i)
        data.append(QVariantList({i%3, QString("11/%1/2023").arg(i%8), i % 2 == 0 ? "SUKA BLYAT" : "Archi HUI", i%4}));
}

void SortManager::printData(const QString &fileName)
{
    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text | QFile::Append) )
    {
        qDebug() << "Print file not exists";
        return;
    }

    QTextStream out(&file);
    out << "\n\nFly" << "\t" << "Date" << "\t\t"  << "Name" << "\t\t"  << "Place";

    foreach (auto list, data)
    {
        out << "\n";
        out << list.at(FLY).toInt() << "\t" << list.at(DATE).toString() << "\t"  << list.at(NAME).toString() << "\t"  << list.at(PLACE).toInt();
    }
}
