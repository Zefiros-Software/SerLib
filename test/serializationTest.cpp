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

#include "serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <climits>
#include <stdlib.h>
#include <stdio.h>
#include <climits>


#define TestSerialClass( test, name, type, seed1, seed2, flag, its )      \
    TEST( P( test ), type ## name )                                            \
    {                                                                     \
        TestClass1< type, its, flag > c1( seed1 ), c2( seed2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_EQ( c1.mMember[i], c2.mMember[i]  );                           \
        }                                                                     \
    }

#define TestFSerialClass( test, name, type, seed1, seed2, flag, its )     \
    TEST( P( test ), type ## name )                                            \
    {                                                                     \
        TestClass1< type, its, flag > c1( seed1 ), c2( seed2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_FLOAT_EQ( c1.mMember[i], c2.mMember[i]  );                     \
        }                                                                     \
    }

#define TestDSerialClass( test, name, type, seed1, seed2, flag, its )     \
    TEST( P( test ), type ## name )                                            \
    {                                                                     \
        TestClass1< type, its, flag > c1( seed1 ), c2( seed2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_DOUBLE_EQ( c1.mMember[i], c2.mMember[i]  );                    \
        }                                                                     \
    }

#define TestIFDMixedSerialClass( test, name, typeI, seed1, seed2, flag, its )  \
    TEST( P( test ), typeI ## name )                                            \
    {                                                                     \
        TestClass2< typeI, float, double, its, flag > c1( seed1 ), c2( seed2 );  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_EQ( c1.mMemberT[i], c2.mMemberT[i]  );                         \
            EXPECT_FLOAT_EQ( c1.mMemberS[i], c2.mMemberS[i]  );                   \
            EXPECT_DOUBLE_EQ( c1.mMemberR[i], c2.mMemberR[i]  );                  \
        }                                                                     \
    }

#define TestFIDMixedSerialClass( test, name, typeI, seed1, seed2, flag, its ) \
    TEST( P( test ), typeI ## name )                                            \
    {                                                                     \
        TestClass2< float, typeI, double, its, flag > c1( seed1 ), c2( seed2 );  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_EQ( c1.mMemberS[i], c2.mMemberS[i]  );                         \
            EXPECT_FLOAT_EQ( c1.mMemberT[i], c2.mMemberT[i]  );                   \
            EXPECT_DOUBLE_EQ( c1.mMemberR[i], c2.mMemberR[i]  );                  \
        }                                                                     \
    }

#define TestIDIMixedSerialClass( test, name, typeI1, typeI2, seed1, seed2, flag, its ) \
    TEST( P( test ), typeI1 ## typeI2 ## name )                                            \
    {                                                                     \
        TestClass2< typeI1, double, typeI2, its, flag > c1( seed1 ), c2( seed2 );  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                           \
        for ( uint32_t i=0; i < its; i++ )                                    \
        {                                                                     \
            EXPECT_EQ( c1.mMemberT[i], c2.mMemberT[i]  );                         \
            EXPECT_DOUBLE_EQ( c1.mMemberS[i], c2.mMemberS[i]  );                  \
            EXPECT_EQ( c1.mMemberR[i], c2.mMemberR[i]  );                         \
        }                                                                     \
    }


namespace
{
    template< typename T, uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass1
        : public ISerialisable
    {
    public:

        TestClass1( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMember[i] = ( T )( ( ( double )( rand() / RAND_MAX ) ) * std::numeric_limits< T >::max() );
            }
        }

        void OnSerialise( Message &message )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                message.Store( mMember[i], i, Flag );
            }
        }

        T mMember[its];
    };

    template< typename T, typename S, typename R, uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass2
        : public ISerialisable
    {
    public:

        TestClass2( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberT[i] = ( T )( ( ( double )( rand() / RAND_MAX ) ) * std::numeric_limits< T >::max() );
                mMemberS[i] = ( S )( ( ( double )( rand() / RAND_MAX ) ) * std::numeric_limits< S >::max() );
                mMemberR[i] = ( R )( ( ( double )( rand() / RAND_MAX ) ) * std::numeric_limits< R >::max() );
            }
        }

        void OnSerialise( Message &message )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                uint32_t j = 3 * i;
                message.Store( mMemberT[i], j, Flag );
                message.Store( mMemberS[i], j + 1, Flag );
                message.Store( mMemberR[i], j + 2, Flag );
            }
        }

        T mMemberT[its];
        S mMemberS[its];
        R mMemberR[its];
    };


    TestSerialClass( MultiSerialization, randomVals, uint8_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint16_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint32_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint64_t, 343422, 21331, 0x00, 100 );

    TestSerialClass( MultiSerialization, randomVals, int8_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int16_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int32_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int64_t, 343422, 21331, 0x00, 100 );

    TestFSerialClass( MultiSerialization, randomVals, float, 343422, 21331, 0x00, 100 );
    TestDSerialClass( MultiSerialization, randomVals, double, 343422, 21331, 0x00, 100 );


    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, uint8_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, uint16_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, uint32_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, uint64_t, 343422, 21331, 0x00, 100 );

    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, int8_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, int16_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, int32_t, 343422, 21331, 0x00, 100 );
    TestIFDMixedSerialClass( MixedIFDSerialization, randomVals, int64_t, 343422, 21331, 0x00, 100 );

    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, uint8_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, uint16_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, uint32_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, uint64_t, 343422, 21331, 0x00, 100 );

    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, int8_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, int16_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, int32_t, 343422, 21331, 0x00, 100 );
    TestFIDMixedSerialClass( MixedFIDSerialization, randomVals, int64_t, 343422, 21331, 0x00, 100 );

    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, uint8_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, uint16_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, uint32_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, uint64_t, 343422, 21331, 0x01, 100 );

    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, int8_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, int16_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, int32_t, 343422, 21331, 0x01, 100 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialization, randomVals, int64_t, 343422, 21331, 0x01, 100 );

    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, uint8_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, uint16_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, uint32_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, uint64_t, 343422, 21331, 0x01, 100 );

    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, int8_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, int16_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, int32_t, 343422, 21331, 0x01, 100 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialization, randomVals, int64_t, 343422, 21331, 0x01, 100 );
}