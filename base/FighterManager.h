// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
#ifndef _BASE_FIGHTERMANAGER_H
#define _BASE_FIGHTERMANAGER_H

#include "../core/fighter.h"

#include <QString>
#include <boost/noncopyable.hpp>
#include <array>

namespace Ipponboard
{

class FighterManager : public boost::noncopyable
{
public:
    FighterManager();

    static char const * const str_FIRSTNAME;
    static char const * const str_LASTNAME;
    static char const * const str_CLUB;
    static char const * const str_WEIGHT;
    static char const * const str_CATEGORY;

    static const std::array<char const * const, 5> Specifiers;

    static QString GetSpecifiererDescription();
    //static bool IsValidSpecifier(QString const& str);
    static bool IsFormatSatisfying(QString const& formatStr);
    static bool DetermineSeparator(QString const& str, QString& sep);

    QString GetExportFormat() const { return m_exportFormat; }
    void SetExportFormat(QString const& formatStr) { m_exportFormat = formatStr; }

    bool ImportFighters(QString const& fileName, QString& errorMsg);
    bool ExportFighters(QString const& fileName, QString& errorMsg);

//private:
    std::vector<Ipponboard::Fighter> m_fighters; //TODO: encapsulate

private:
    QString m_exportFormat;
};

}  // namespace Ipponboard

#endif // _BASE_FIGHTERMANAGER_H
