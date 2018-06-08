/******************************************************************************
* Copyright (c) 2018, Howard Butler (howard@hobu.co)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/
#pragma once

#include "pdal/PointTable.hpp"

namespace pdal
{


class PDAL_DLL ArrowPointTable : public BasePointTable
{

protected:
    ArrowPointTable(PointLayout& layout) : BasePointTable(layout)
        {}

//    ~ArrowPointTable() {};

protected:
    std::size_t pointsToBytes(point_count_t numPts) const
        { return m_layoutRef.pointSize() * numPts; }

private:
    virtual void setFieldInternal(Dimension::Id id, PointId idx,
        const void *value);
    virtual void getFieldInternal(Dimension::Id id, PointId idx,
        void *value) const;

    // The number of points in each memory block.
    char *getDimension(const Dimension::Detail *d, PointId idx)
        { return getPoint(idx) + d->offset(); }

    const char *getDimension(const Dimension::Detail *d, PointId idx) const
    {
        ArrowPointTable *ncThis = const_cast<ArrowPointTable *>(this);
        return ncThis->getPoint(idx) + d->offset();
    }
};
} // namespace pdal