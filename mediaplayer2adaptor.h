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
#ifndef MEDIAPLAYER2ADAPTOR_H
#define MEDIAPLAYER2ADAPTOR_H

#include <QtDBus/QDBusAbstractAdaptor>

class QCoreApplication;
class QStringList;

class MediaPlayer2Adaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(bool HasTracklist READ HasTracklist)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)
    Q_CLASSINFO("D-Bus Introspection", "  <interface name=\"org.mpris.MediaPlayer2\">\n"
                                       "    <property access=\"read\" type=\"b\" name=\"CanQuit\"/>\n"
                                       "    <property access=\"read\" type=\"b\" name=\"CanRaise\"/>\n"
                                       "    <property access=\"read\" type=\"b\" name=\"HasTrackList\"/>\n"
                                       "    <property access=\"read\" type=\"s\" name=\"Identity\"/>\n"
                                       "    <property access=\"read\" type=\"as\" name=\"SupportedUriSchemes\"/>\n    "
                                       "    <property access=\"read\" type=\"as\" name=\"SupportedMimeTypes\"/>\n"
                                       "    <method name=\"Raise\" />\n"
                                       "    <method name=\"Quit\">\n"
                                       "     <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\"/>\n"
                                       "    </method>\n"
                                       "  </interface>\n")

    public:
        MediaPlayer2Adaptor(QCoreApplication *app);

        bool CanQuit() const;
        bool CanRaise() const;
        bool HasTracklist() const;
        QString Identity() const;
        QStringList SupportedUriSchemes() const;
        QStringList SupportedMimeTypes() const;

    public slots:
        void Raise() const;
        Q_NOREPLY void Quit() const;
};

#endif
