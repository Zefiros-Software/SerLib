#pragma region copyright
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
#pragma endregion

#include "stringSerialiseData.h"
#include "varint.h"
#include "util.h"

void StringSerialiseData::Store( std::string &str, Mode::Mode mode )
{
    switch ( mode )
    {
    case Mode::Serialise:
        mString = str;
        break;

    case Mode::Deserialise:
        str = mString;
        break;
    }
}

Type::Type StringSerialiseData::GetType() const
{
    return Type::String;
}

size_t StringSerialiseData::Size() const
{
    return mString.size() + Util::CalculateVarIntSize( mString.size() );
}

void StringSerialiseData::ReadFromStream( std::istream &stream )
{
    VarInt< size_t > length;
    length.ReadFromStream( stream );
    mString.resize( length.GetValue() );
    stream.read( &*mString.begin(), length.GetValue() );
}

void StringSerialiseData::WriteToStream( std::ostream &stream ) const
{
    VarInt< size_t > length( mString.length() );
    length.WriteToStream( stream );
    stream.write( &*mString.begin(), mString.size() );
}
