#include "binaryDeserialiser.h"
#include "varint.h"
#include "util.h"
#include "message.h"

BinaryDeserialiser::BinaryDeserialiser( std::istream &stream ) : mStream( &stream )
{

}

void BinaryDeserialiser::DeserialiseMessage( Message &message )
{
	Mode::Mode tempMode = message.GetMode();

    message.SetMode( Mode::Serialise );

    for ( uint32_t i = 0, end = ( uint32_t )ReadVarInt(); i < end; ++i )
    {
        uint64_t header = ReadVarInt();

        Internal::Type::Type type = Util::GetHeaderType( header );
        uint32_t index = Util::GetHeaderIndex( header );

        Deserialise( message, index, type );
    }

	message.SetMode( tempMode );
}

void BinaryDeserialiser::Deserialise( Message &message, const uint32_t index, const Internal::Type::Type type )
{
    switch ( type )
    {
    case Internal::Type::Repeated:
        DeserialiseRepeated( message, index );
        break;

    case Internal::Type::Variable:
        {
            Message *const subMessage = static_cast< Message * >( AbstractDeserialiser::GetFromMessage< Message >( message, index ) );
            DeserialiseMessage( *subMessage );
        }
        break;

    case Internal::Type::String:
        {
            std::string str;
			ReadString( str );
            message.Store( str, index, 0 );
        }
        break;

    case Internal::Type::UInt8:
        DeserialiseNum< uint8_t >( message, index );
        break;

    case Internal::Type::UInt16:
        DeserialiseNum< uint16_t >( message, index );
        break;

    case Internal::Type::UInt32:
        DeserialiseNum< uint32_t >( message, index );
        break;

    case Internal::Type::UInt64:
        DeserialiseNum< uint64_t >( message, index );
        break;

    case Internal::Type::VarInt:
        {
            uint64_t value = ReadVarInt();
            message.Store( value, index, ( uint32_t )Message::Packed );
        }
        break;

    default:
        break;
    }
}

void BinaryDeserialiser::DeserialiseRepeated( Message &message, const uint32_t index )
{
    uint64_t header = ReadVarInt();
    Internal::Type::Type type = Util::GetHeaderType( header );
    uint32_t size = Util::GetHeaderIndex( header );

    uint32_t flags = type == Internal::Type::VarInt ? ( uint32_t )Message::Packed : 0;

    message.CreateRepeated( ( Type::Type )type, size, index, flags );

    switch ( type )
    {
    case Internal::Type::Variable:
        {
            RepeatedData< Message > *const repeated =
                static_cast< RepeatedData< Message > * >(
                    AbstractDeserialiser::GetFromMessage < RepeatedData< Message > >( message, index ) );

            for ( uint32_t i = 0; i < size; ++i )
            {
                DeserialiseMessage( *repeated->GetConcreteSerialisable( i ) );
            }
        }
        break;

    case Internal::Type::String:
        {
            std::string str;

            for ( uint32_t i = 0; i < size; ++i )
            {
                ReadString( str );
                message.StoreRepeated( str, index, i );
            }
        }
        break;

    case Internal::Type::UInt8:
        DeserialiseRepeatedNum< uint8_t >( message, index, size );
        break;

    case Internal::Type::UInt16:
        DeserialiseRepeatedNum< uint16_t >( message, index, size );
        break;

    case Internal::Type::UInt32:
        DeserialiseRepeatedNum< uint32_t >( message, index, size );
        break;

    case Internal::Type::UInt64:
        DeserialiseRepeatedNum< uint64_t >( message, index, size );
        break;

    case Internal::Type::VarInt:
        {
            for ( uint32_t i = 0; i < size; ++i )
			{
				uint64_t value = ReadVarInt();
                message.StoreRepeated( value, index, i );
            }
        }
        break;

    default:
        break;
    }
}

void BinaryDeserialiser::ReadString( std::string &str )
{
	size_t vSize = ( size_t )ReadVarInt();

	str.resize( vSize );
	ReadBytes( &*str.begin(), vSize );
}

uint64_t BinaryDeserialiser::ReadVarInt()
{
	VarInt< uint64_t > vi;
	vi.ReadFromStream( *mStream );
	return vi.GetValue();
}
