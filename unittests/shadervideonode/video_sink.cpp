/*
 * Copyright © 2021 UBports Foundation.
 *
 * Contact: Alberto Mardegan <mardy@users.sourceforge.net>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <MediaHub/VideoSink>

using namespace lomiri::MediaHub;

namespace lomiri {
namespace MediaHub {

class VideoSinkPrivate
{
public:
    QMatrix4x4 m_transformationMatrix;
};

} // namespace MediaHub
} // namespace lomiri

VideoSink::VideoSink(VideoSinkPrivate *, QObject *parent):
    QObject(parent),
    d_ptr(new VideoSinkPrivate)
{
}

VideoSink::~VideoSink() = default;

const QMatrix4x4 &VideoSink::transformationMatrix() const
{
    Q_D(const VideoSink);
    return d->m_transformationMatrix;
}
