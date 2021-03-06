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
#ifndef __SERIALISATION_TYPES_H__
#define __SERIALISATION_TYPES_H__

#include <stdint.h>
#include <string>

class Message;

template< typename T >
class SerialiseData;

namespace Internal
{
    namespace Mode
    {
        enum Mode
        {
            Serialise       = 0x00,
            Deserialise     = 0x01,
            DeserialiseTemp = 0x02
        };
    }

    namespace Format
    {
        enum Format
        {
            Binary = 0x00
        };
    }

    namespace Flags
    {
        enum Flags
        {
            Packed = 0x01
        };
    }

    namespace Type
    {
        // underlying type is uint8_t
        enum Type
        {
            Terminator = 0x00,
            Array      = 0x01,
            Object     = 0x02,
            String     = 0x03,
            UInt8      = 0x04,
            UInt16     = 0x05,
            UInt32     = 0x06,
            UInt64     = 0x07,
            SInt8      = 0x08,
            SInt16     = 0x09,
            SInt32     = 0x0A,
            SInt64     = 0x0B,
            Float      = 0x0C,
            Double     = 0x0D
        };

        bool IsInteger(Type type);

        bool IsSignedInt(Type type);

        bool AreCompatible(const Type type, const Type type2);

        template< typename T >
        inline Type GetEnum()
        {
            return Object;
        }

        template<>
        inline Type GetEnum< std::string >()
        {
            return String;
        }

        template<>
        inline Type GetEnum< uint8_t >()
        {
            return UInt8;
        }

        template<>
        inline Type GetEnum< uint16_t >()
        {
            return UInt16;
        }

        template<>
        inline Type GetEnum< uint32_t >()
        {
            return UInt32;
        }

        template<>
        inline Type GetEnum< uint64_t >()
        {
            return UInt64;
        }

        template<>
        inline Type GetEnum< int8_t >()
        {
            return SInt8;
        }

        template<>
        inline Type GetEnum< int16_t >()
        {
            return SInt16;
        }

        template<>
        inline Type GetEnum< int32_t >()
        {
            return SInt32;
        }

        template<>
        inline Type GetEnum< int64_t >()
        {
            return SInt64;
        }

        template<>
        inline Type GetEnum< float >()
        {
            return Float;
        }

        template<>
        inline Type GetEnum< double >()
        {
            return Double;
        }
    }
}

namespace Type
{
    enum Type
    {
        Object   = Internal::Type::Object,
        String   = Internal::Type::String,
        UInt8    = Internal::Type::UInt8,
        UInt16   = Internal::Type::UInt16,
        UInt32   = Internal::Type::UInt32,
        UInt64   = Internal::Type::UInt64,
        SInt8    = Internal::Type::SInt8,
        SInt16   = Internal::Type::SInt16,
        SInt32   = Internal::Type::SInt32,
        SInt64   = Internal::Type::SInt64,
        Float    = Internal::Type::Float,
        Double   = Internal::Type::Double
    };
}

namespace Mode
{
    enum Mode
    {
        Serialise   = Internal::Mode::Serialise,
        Deserialise = Internal::Mode::Deserialise
    };
}

namespace Format
{
    enum Format
    {
        Binary = Internal::Format::Binary
    };
}

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/types.cpp"
#endif

#endif