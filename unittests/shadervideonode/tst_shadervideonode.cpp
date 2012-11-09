/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Guenter Schwann <guenter.schwann@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtTest/QtTest>

#include <QImage>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>

#include <shadervideomaterial.h>

#include "camera_compatibility_layer.h"

#define private public
#include "shadervideonode.h"

class tst_ShaderVideoNode : public QObject
{
    Q_OBJECT
private slots:
    void testSetCurrentFrame();
};

void tst_ShaderVideoNode::testSetCurrentFrame()
{
    QVideoSurfaceFormat format;
    ShaderVideoNode node(format);

    CameraControl *cc = new CameraControl;
    QImage img(320, 240, QImage::Format_ARGB32);
    QVideoFrame frame(img);

    node.setCurrentFrame(frame);
    QCOMPARE((int)node.m_material->cameraControl(), 0);

    frame.setMetaData("CamControl", (int)cc);
    node.setCurrentFrame(frame);
    QCOMPARE((int)node.m_material->cameraControl(), (int)cc);
}

QTEST_MAIN(tst_ShaderVideoNode)

#include "tst_shadervideonode.moc"
