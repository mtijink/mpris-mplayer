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
#include <QtCore/QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <iostream>

#include "mediaplayer2adaptor.h"
#include "playeradaptor.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/org/mpris/MediaPlayer2", &app);
    dbus.registerService("org.mpris.MediaPlayer2.mplayer");
    new MediaPlayer2Adaptor(&app);

    QString arg = "";
    if (argc>=2) arg = argv[1];
    try {
        new PlayerAdaptor(arg, &app);
    } catch (...) {
        std::cerr << "Invalid radio station \"" << argv[1] << "\"" << std::endl;
        return 1;
    }

    return app.exec();
}
