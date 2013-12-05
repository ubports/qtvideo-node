/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "surface_texture_client_hybris.h"

#include <QtGlobal>

typedef void* SurfaceTextureClientHybris;

void surface_texture_client_get_transformation_matrix(SurfaceTextureClientHybris stc, float *matrix)
{
    Q_UNUSED(stc);
    Q_UNUSED(matrix);
}

void surface_texture_client_update_texture(SurfaceTextureClientHybris stc)
{
    Q_UNUSED(stc);
}
