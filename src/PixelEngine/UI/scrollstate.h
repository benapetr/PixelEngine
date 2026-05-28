//! \file scrollstate.h
//! \brief Header file for shared UI scrolling state.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef SCROLLSTATE_H
#define SCROLLSTATE_H

#include "../definitions.h"

namespace PE
{
    class ScrollState
    {
        public:
            void SetSizes(pe_float_t viewportSize, pe_float_t contentSize);
            void SetOffset(pe_float_t offset);
            void ScrollBy(pe_float_t delta);
            void ScrollLine(int direction);
            void ScrollPage(int direction);
            pe_float_t GetOffset() const;
            pe_float_t GetMaxOffset() const;
            pe_float_t GetContentSize() const;
            pe_float_t GetViewportSize() const;
            pe_float_t GetThumbSize(pe_float_t trackSize) const;
            pe_float_t GetThumbOffset(pe_float_t trackSize) const;
            bool CanScroll() const;

            pe_float_t LineStep = 22;
            pe_float_t PageStep = 0;

        private:
            void clampOffset();

            pe_float_t viewportSize = 0;
            pe_float_t contentSize = 0;
            pe_float_t offset = 0;
    };
}

#endif // SCROLLSTATE_H
