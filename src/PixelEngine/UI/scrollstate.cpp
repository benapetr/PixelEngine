//! \file scrollstate.cpp
//! \brief Implementation of shared UI scrolling state.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "scrollstate.h"
#include <algorithm>

using namespace PE;

void ScrollState::SetSizes(pe_float_t viewportSize, pe_float_t contentSize)
{
    this->viewportSize = std::max<pe_float_t>(0, viewportSize);
    this->contentSize = std::max<pe_float_t>(0, contentSize);
    this->clampOffset();
}

void ScrollState::SetOffset(pe_float_t offset)
{
    this->offset = offset;
    this->clampOffset();
}

void ScrollState::ScrollBy(pe_float_t delta)
{
    this->offset += delta;
    this->clampOffset();
}

void ScrollState::ScrollLine(int direction)
{
    this->ScrollBy(this->LineStep * direction);
}

void ScrollState::ScrollPage(int direction)
{
    pe_float_t step = this->PageStep > 0 ? this->PageStep : this->viewportSize;
    this->ScrollBy(step * direction);
}

pe_float_t ScrollState::GetOffset() const
{
    return this->offset;
}

pe_float_t ScrollState::GetMaxOffset() const
{
    return std::max<pe_float_t>(0, this->contentSize - this->viewportSize);
}

pe_float_t ScrollState::GetContentSize() const
{
    return this->contentSize;
}

pe_float_t ScrollState::GetViewportSize() const
{
    return this->viewportSize;
}

pe_float_t ScrollState::GetThumbSize(pe_float_t trackSize) const
{
    if (!this->CanScroll() || this->contentSize <= 0)
        return trackSize;

    pe_float_t ratio = this->viewportSize / this->contentSize;
    return std::max<pe_float_t>(12, trackSize * ratio);
}

pe_float_t ScrollState::GetThumbOffset(pe_float_t trackSize) const
{
    pe_float_t maxOffset = this->GetMaxOffset();
    if (maxOffset <= 0)
        return 0;

    pe_float_t thumbSize = this->GetThumbSize(trackSize);
    pe_float_t trackTravel = std::max<pe_float_t>(0, trackSize - thumbSize);
    return trackTravel * (this->offset / maxOffset);
}

bool ScrollState::CanScroll() const
{
    return this->contentSize > this->viewportSize;
}

void ScrollState::clampOffset()
{
    this->offset = std::max<pe_float_t>(0, std::min(this->offset, this->GetMaxOffset()));
}
