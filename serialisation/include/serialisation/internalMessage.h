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
#ifndef __SERIALISATION_INTERNALMESSAGE_H__
#define __SERIALISATION_INTERNALMESSAGE_H__

#include "serialisation/types.h"

class InternalMessage
{
public:

    virtual ~InternalMessage()
    {
    }

    virtual void ClearBuffer() = 0;

    virtual void InitObject() = 0;
    virtual void FinishObject() = 0;

    virtual bool InitObject(uint8_t index) = 0;
    virtual void FinishObject(uint8_t index) = 0;

    virtual bool InitParent(uint8_t index) = 0;
    virtual void FinishParent(uint8_t index) = 0;

    virtual void InitArrayObject() = 0;
    virtual void FinishArrayObject() = 0;

    virtual void Store(std::string &value, uint8_t index) = 0;

    virtual void Store(uint8_t &value, uint8_t index) = 0;
    virtual void Store(uint16_t &value, uint8_t index) = 0;
    virtual void Store(uint32_t &value, uint8_t index) = 0;
    virtual void Store(uint64_t &value, uint8_t index) = 0;

    virtual void Store(int8_t &value, uint8_t index) = 0;
    virtual void Store(int16_t &value, uint8_t index) = 0;
    virtual void Store(int32_t &value, uint8_t index) = 0;
    virtual void Store(int64_t &value, uint8_t index) = 0;

    virtual void Store(float &value, uint8_t index) = 0;
    virtual void Store(double &value, uint8_t index) = 0;

    virtual size_t CreateArray(Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00) = 0;

    virtual void StoreArrayItem(std::string &value) = 0;

    virtual void StoreArrayItem(uint8_t &value) = 0;
    virtual void StoreArrayItem(uint16_t &value) = 0;
    virtual void StoreArrayItem(uint32_t &value) = 0;
    virtual void StoreArrayItem(uint64_t &value) = 0;

    virtual void StoreArrayItem(int8_t &value) = 0;
    virtual void StoreArrayItem(int16_t &value) = 0;
    virtual void StoreArrayItem(int32_t &value) = 0;
    virtual void StoreArrayItem(int64_t &value) = 0;

    virtual void StoreArrayItem(float &value) = 0;
    virtual void StoreArrayItem(double &value) = 0;



    virtual void StoreContiguous(uint8_t *begin, size_t size) = 0;
    virtual void StoreContiguous(uint16_t *begin, size_t size) = 0;
    virtual void StoreContiguous(uint32_t *begin, size_t size) = 0;
    virtual void StoreContiguous(uint64_t *begin, size_t size) = 0;

    virtual void StoreContiguous(int8_t *begin, size_t size) = 0;
    virtual void StoreContiguous(int16_t *begin, size_t size) = 0;
    virtual void StoreContiguous(int32_t *begin, size_t size) = 0;
    virtual void StoreContiguous(int64_t *begin, size_t size) = 0;

    virtual void StoreContiguous(float *begin, size_t size) = 0;
    virtual void StoreContiguous(double *begin, size_t size) = 0;
};

#endif