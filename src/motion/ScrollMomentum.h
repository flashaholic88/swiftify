/*
 * Swiftify, a spotify player for desktop
 * Copyright (C) 2026  shred0s
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QElapsedTimer>
#include <QList>
#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QTimer>
#include <QWindow>

class QWheelEvent;

namespace swiftify {

/// a trackpad swipe glides on once the fingers lift, as it does on macos.
/// macos sends the glide itself, as wheel events in the momentum phase, and
/// qt's flickables follow them; wayland compositors (libinput) stop at the
/// lift and leave the glide to the app, so a list stopped dead under the
/// fingers. installed on the application, this watches a window's trackpad
/// scrolls and, after one ends, sends the momentum events macos would have:
/// the swipe's speed, slowing, until it rests or a new touch stops it
class ScrollMomentum : public QObject {
    Q_OBJECT

public:
    explicit ScrollMomentum(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    struct Sample {
        quint64 timestamp;
        QPointF delta;
    };

    void track(QWindow *window, const QWheelEvent *event);
    void start(const QWheelEvent *end);
    void glide();
    void stop();
    void send(Qt::ScrollPhase phase, const QPoint &pixelDelta);

    QList<Sample> m_samples;
    QTimer m_timer;
    QElapsedTimer m_clock;
    qint64 m_lastTick = 0;
    QPointer<QWindow> m_window;
    QPointF m_position;
    QPointF m_globalPosition;
    Qt::KeyboardModifiers m_modifiers;
    bool m_inverted = false;
    quint64 m_timestamp = 0;
    QPointF m_velocity;
    QPointF m_remainder;
    bool m_gliding = false;
    bool m_sending = false;
};

} // namespace swiftify
