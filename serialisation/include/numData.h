/**
 * Copyright (c) 2014 Mick van Duijn, Koen Visscher and Paul Visscher
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
 */

#pragma once
#ifndef __SERIALISATION_NUMDATA_H__
#define __SERIALISATION_NUMDATA_H__

#include "interface/ISerialiseData.h"

template< typename U >
class NumData
    : public ISerialiseData
{
public:

    NumData() : mValue(0)
    {}

	template< typename V >
    void Store( V &val, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            SetValue( val );
            break;

        case Mode::Deserialise:
            val = GetValue< V >();
            break;
        }
    }

	inline Type::Type GetType() const
	{
		return Type::GetEnum< NumData< U > >();
	}

    virtual size_t Size() const
    {
        return sizeof( U );
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        stream.read( ( char * )&mValue, sizeof( U ) );
    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        stream.write( ( const char * )&mValue, sizeof( U ) );
    }

protected:

    U mValue;

	template< typename V >
	inline V GetValue() const
	{
		return ( V )mValue;
	}

	template< typename V >
	inline void SetValue( const V value )
	{
		mValue = ( U )value;
	}
};

template<> 
Type::Type Type::GetEnum< NumData< uint8_t > >();

template<>
Type::Type Type::GetEnum< NumData< uint16_t > >();

template<>
Type::Type Type::GetEnum< NumData< uint32_t > >();

template<>
Type::Type Type::GetEnum< NumData< uint64_t > >();

#endif