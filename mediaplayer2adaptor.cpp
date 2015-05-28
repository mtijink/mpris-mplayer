/*
 * Copyright 2015 Matthijs Tijink
 *
 * This file is part of mpris-mplayer.
 *
 * Mpris-mplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mpris-mplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mpris-mplayer.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mediaplayer2adaptor.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>

MediaPlayer2Adaptor::MediaPlayer2Adaptor(QCoreApplication *app) : QDBusAbstractAdaptor(app) {}

bool MediaPlayer2Adaptor::CanQuit() const {
    return true;
}
bool MediaPlayer2Adaptor::CanRaise() const {
    return false;
}
bool MediaPlayer2Adaptor::HasTracklist() const {
    return false;
}
QString MediaPlayer2Adaptor::Identity() const {
    return "Mplayer";
}
QStringList MediaPlayer2Adaptor::SupportedUriSchemes() const {
    return {};
}
QStringList MediaPlayer2Adaptor::SupportedMimeTypes() const {
    return QStringList{};
}

void MediaPlayer2Adaptor::Raise() const {}
void MediaPlayer2Adaptor::Quit() const {
    qApp->quit();
}
