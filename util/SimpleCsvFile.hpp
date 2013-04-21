// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
#ifndef _UTIL_SIMPLECSVFILE_HPP
#define _UTIL_SIMPLECSVFILE_HPP

#include <QFile>
#include <QObject>  // needed for tr()
#include <QString>
#include <QTextStream>

#include <boost/noncopyable.hpp>


namespace fmu
{

class SimpleCsvFile : boost::noncopyable
{
public:
    static bool ReadItems(
            QString const& fileName,
            QString const& separator,
            std::vector<QStringList>& readData,
            QString& errorMsg)
    {
        errorMsg.clear();

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            errorMsg = QObject::tr("Unable to open file %1").arg(fileName);
            return false;
        }

        readData.clear();

        QTextStream in(&file);
        int lineNo = 0;
        int itemsPerLine = 0;

        for (QString line = in.readLine(); !line.isNull(); ++lineNo)
        {
            QStringList splittedLine = line.split(separator);

            if (itemsPerLine == 0)
            {
                itemsPerLine = splittedLine.length();
            }

            if (splittedLine.length() != itemsPerLine)
            {
                errorMsg = QObject::tr("Error parsing file %1:\n"
                              "line %2 has %3 items instead of %4 like in the previous lines")
                        .arg(fileName)
                        .arg(QString::number(lineNo+1))
                        .arg(QString::number(itemsPerLine))
                        .arg(QString::number(splittedLine.length()));

                return false;
            }

            readData.push_back(splittedLine);

            line = in.readLine();
        }

        file.close();

        return true;
    }

    static bool WriteData(
            QString const& fileName,
            QStringList const& data,
            QString& errorMsg)
    {
        errorMsg.clear();

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            errorMsg = QObject::tr("Unable to open file %1 for writing").arg(fileName);
            return false;
        }

        if (data.empty())
        {
        //    errorMsg = QObject::tr("Error writing to file %1:\nno data to write").arg(fileName);
        //    return false;
        }
		else
		{
			QTextStream outStream(&file);
			Q_FOREACH(QString const& line, data)
			{
				outStream << line << endl;
			}
		}

        return true;
    }
};
}

#endif // _UTIL_SIMPLECSVFILE_HPP