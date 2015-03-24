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
#ifndef __SERIALISATION_MESSAGE_H__
#define __SERIALISATION_MESSAGE_H__

#include "interface/IMessage.h"

#include "internalMessage.h"
#include "streamBuffer.h"
#include "defines.h"
#include "types.h"
#include "messageAdapter.h"
#include "binarySerMessage.h"
#include "binaryDeserMessage.h"

#include <assert.h>
#include "messageHelper.h"


class Message
{
public:

    enum Flags
    {
        Packed = Internal::Flags::Packed
    };

    Message( const std::string &fileName, Format::Format format, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( fileName ),
          mInternalMessage( GetInternalMessage( format, mode ) ),
          mMode( mode ),
          mFormat( format )
    {
    }

    Message( std::iostream &stream, Format::Format format, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( stream ),
          mInternalMessage( GetInternalMessage( format, mode ) ),
          mMode( mode ),
          mFormat( format )
    {
    }

    ~Message()
    {
        ClearBuffers();
        DeleteInternalMessage( mInternalMessage );
    }

    void ClearBuffers()
    {
        mStreamBuffer.ClearReadBuffer();
        mStreamBuffer.FlushWriteBuffer();
    }

    void SetMode( Mode::Mode mode )
    {
        mMode = mode;

        DeleteInternalMessage( mInternalMessage );

        mInternalMessage = GetInternalMessage( mFormat, mMode );
    }

    Mode::Mode GetMode()
    {
        return mMode;
    }

    void SetFormat( Format::Format format )
    {
        mFormat = format;
    }

    Format::Format GetFormat()
    {
        return mFormat;
    }


    template< typename TSerialisable >
    void Store( TSerialisable &serialisable )
    {
        mInternalMessage->InitObject();

        MessageHelper::SERIALISATION_CUSTOM_INTERFACE( serialisable, *this );

        mInternalMessage->FinishObject();
    }

    void Store( ISerialisable &serialisable )
    {
        Store< ISerialisable >( serialisable );
    }

    template< typename TSerialisable >
    void Store( TSerialisable &serialisable, uint8_t index )
    {
        if ( mInternalMessage->InitObject( index ) )
        {
            MessageHelper::SERIALISATION_CUSTOM_INTERFACE( serialisable, *this );

            mInternalMessage->FinishObject( index );
        }
    }

    void Store( ISerialisable &serialisable, uint8_t index )
    {
        Store< ISerialisable >( serialisable, index );
    }

    void Store( std::string &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( uint8_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( uint16_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( uint32_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( uint64_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( int8_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( int16_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( int32_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    void Store( int64_t &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    virtual void Store( float &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    virtual void Store( double &value, uint8_t index )
    {
        mInternalMessage->Store( value, index );
    }

    size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 )
    {
        return mInternalMessage->CreateArray( type, size, index, flags );
    }

    template< typename TSerialisable >
    void StoreArrayItem( TSerialisable &value )
    {
        mInternalMessage->InitArrayObject();

        MessageHelper::SERIALISATION_CUSTOM_INTERFACE( value, *this );

        mInternalMessage->FinishArrayObject();
    }

    void StoreArrayItem( ISerialisable &value )
    {
        StoreArrayItem< ISerialisable >( value );
    }

    void StoreArrayItem( std::string &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( uint8_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( uint16_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( uint32_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( uint64_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( int8_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( int16_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( int32_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( int64_t &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( float &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    void StoreArrayItem( double &value )
    {
        mInternalMessage->StoreArrayItem( value );
    }

    template< typename TContainer >
    void StoreContainer( TContainer &container, uint8_t index, uint8_t flags = 0x00 )
    {
        const Type::Type type = static_cast< Type::Type >( Internal::Type::GetEnum< typename TContainer::value_type >() );
        const size_t size = CreateArray( type, container.size(), index, flags );
        container.resize( size );

        for ( typename TContainer::iterator it = container.begin(), end = container.end(); it != end; ++it )
        {
            StoreArrayItem( *it );
        }
    }

    void StoreContainer( std::vector< uint8_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< uint16_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< uint32_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< uint64_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< int8_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< int16_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< int32_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< int64_t > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< float > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

    void StoreContainer( std::vector< double > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        mInternalMessage->StoreContainer( container, index, flags );
    }

private:

    StreamBuffer< SERIALISERS_BUFFERSIZE > mStreamBuffer;

    InternalMessage *mInternalMessage;

    Mode::Mode mMode;
    Format::Format mFormat;

    InternalMessage *GetInternalMessage( Format::Format format, Mode::Mode mode )
    {
        InternalMessage *iMessage = NULL;

        switch ( format )
        {
        case Format::Binary:
            iMessage = mode == Mode::Serialise ? CreateInternalMessage< BinarySerMessage >() :
                       CreateInternalMessage< BinaryDeserMessage >();
            break;

        default:
            assert( false && "Something went terribly haywire..." );
            break;
        }

        return iMessage;
    }

    template< typename TMessage >
    InternalMessage *CreateInternalMessage()
    {
        return new MessageAdapter< TMessage >( mStreamBuffer );
    }

    template< typename TMessage >
    void DeleteInternalMessage( TMessage *message )
    {
        delete message;
    }
};

#endif
