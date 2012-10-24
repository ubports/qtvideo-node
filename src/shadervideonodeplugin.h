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

#ifndef SHADERVIDEONODEPLUGIN_H
#define SHADERVIDEONODEPLUGIN_H

#include <private/qsgvideonode_p.h>
#include <QMutex>
#include <QVideoSurfaceFormat>

class ShaderVideoMaterial;

class ShaderVideoNode : public QSGVideoNode
{
public:
    ShaderVideoNode(const QVideoSurfaceFormat &format);

    virtual QVideoFrame::PixelFormat pixelFormat() const {
        return m_format.pixelFormat();
    }
    void setCurrentFrame(const QVideoFrame &frame);

private:
    QMutex m_guard;
    QVideoSurfaceFormat m_format;
    QVideoFrame m_frame;
    ShaderVideoMaterial *m_material;
};

class ShaderVideoNodePlugin : public QObject, public QSGVideoNodeFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QSGVideoNodeFactoryInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.sgvideonodefactory/5.0" FILE "shadervideonode.json")

public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    QSGVideoNode *createNode(const QVideoSurfaceFormat &format);
};

#endif // SHADERVIDEONODEPLUGIN_H
