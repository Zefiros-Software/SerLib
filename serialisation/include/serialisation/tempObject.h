/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */
#pragma once
#ifndef __SERIALISATION_TEMPOBJECT_H__
#define __SERIALISATION_TEMPOBJECT_H__

#include "serialisation/interface/ITempData.h"

#include <vector>

class TempObject
    : public ITempData
{
public:

    TempObject();

    virtual Internal::Type::Type GetType() const;

    void AddData(uint8_t index, ITempData *data);

    ITempData *TryRemoveData(uint8_t index);

    size_t GetSize();

    ITempData *PopBack();

    bool GetTerminatorRead() const;

    void SetTerminatorRead();

    bool IsNonEmpty() const;

private:

    std::vector< std::pair< uint8_t, ITempData * >> mTempData;

    bool mTerminatorRead;
};

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/tempObject.cpp"
#endif

#endif