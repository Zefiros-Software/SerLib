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

#include "serialisation/serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define TestReorderedPrimitive( test, name, seed1, seed2, type1, type2 )  \
    TEST( P(test), type1 ## type2 ## name )                               \
    {                                                                     \
        NormalPrimitive< type1, type2 > c1( seed1 );                      \
        ReorderedPrimitive< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseStream( c1, c2 );                       \
        c1.TestEqual( c2 );                                               \
    }

#define TestReorderedObjectClass( test, name, seed1, seed2, type1, type2 )  \
    TEST( P(test), type1 ## type2 ## name )                                 \
    {                                                                       \
        NormalObjectClass< type1, type2 > c1( seed1 );                      \
        ReorderedObjectClass< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseStream( c1, c2 );                         \
        c1.TestEqual( c2 );                                                 \
    }

#define TestReorderedRepeatedPrimitive( test, name, seed1, seed2, type1, type2, its ) \
    TEST( P( test ), type1 ## type2 ## name )                                         \
    {                                                                                 \
        NormalRepeatedPrimitive< type1, type2, its > c1( seed1 );                     \
        ReorderedRepeatedPrimitive< type1, type2, its > c2( seed2 );                  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                   \
        c1.TestEqual( c2 );                                                           \
    }

#define TestReorderedRepeatedObjectClass( test, name, seed1, seed2, type1, type2, its ) \
    TEST( P( test ), type1 ## type2 ## name )                                           \
    {                                                                                   \
        NormalRepeatedObjectClass< type1, type2, its > c1( seed1 );                     \
        ReorderedRepeatedObjectClass< type1, type2, its > c2( seed2 );                  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                     \
        c1.TestEqual( c2 );                                                             \
    }

namespace TestClasses
{
    template< typename T1, typename T2 >
    class NormalPrimitive
        : public ISerialisable
    {
    public:

        NormalPrimitive( uint32_t seed = 233232 )
        {
            std::srand( seed );

            mMemberT1 = GetRandom< T1 >();
            mMemberT2 = GetRandom< T2 >();
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMemberT1, 0 );
            message.Store( mMemberT2, 1 );
        }

        void TestEqual( NormalPrimitive< T1, T2 > &c2 )
        {
            ASSERT_EQ( mMemberT1, c2.mMemberT1 );
            ASSERT_EQ( mMemberT2, c2.mMemberT2 );
        }

    protected:

        T1 mMemberT1;
        T2 mMemberT2;
    };

    template< typename T1, typename T2 >
    class ReorderedPrimitive
        : public NormalPrimitive< T1, T2 >
    {
    public:

        ReorderedPrimitive( uint32_t seed = 233232 )
            : NormalPrimitive< T1, T2 >( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( NormalPrimitive< T1, T2 >::mMemberT2, 1 );
            message.Store( NormalPrimitive< T1, T2 >::mMemberT1, 0 );
        }
    };

    template< typename T1, typename T2 >
    class NormalObjectClass
        : public ISerialisable
    {
    public:

        NormalObjectClass( uint32_t seed = 233232 )
            : mMember1( seed ), mMember2( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.Store( mMember2, 1 );
        }

        void TestEqual( NormalObjectClass< T1, T2 > &c2 )
        {
            mMember1.TestEqual( c2.mMember1 );
            mMember2.TestEqual( c2.mMember2 );
        }

    protected:

        NormalPrimitive< T1, T2 > mMember1;
        ReorderedPrimitive< T2, T1 > mMember2;
    };

    template< typename T1, typename T2 >
    class ReorderedObjectClass
        : public NormalObjectClass< T1, T2 >
    {
    public:

        ReorderedObjectClass( uint32_t seed = 233232 )
            : NormalObjectClass< T1, T2 >( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( NormalObjectClass< T1, T2 >::mMember2, 1 );
            message.Store( NormalObjectClass< T1, T2 >::mMember1, 0 );
        }
    };

    template< typename T1, typename T2, uint32_t its >
    class NormalRepeatedPrimitive
        : public ISerialisable
    {
    public:

        NormalRepeatedPrimitive( uint32_t seed = 233232 )
        {
            std::srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMembers1.push_back( GetRandom< T1 >() );
                mMembers2.push_back( GetRandom< T2 >() );
            }
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.StoreContainer( mMembers1, 0 );
            message.StoreContainer( mMembers2, 1 );
        }

        void TestEqual( NormalRepeatedPrimitive< T1, T2, its > &c2 )
        {
            std::vector< T1 > &cMembers1 = c2.mMembers1;
            std::vector< T2 > &cMembers2 = c2.mMembers2;

            for ( uint32_t i = 0; i < its; ++i )
            {
                ASSERT_EQ( mMembers1[ i ], cMembers1[ i ] );
            }

            for ( uint32_t i = 0; i < its; ++i )
            {
                ASSERT_EQ( mMembers2[ i ], cMembers2[ i ] );
            }
        }

    protected:

        std::vector< T1 > mMembers1;
        std::vector< T2 > mMembers2;
    };

    template< typename T1, typename T2, uint32_t its >
    class ReorderedRepeatedPrimitive
        : public NormalRepeatedPrimitive< T1, T2, its >
    {
    public:

        ReorderedRepeatedPrimitive( uint32_t seed = 233232 )
            : NormalRepeatedPrimitive< T1, T2, its >( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            NormalRepeatedPrimitive< T1, T2, its >::mMembers2.resize( message.CreateArray( static_cast< Type::Type >
                    ( Internal::Type::GetEnum< T2 >() ), NormalRepeatedPrimitive< T1, T2, its >::mMembers2.size(), 1 ) );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( NormalRepeatedPrimitive< T1, T2, its >::mMembers2[ i ] );
            }

            NormalRepeatedPrimitive< T1, T2, its >::mMembers1.resize( message.CreateArray( static_cast< Type::Type >
                    ( Internal::Type::GetEnum< T1 >() ), NormalRepeatedPrimitive< T1, T2, its >::mMembers1.size(), 0 ) );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( NormalRepeatedPrimitive< T1, T2, its >::mMembers1[ i ] );
            }
        }
    };

    template< typename T1, typename T2, uint32_t its >
    class NormalRepeatedObjectClass
        : public ISerialisable
    {
    public:

        NormalRepeatedObjectClass( uint32_t seed = 233232 )
        {
            std::srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMember1.push_back( NormalPrimitive< T1, T2 >( std::rand() ) );
                mMember2.push_back( NormalObjectClass< T1, T2 >( std::rand() ) );
            }
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            mMember1.resize( message.CreateArray( Type::Object, mMember1.size(), 0 ) );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMember1[ i ] );
            }

            mMember2.resize( message.CreateArray( Type::Object, mMember2.size(), 1 ) );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMember2[ i ] );
            }
        }

        void TestEqual( NormalRepeatedObjectClass< T1, T2, its > &c2 )
        {
            std::vector< NormalPrimitive< T1, T2 > > &cMember1 = c2.mMember1;
            std::vector< NormalObjectClass< T1, T2 > > &cMember2 = c2.mMember2;

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMember1[ i ].TestEqual( cMember1[ i ] );
            }

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMember2[ i ].TestEqual( cMember2[ i ] );
            }
        }

    protected:

        std::vector< NormalPrimitive< T1, T2 > > mMember1;
        std::vector< NormalObjectClass< T1, T2 > > mMember2;
    };



    template< typename T1, typename T2, uint32_t its >
    class ReorderedRepeatedObjectClass
        : public NormalRepeatedObjectClass< T1, T2, its >
    {
    public:

        ReorderedRepeatedObjectClass( uint32_t seed = 233232 )
            : NormalRepeatedObjectClass< T1, T2, its >( seed )
        {
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.StoreContainer( NormalRepeatedObjectClass< T1, T2, its >::mMember2, 1 );
            message.StoreContainer( NormalRepeatedObjectClass< T1, T2, its >::mMember1, 0 );
        }
    };

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int64_t );



    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int64_t );



    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, uint8_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, uint16_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, uint32_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, uint64_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, int8_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, int16_t, 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint8_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint16_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint32_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, uint64_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int8_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int16_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int32_t, int64_t , 100 );

    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, uint8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, uint16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, uint32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, uint64_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, int8_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, int16_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, int32_t , 100 );
    TestReorderedRepeatedPrimitive( ReorderedRepeatedPrimitive, randomVals, 343422, 21331, int64_t, int64_t , 100 );



    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, uint8_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, uint16_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, uint32_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, uint64_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, int8_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, int16_t, 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint8_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint16_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint32_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, uint64_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int8_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int16_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int32_t, int64_t , 100 );

    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, uint8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, uint16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, uint32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, uint64_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, int8_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, int16_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, int32_t , 100 );
    TestReorderedRepeatedObjectClass( ReorderedRepeatedObjectClass, randomVals, 343422, 21331, int64_t, int64_t , 100 );
}