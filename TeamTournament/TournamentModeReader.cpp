﻿// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#include "TournamentModeReader.h"
#include "TournamentMode.h"

#include <QString>
#include <QStringList>
#include <QSettings>

TournamentModeReader::TournamentModeReader()
    : m_modes()
{
}

