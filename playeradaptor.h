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
#ifndef PLAYERADAPTOR_H
#define PLAYERADAPTOR_H

#include <QtDBus/QDBusAbstractAdaptor>
#include <memory>

class QObject;
class QString;
class QProcess;

class PlayerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QString LoopStatus READ LoopStatus WRITE setLoopStatus)
    Q_PROPERTY(double Rate READ Rate WRITE setRate)
    Q_PROPERTY(bool Shuffle READ Shuffle WRITE setShuffle)
    Q_PROPERTY(double Volume READ Volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position READ Position)
    Q_PROPERTY(double MinimumRate READ MinimumRate)
    Q_PROPERTY(double MaximumRate READ MaximumRate)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(bool CanControl READ CanControl)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
    Q_CLASSINFO("D-Bus Introspection", "  <interface name=\"org.mpris.MediaPlayer2.Player\">\n"
                                       "   <property name=\"PlaybackStatus\" type=\"s\" access=\"read\" />\n"
                                       "   <property name=\"LoopStatus\" type=\"s\" access=\"readwrite\" />\n"
                                       "   <property name=\"Rate\" type=\"d\" access=\"readwrite\" />\n"
                                       "   <property name=\"Shuffle\" type=\"b\" access=\"readwrite\" />\n"
                                       "   <property name=\"Volume\" type=\"d\" access=\"readwrite\" />\n"
                                       "   <property name=\"Position\" type=\"x\" access=\"read\" />\n"
                                       "   <property name=\"MinimumRate\" type=\"d\" access=\"read\" />\n"
                                       "   <property name=\"MaximumRate\" type=\"d\" access=\"read\" />\n"
                                       "   <property name=\"CanGoNext\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"CanGoPrevious\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"CanPlay\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"CanPause\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"CanSeek\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"CanControl\" type=\"b\" access=\"read\" />\n"
                                       "   <property name=\"Metadata\" type=\"a{sv}\" access=\"read\" />\n"

                                       "   <signal name=\"Seeked\">\n"
                                       "    <arg name=\"Position\" type=\"x\" direction=\"out\" />\n"
                                       "   </signal>\n"

                                       "   <method name=\"PlayPause\" />\n"
                                       "   <method name=\"Stop\" />\n"
                                       "   <method name=\"Play\" />\n"
                                       "   <method name=\"Pause\" />\n"
                                       "   <method name=\"Next\" />\n"
                                       "   <method name=\"Previous\" />\n"
                                       "   <method name=\"Seek\">\n"
                                       "    <arg name=\"Offset\" type=\"x\" direction=\"in\" />\n"
                                       "   </method>\n"
                                       "   <method name=\"SetPosition\">\n"
                                       "    <arg name=\"TrackId\" type=\"s\" direction=\"in\" />\n"
                                       "    <arg name=\"Position\" type=\"x\" direction=\"in\" />\n"
                                       "   </method>\n"
                                       "   <method name=\"OpenUri\">\n"
                                       "    <arg name=\"Uri\" type=\"s\" direction=\"in\" />\n"
                                       "   </method>\n"
                                       "  </interface>\n")

    private:
        double playing;
        int index;
        QProcess *process;

        void notifyChanges();
        void notifySwitch(bool isSwitching);
        void doPlay();

    public:
        PlayerAdaptor(QString choose, QObject *parent);
        ~PlayerAdaptor();

        QString PlaybackStatus() const;
        QString LoopStatus() const;
        void setLoopStatus(QString ls) const;
        double Rate() const;
        void setRate(double r) const;
        bool Shuffle() const;
        void setShuffle(bool s) const;
        double Volume() const;
        double setVolume(double v) const;
        qlonglong Position() const;
        double MinimumRate() const;
        double MaximumRate() const;
        bool CanGoNext() const;
        bool CanGoPrevious() const;
        bool CanPlay() const;
        bool CanPause() const;
        bool CanSeek() const;
        bool CanControl() const;
        QVariantMap Metadata() const;

    public slots:
        void PlayPause();
        void Stop();
        void Play();
        void Pause();
        void Next();
        void Previous();

        //stubs
        void Seek(const qlonglong &Offset) const;
        void SetPosition(const QString &TrackId, const qlonglong &Position) const;
        void OpenUri(const QString &Uri) const;

        void finishedMplayer();

    signals:
        void Seeked(qlonglong Position);
};

#endif
