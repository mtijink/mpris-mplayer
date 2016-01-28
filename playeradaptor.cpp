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
#include "playeradaptor.h"

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

struct RadioChannel {
    QString name;
    QString url;
    bool needPlaylist;
    std::vector<QString> aliases;
};

const std::vector<RadioChannel> channels{
    {"3 FM", "http://icecast.omroep.nl/3fm-bb-aac", false, {"3FM", "3fm", "3 FM", "3 fm"}},
    {"Radio 10", "http://www.radio10.nl/players/stream/radio10.asx", true, {"Radio10", "radio10", "Radio 10", "radio 10"}},
    {"Radio 538", "http://vip-icecast.538.lw.triple-it.nl/RADIO538_MP3.m3u", true, {"Radio 538", "radio 538", "Radio538", "radio538", "538"}},
    {"Radio Veronica", "http://8543.live.streamtheworld.com/VERONICACMP3", false, {"Veronica", "veronica"}},
    {"Radio 2", "http://icecast.omroep.nl/radio2-bb-aac", false, {"Radio 2", "Radio2", "radio 2", "radio2"}}
};

PlayerAdaptor::PlayerAdaptor(QString choose, QObject *parent): QDBusAbstractAdaptor(parent), playing(false), index(0) {
    bool found = false;
    for (int i = 0; i<channels.size(); ++i) {
        auto &channel = channels[i];
        for (auto &alias : channel.aliases) {
            if (alias==choose) {
                index = i;
                found = true;
            }
        }
    }

    if (!found && choose.size()>0) {
        throw 5;
    }
    Play();
}
PlayerAdaptor::~PlayerAdaptor() = default;

QString PlayerAdaptor::PlaybackStatus() const {
    if (playing) return "Playing";
    return "Paused";
}
QString PlayerAdaptor::LoopStatus() const {
    return "None";
}
void PlayerAdaptor::setLoopStatus(QString ls) const {}
double PlayerAdaptor::Rate() const {
    return 1;
}
void PlayerAdaptor::setRate(double r) const {}
bool PlayerAdaptor::Shuffle() const {
    return false;
}
void PlayerAdaptor::setShuffle(bool s) const {}
double PlayerAdaptor::Volume() const {
    return 1;
}
double PlayerAdaptor::setVolume(double v) const {}
double PlayerAdaptor::MinimumRate() const {
    return 1;
}
qlonglong PlayerAdaptor::Position() const {
    return 0;
}
double PlayerAdaptor::MaximumRate() const {
    return 1;
}
bool PlayerAdaptor::CanGoNext() const {
    return channels.size()>1;
}
bool PlayerAdaptor::CanGoPrevious() const {
    return channels.size()>1;
}
bool PlayerAdaptor::CanPlay() const {
    return true;
}
bool PlayerAdaptor::CanPause() const {
    return true;
}
bool PlayerAdaptor::CanSeek() const {
    return false;
}
bool PlayerAdaptor::CanControl() const {
    return true;
}
QVariantMap PlayerAdaptor::Metadata() const {
    QVariantMap res;
    QString track = "/org/mpris/MediaPlayer2/mplayer/track/";
    track += index;
    res.insert("mpris:trackid", track);
    res.insert("xesam:title", channels[index].name);
    return res;
}

void PlayerAdaptor::PlayPause() {
    if (playing) {
        Stop();
    } else {
        Play();
    }
}
void PlayerAdaptor::Stop() {
    if (!playing) return;
    if (!process) return;
    disconnect(process, SIGNAL(finished(int)), this, SLOT(finishedMplayer()));
    process->terminate();
    process = nullptr;
    playing = false;
    notifyChanges();
}
void PlayerAdaptor::Play() {
    if (playing) return;
    doPlay();
    notifySwitch(false);
}
void PlayerAdaptor::Pause() {
    Stop();
}
void PlayerAdaptor::Next() {
    Stop();
    index = (index+1)%channels.size();
    doPlay();
    notifySwitch(true);
}
void PlayerAdaptor::Previous() {
    Stop();
    index = (index+channels.size()-1)%channels.size();
    doPlay();
    notifySwitch(true);
}

void PlayerAdaptor::Seek(const qlonglong &Offset) const {}
void PlayerAdaptor::SetPosition(const QString &TrackId, const qlonglong &Position) const {}
void PlayerAdaptor::OpenUri(const QString &uri) const {}

void PlayerAdaptor::finishedMplayer() {
    process = nullptr;
    playing = false;
    notifyChanges();
}

void PlayerAdaptor::notifyChanges() {
    QVariantMap changes;
    changes.insert("PlaybackStatus", PlaybackStatus());
    changes.insert("Metadata", Metadata());
    QList<QVariant> args;
    QString interface = "org.mpris.MediaPlayer2.Player";
    args.push_back(QVariant(interface));
    args.push_back(changes);
    args.push_back(QStringList{});
    QDBusMessage message = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    message.setArguments(args);
    QDBusConnection::sessionBus().send(message);
}
void PlayerAdaptor::notifySwitch(bool isSwitching) {
    QList<QVariant> args;
    args << "mpris-mplayer"; //app name
    args << (uint) 4242; //notification id
    args << "kmix"; //app icon
    if (isSwitching) {
        args << "Switching radio"; //summary
        args << "New radio channel: "+channels[index].name; //body
    } else {
        args << "Starting radio"; //summary
        args << "Radio channel: "+channels[index].name; //body
    }
    args << QStringList{}; //actions
    args << QVariantMap{}; //hints
    args << (int) 5000; //timeout

    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications",
                                                          "org.freedesktop.Notifications", "CloseNotification");
    message.setArguments({(uint) 4242});
    QDBusConnection::sessionBus().send(message);
    message = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications",
                                                          "org.freedesktop.Notifications", "Notify");
    message.setArguments(args);
    QDBusConnection::sessionBus().send(message);
}
void PlayerAdaptor::doPlay() {
    if (playing) return;
    QStringList args;
    if (channels[index].needPlaylist) args << "-playlist";
    args << channels[index].url;
    process = new QProcess(this);
    process->start("mplayer", args);
    playing = true;

    connect(process, SIGNAL(finished(int)), this, SLOT(finishedMplayer()));
    notifyChanges();
}
