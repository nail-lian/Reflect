#pragma once

#include <map>

#include "Platform/Types.h"

#include "Foundation/SmartPtr.h"

#include "Reflect/API.h"

//
// Obviously the reflection system itself can't use the same type checking as the code
//  its reflecting upon, so define a simple type checking system just for the reflection classes
//

#define REFLECT_META_BASE(__Id, __Type) \
	typedef __Type This; \
	const static int s_MetaId = __Id; \
	virtual int GetReflectionType() const { return __Id; } \
	virtual bool HasReflectionType(int id) const { return __Id == id; }

#define REFLECT_META_DERIVED(__Id, __Type, __Base) \
	typedef __Type This; \
	typedef __Base Base; \
	const static int s_MetaId = __Id; \
	virtual int GetReflectionType() const HELIUM_OVERRIDE { return __Id; } \
	virtual bool HasReflectionType(int id) const HELIUM_OVERRIDE { return __Id == id || Base::HasReflectionType(id); }

namespace Helium
{
	namespace Reflect
	{
		//
		// All types have to belong to this enum
		//

		namespace MetaIds
		{
			enum MetaId
			{
				Invalid = -1,

				// type meta-data class hierarchy
				MetaType,
					MetaEnum,
					MetaStruct,
						MetaClass,
				
				// data abstraction class hierarchy
				Translator,
					ScalarTranslator,
					StructureTranslator,
					PointerTranslator,
					EnumerationTranslator,
					TypeTranslator,
					ContainerTranslator,
						SetTranslator,
						SequenceTranslator,
						AssociationTranslator,

				Count,
			};

			extern const char* Strings[ MetaIds::Count ];
		}
		typedef MetaIds::MetaId MetaId;

		//
		// A block of string-based properties
		//

		class HELIUM_REFLECT_API PropertyCollection
		{
		protected:
			mutable std::map< std::string, std::string > m_Properties;

		public:
			template<class T>
			inline void SetProperty( const std::string& key, const T& value ) const;

			template<class T>
			inline bool GetProperty( const std::string& key, T& value ) const;

			inline const std::string& GetProperty( const std::string& key ) const;
		};

		//
		// This lets us safely cast between reflection class pointers
		//

		class HELIUM_REFLECT_API Meta : public Helium::AtomicRefCountBase< Meta >, public PropertyCollection
		{
		public:
			REFLECT_META_BASE( MetaIds::Invalid, Meta );

			Meta();
			virtual ~Meta();
		};

		template<typename T>
		T* ReflectionCast(Meta* info)
		{
			return (info && info->HasReflectionType( T::s_MetaId )) ? static_cast<T*>(info) : NULL;
		}

		template<typename T>
		const T* ReflectionCast(const Meta* info)
		{
			return (info && info->HasReflectionType( T::s_MetaId )) ? static_cast<const T*>(info) : NULL;
		}
	}
}

#include "Reflect/Meta.inl"