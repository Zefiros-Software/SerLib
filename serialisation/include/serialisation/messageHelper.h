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
#ifndef __SERIALISATION_MESSAGEHELPER_H__
#define __SERIALISATION_MESSAGEHELPER_H__

#include "serialisation/interface/ISerialisable.h"

namespace MessageHelper
{
    template< typename TSerialisable >
    void SERIALISATION_CUSTOM_INTERFACE(TSerialisable &serialisable, Message &message)
    {
        serialisable.SERIALISATION_CUSTOM_INTERFACE(message);
    }

    template< typename TParent >
    void StoreParent(TParent &parent, Message &message)
    {
        parent.TParent::SERIALISATION_CUSTOM_INTERFACE(message);
    }

    template< typename TSerialisable >
    void Store(Message &message, TSerialisable &serialisable, bool clearBuffers = true);

    void Store(Message &message, ISerialisable &serialisable);
}

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/messageHelper.cpp"
#endif

#endif