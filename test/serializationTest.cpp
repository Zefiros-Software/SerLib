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

#include "serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <climits>
#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include <vector>

#define TestSerialClass( test, name, type, seed1, seed2, flag, its )      \
    TEST( P( test ), type ## name )                                       \
    {                                                                     \
        TestClass1< type, its, flag > c1( seed1 ), c2( seed2 );           \
        SimpleSerialiseDeserialiseStream( c1, c2 );                       \
        c1.TestEqual( c2 );                                               \
    }

#define TestFSerialClass( test, name, seed1, seed2, flag, its )     \
    TEST( P( test ), float ## name )                                \
    {                                                               \
        TestClass1F< its, flag > c1( seed1 ), c2( seed2 );          \
        SimpleSerialiseDeserialiseStream( c1, c2 );                 \
        c1.TestEqual( c2 );                                         \
    }

#define TestDSerialClass( test, name, seed1, seed2, flag, its ) \
    TEST( P( test ), double ## name )                           \
    {                                                           \
        TestClass1D< its, flag > c1( seed1 ), c2( seed2 );      \
        SimpleSerialiseDeserialiseStream( c1, c2 );             \
        c1.TestEqual( c2 );                                     \
    }

#define TestIFDMixedSerialClass( test, name, typeI, seed1, seed2, flag, its )   \
    TEST( P( test ), typeI ## name )                                            \
    {                                                                           \
        TestClass2F1< typeI, its, flag > c1( seed1 ), c2( seed2 );              \
        SimpleSerialiseDeserialiseStream( c1, c2 );                             \
        c1.TestEqual( c2 );                                                     \
    }

#define TestFIDMixedSerialClass( test, name, typeI, seed1, seed2, flag, its )  \
    TEST( P( test ), typeI ## name )                                           \
    {                                                                          \
        TestClass2F2< typeI, its, flag > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseStream( c1, c2 );                            \
        c1.TestEqual( c2 );                                                    \
    }

#define TestIDIMixedSerialClass( test, name, typeI1, typeI2, seed1, seed2, flag, its )  \
    TEST( P( test ), typeI1 ## typeI2 ## name )                                         \
    {                                                                                   \
        TestClass2F3< typeI1, typeI2, its, flag > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                     \
        c1.TestEqual( c2 )                                                              \
    }

#define TestEasyRepeatedClass( test, name, seed1, seed2, flag, its ) \
    TEST( P( test ), name )                                          \
    {                                                                \
        TestClass3< its, flag > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );                  \
        c1.TestEqual( c2 );                                          \
    }

#define TestRepeatedPrimitiveMessageClass( test, name, seed1, seed2, flag, its, type )      \
    TEST( P( test ), type ## name )                                                         \
    {                                                                                       \
        TestClass4< Primitive< type, 0x00 >, its, flag, type > c1( seed1 ), c2( seed2 );    \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                         \
        c1.TestEqual( c2 );                                                                 \
    }

namespace TestClasses
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
                mMember[i] = ( T )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< T >::max() );
            }
        }

        void OnSerialise( Message &message )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                message.Store( mMember[i], i, Flag );
            }
        }

        virtual void TestEqual( TestClass1 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_EQ( mMember[i], c2.mMember[i] );
            }
        }

        T mMember[its];
    };

    template< uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass1F
        : public TestClass1< float, its, Flag >
    {
    public:

        TestClass1F( uint32_t seed = 233232 )
            : TestClass1< float, its, Flag >( seed )
        {
        }

        virtual void TestEqual( TestClass1< float, its, Flag > &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_FLOAT_EQ( mMember[i], c2.mMember[i] );
            }
        }
    };

    template< uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass1D
        : public TestClass1< double, its, Flag >
    {
    public:

        TestClass1D( uint32_t seed = 233232 )
            : TestClass1< double, its, Flag >( seed )
        {
        }

        virtual void TestEqual( TestClass1< double, its, Flag > &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_DOUBLE_EQ( mMember[i], c2.mMember[i] );
            }
        }
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
                mMemberT[i] = ( T )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< T >::max() );
                mMemberS[i] = ( S )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< S >::max() );
                mMemberR[i] = ( R )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< R >::max() );
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

        virtual void TestEqual( TestClass2 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_EQ( mMemberR[i], c2.mMemberR[i] );
            }
        }

        T mMemberT[its];
        S mMemberS[its];
        R mMemberR[its];
    };

    template< typename T, uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass2F1
        : public TestClass2< T, float, double, its, Flag >
    {
    public:

        TestClass2F1( uint32_t seed = 233232 )
            : TestClass2< T, float, double, its, Flag >( seed )
        {
        }

        virtual void TestEqual( TestClass2 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_FLOAT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_DOUBLE_EQ( mMemberR[i], c2.mMemberR[i] );
            }
        }
    };

    template< typename T, uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass2F2
        : public TestClass2< float, T, double, its, Flag >
    {
    public:

        TestClass2F2( uint32_t seed = 233232 )
            : TestClass2< float, T, double, its, Flag >( seed )
        {
        }

        virtual void TestEqual( TestClass2 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_FLOAT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_DOUBLE_EQ( mMemberR[i], c2.mMemberR[i] );
            }
        }
    };

    template< typename T, typename R, uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass2F3
        : public TestClass2< T, double, R, its, Flag >
    {
    public:

        TestClass2F3( uint32_t seed = 233232 )
            : TestClass2< T, double, R, its, Flag >( seed )
        {
        }

        virtual void TestEqual( TestClass2 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_DOUBLE_EQ( mMemberS[i], c2.mMemberS[i] );
            }
        }
    };

    template< uint32_t its = 100, uint32_t Flag = 0x00 >
    class TestClass3
        : public ISerialisable
    {
    public:

        TestClass3( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberT.push_back( ( uint8_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< uint8_t >::max() ) );
                mMemberS.push_back( ( uint16_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< uint16_t >::max() ) );
                mMemberR.push_back( ( uint32_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< uint32_t >::max() ) );
                mMemberG.push_back( ( uint64_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< uint64_t >::max() ) );

                mMemberTs.push_back( ( int8_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< int8_t >::max() ) );
                mMemberSs.push_back( ( int16_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< int16_t >::max() ) );
                mMemberRs.push_back( ( int32_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< int32_t >::max() ) );
                mMemberGs.push_back( ( int64_t )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< int64_t >::max() ) );

                mMemberF.push_back( ( float )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< float >::max() ) );
                mMemberD.push_back( ( double )( ( ( double )rand() / RAND_MAX ) * std::numeric_limits< double >::max() ) );

            }

            name1 = "TestClassTres";
            name2 = "CouldBeUsedToStoreNames";
        }

        void OnSerialise( Message &message )
        {
            message.CreateRepeated( Type::Char, its, 0, Flag );
            message.CreateRepeated( Type::Char, its, 1, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberT[i], 0, i, Flag );
                message.StoreRepeated( mMemberTs[i], 1, i, Flag );
            }

            message.CreateRepeated( Type::WORD, its, 2, Flag );
            message.CreateRepeated( Type::WORD, its, 3, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberS[i], 2, i, Flag );
                message.StoreRepeated( mMemberSs[i], 3, i, Flag );
            }

            message.CreateRepeated( Type::DWORD, its, 4, Flag );
            message.CreateRepeated( Type::DWORD, its, 5, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberR[i], 4, i, Flag );
                message.StoreRepeated( mMemberRs[i], 5, i, Flag );
            }

            message.CreateRepeated( Type::QWORD, its, 6, Flag );
            message.CreateRepeated( Type::QWORD, its, 7, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberG[i], 6, i, Flag );
                message.StoreRepeated( mMemberGs[i], 7, i, Flag );
            }

            message.Store( name1, 8, Flag );
            message.Store( name2, 9, Flag );

            message.CreateRepeated( Type::DWORD, its, 10, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberF[i], 10, i, Flag );
            }

            message.CreateRepeated( Type::QWORD, its, 11, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( mMemberD[i], 11, i, Flag );
            }

        }

        void TestEqual( TestClass3 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( mMemberTs[i], c2.mMemberTs[i] );
                EXPECT_EQ( mMemberR[i], c2.mMemberR[i] );
                EXPECT_EQ( mMemberRs[i], c2.mMemberRs[i] );
                EXPECT_EQ( mMemberG[i], c2.mMemberG[i] );
                EXPECT_EQ( mMemberGs[i], c2.mMemberGs[i] );
                EXPECT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_EQ( mMemberSs[i], c2.mMemberSs[i] );
                EXPECT_FLOAT_EQ( mMemberF[i], c2.mMemberF[i] );
                EXPECT_DOUBLE_EQ( mMemberD[i], c2.mMemberD[i] );
                EXPECT_EQ( name1.compare( c2.name1 ), 0 );
                EXPECT_EQ( name2.compare( c2.name2 ), 0 );
            }
        }

        std::vector< uint8_t > mMemberT;
        std::vector< uint16_t > mMemberS;
        std::vector< uint32_t > mMemberR;
        std::vector< uint64_t > mMemberG;

        std::vector< int8_t > mMemberTs;
        std::vector< int16_t > mMemberSs;
        std::vector< int32_t > mMemberRs;
        std::vector< int64_t > mMemberGs;

        std::string name1;
        std::string name2;

        std::vector< float > mMemberF;
        std::vector< double > mMemberD;
    };

    template< typename T, uint32_t Flag = 0x00 >
    class Primitive
        : public ISerialisable
    {
    public:

        Primitive( const T &value )
            : mMember( value )
        {
        }

        void OnSerialise( Message &message )
        {
            message.Store( mMember, 1, Flag );
        }

        void TestEqual( Primitive &c2 )
        {
            EXPECT_EQ( mMember, c2.mMember );
        }

        T mMember;
    };

    template< typename TestClass, uint32_t its = 100, uint32_t Flag = 0x00, typename T = uint32_t >
    class TestClass4
        : public ISerialisable
    {
    public:

        TestClass4( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberTestClasses.push_back( TestClass( ( T )( ( double )rand() / RAND_MAX )*std::numeric_limits< T >::max() ) );
            }
        }

        void OnSerialise( Message &message )
        {
            message.CreateRepeated( Type::Message, its, 0, Flag );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreRepeated( &mMemberTestClasses[ i ], 0, i, 0 );
            }
        }

        void TestEqual( TestClass4 &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberTestClasses[ i ].TestEqual( c2.mMemberTestClasses[ i ] );
            }
        }

        std::vector< TestClass > mMemberTestClasses;
    };



    TestSerialClass( MultiSerialization, randomVals, uint8_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint16_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint32_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, uint64_t, 343422, 21331, 0x00, 100 );

    TestSerialClass( MultiSerialization, randomVals, int8_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int16_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int32_t, 343422, 21331, 0x00, 100 );
    TestSerialClass( MultiSerialization, randomVals, int64_t, 343422, 21331, 0x00, 100 );

    TestFSerialClass( MultiSerialization, randomVals, 343422, 21331, 0x00, 100 );
    TestDSerialClass( MultiSerialization, randomVals, 343422, 21331, 0x00, 100 );


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

    TestEasyRepeatedClass( EasyRepeated, randomVals, 343422, 21331, 0x00, 100 );
    TestEasyRepeatedClass( EasyRepeatedPacked, randomVals, 343422, 21331, 0x01, 100 );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, uint8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, uint16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, uint32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, uint64_t );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, int8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, int16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, int32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 0x00, 100, int64_t );

}