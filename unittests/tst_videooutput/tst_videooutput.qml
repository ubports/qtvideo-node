/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.2
import QtMultimedia 5.0
import QtTest 1.1

TestCase {
    id: root
    name: "VideoOutput"
    when: windowShown
    visible: true
    width: 400
    height: 400

    function test_z_ordering_after_rotation() {
        player.play();
        player.seek(1000);
        rotation.angle = -2;
        waitForRendering(videoOutput);
        var image = grabImage(root);
        var actualColor = String(image.pixel(20, 20));
        var expectedColor = String(middleRectangle.color);
        compare(actualColor, expectedColor);
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        transform: [
            Rotation {
                id: rotation
                axis.x: 0; axis.y: 1; axis.z: 0
                angle: 0
            }
        ]
        fillMode: VideoOutput.Stretch
        source: MediaPlayer {
            id: player
            source: "small.mp4"
        }
    }

    Rectangle {
        id: middleRectangle
        anchors.fill: parent
        color: "green"
    }

    Rectangle {
        id: topRectangle
        anchors.fill: parent
        anchors.leftMargin: 50
        color: "red"
    }
}
