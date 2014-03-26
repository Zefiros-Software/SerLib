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

#include "helper.h"

#include "util.h"

#include "varint.h"

#include "gtest/gtest.h"

#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#define VarIntTest( test, name, type, init1, init2 )         \
    TEST( P(test), type ## name )                     \
    {                                                        \
        std::ofstream wstream( "test", std::ios::binary );   \
        VarInt< type > wVar( init1 ), rVar( init2 );         \
        wVar.WriteToStream( wstream );                       \
        wstream.close();                                     \
        std::ifstream rstream( "test", std::ios::binary );   \
        rVar.ReadFromStream( rstream );                      \
        rstream.close();                                     \
        EXPECT_EQ( wVar.GetValue(), rVar.GetValue() );       \
        EXPECT_EQ( wVar.GetByteSize(), rVar.GetByteSize() ); \
        EXPECT_EQ( init1, wVar.GetValue() );                 \
    }

namespace
{
    template< class T >
    T GenNum( size_t numBits )
    {
        T rVal = 0;

        for ( size_t i = 0; i < numBits; i++ )
        {
            rVal |= ( 1ull << i );
        }

        return rVal;
    }

    /*VarIntTest( VarIntTestModule, 0, uint8_t, GenNum<uint8_t>( 0 ), 1 );
    VarIntTest( VarIntTestModule, 1, uint8_t, GenNum<uint8_t>( 1 ), 1 );
    VarIntTest( VarIntTestModule, 7, uint8_t, GenNum<uint8_t>( 7 ), 1 );
    VarIntTest( VarIntTestModule, 8, uint8_t, GenNum<uint8_t>( 8 ), 1 );

    VarIntTest( VarIntTestModule, 0, uint16_t, GenNum<uint16_t>( 0 ), 1 );
    VarIntTest( VarIntTestModule, 1, uint16_t, GenNum<uint16_t>( 1 ), 1 );
    VarIntTest( VarIntTestModule, 7, uint16_t, GenNum<uint16_t>( 7 ), 1 );
    VarIntTest( VarIntTestModule, 8, uint16_t, GenNum<uint16_t>( 8 ), 1 );
    VarIntTest( VarIntTestModule, 9, uint16_t, GenNum<uint16_t>( 9 ), 1 );
    VarIntTest( VarIntTestModule, 15, uint16_t, GenNum<uint16_t>( 15 ), 1 );
    VarIntTest( VarIntTestModule, 16, uint16_t, GenNum<uint16_t>( 16 ), 1 );

    VarIntTest( VarIntTestModule, 0, uint32_t, GenNum<uint32_t>( 0 ), 1 );
    VarIntTest( VarIntTestModule, 1, uint32_t, GenNum<uint32_t>( 1 ), 1 );
    VarIntTest( VarIntTestModule, 7, uint32_t, GenNum<uint32_t>( 7 ), 1 );
    VarIntTest( VarIntTestModule, 8, uint32_t, GenNum<uint32_t>( 8 ), 1 );
    VarIntTest( VarIntTestModule, 9, uint32_t, GenNum<uint32_t>( 9 ), 1 );
    VarIntTest( VarIntTestModule, 15, uint32_t, GenNum<uint32_t>( 15 ), 1 );
    VarIntTest( VarIntTestModule, 16, uint32_t, GenNum<uint32_t>( 16 ), 1 );
    VarIntTest( VarIntTestModule, 17, uint32_t, GenNum<uint32_t>( 17 ), 1 );
    VarIntTest( VarIntTestModule, 23, uint32_t, GenNum<uint32_t>( 23 ), 1 );
    VarIntTest( VarIntTestModule, 24, uint32_t, GenNum<uint32_t>( 24 ), 1 );
    VarIntTest( VarIntTestModule, 25, uint32_t, GenNum<uint32_t>( 25 ), 1 );
    VarIntTest( VarIntTestModule, 31, uint32_t, GenNum<uint32_t>( 31 ), 1 );
    VarIntTest( VarIntTestModule, 32, uint32_t, GenNum<uint32_t>( 32 ), 1 );

    VarIntTest( VarIntTestModule, 0, uint64_t, GenNum<uint64_t>( 0 ), 1 );
    VarIntTest( VarIntTestModule, 1, uint64_t, GenNum<uint64_t>( 1 ), 1 );
    VarIntTest( VarIntTestModule, 7, uint64_t, GenNum<uint64_t>( 7 ), 1 );
    VarIntTest( VarIntTestModule, 8, uint64_t, GenNum<uint64_t>( 8 ), 1 );
    VarIntTest( VarIntTestModule, 9, uint64_t, GenNum<uint64_t>( 9 ), 1 );
    VarIntTest( VarIntTestModule, 15, uint64_t, GenNum<uint64_t>( 15 ), 1 );
    VarIntTest( VarIntTestModule, 16, uint64_t, GenNum<uint64_t>( 16 ), 1 );
    VarIntTest( VarIntTestModule, 17, uint64_t, GenNum<uint64_t>( 17 ), 1 );
    VarIntTest( VarIntTestModule, 23, uint64_t, GenNum<uint64_t>( 23 ), 1 );
    VarIntTest( VarIntTestModule, 24, uint64_t, GenNum<uint64_t>( 24 ), 1 );
    VarIntTest( VarIntTestModule, 25, uint64_t, GenNum<uint64_t>( 25 ), 1 );
    VarIntTest( VarIntTestModule, 31, uint64_t, GenNum<uint64_t>( 31 ), 1 );
    VarIntTest( VarIntTestModule, 32, uint64_t, GenNum<uint64_t>( 32 ), 1 );
    VarIntTest( VarIntTestModule, 33, uint64_t, GenNum<uint64_t>( 33 ), 1 );
    VarIntTest( VarIntTestModule, 39, uint64_t, GenNum<uint64_t>( 39 ), 1 );
    VarIntTest( VarIntTestModule, 40, uint64_t, GenNum<uint64_t>( 40 ), 1 );
    VarIntTest( VarIntTestModule, 41, uint64_t, GenNum<uint64_t>( 41 ), 1 );
    VarIntTest( VarIntTestModule, 47, uint64_t, GenNum<uint64_t>( 47 ), 1 );
    VarIntTest( VarIntTestModule, 48, uint64_t, GenNum<uint64_t>( 48 ), 1 );
    VarIntTest( VarIntTestModule, 49, uint64_t, GenNum<uint64_t>( 49 ), 1 );
    VarIntTest( VarIntTestModule, 55, uint64_t, GenNum<uint64_t>( 55 ), 1 );
    VarIntTest( VarIntTestModule, 56, uint64_t, GenNum<uint64_t>( 56 ), 1 );
    VarIntTest( VarIntTestModule, 57, uint64_t, GenNum<uint64_t>( 57 ), 1 );
    VarIntTest( VarIntTestModule, 63, uint64_t, GenNum<uint64_t>( 63 ), 1 );
    VarIntTest( VarIntTestModule, 64, uint64_t, GenNum<uint64_t>( 64 ), 1 );

    VarIntTest( VarIntTestModule, zebra, uint8_t, GenerateZebraValue<uint8_t>(), 1 );
    VarIntTest( VarIntTestModule, zebra, uint16_t, GenerateZebraValue<uint16_t>(), 1 );
    VarIntTest( VarIntTestModule, zebra, uint32_t, GenerateZebraValue<uint32_t>(), 1 );

    VarIntTest( VarIntTestModule, zebra, uint64_t, GenerateZebraValue<uint64_t>(), 1 );
    VarIntTest( VarIntTestModule, zebraInv, uint8_t, GenerateInvZebraValue<uint8_t>(), 1 );
    VarIntTest( VarIntTestModule, zebraInv, uint16_t, GenerateInvZebraValue<uint16_t>(), 1 );
    VarIntTest( VarIntTestModule, zebraInv, uint32_t, GenerateInvZebraValue<uint32_t>(), 1 );
    VarIntTest( VarIntTestModule, zebraInv, uint64_t, GenerateInvZebraValue<uint64_t>(), 1 );*/

}









