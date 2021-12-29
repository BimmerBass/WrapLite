#ifndef OPTIONS_H
#define OPTIONS_H
#include <Legacy/sqlite3.h>

namespace wraplite::conversion_layer::options {

	namespace constants {

		enum class open_flags : int {
			// Mandatory
			READONLY = SQLITE_OPEN_READONLY,			/* Open in readonly mode. */
			READWRITE = SQLITE_OPEN_READWRITE,			/* Open in read-write mode. */
			CREATE = SQLITE_OPEN_CREATE,				/* Create database if the file doesn't already exist. */

			// Optional
			URI = SQLITE_OPEN_URI,						/* Interpret database filename as URI. */
			MEMORY = SQLITE_OPEN_MEMORY,				/* Open as in-memory database. */
			NOMUTEX = SQLITE_OPEN_NOMUTEX,				/* Open without mutex, which allows separate threads to access the database at the same time as long as they use different connections. */
			FULLMUTEX = SQLITE_OPEN_FULLMUTEX,			/* Serialized mode (thread-safe). Multiple threads can use the same connection to read and write. */
			SHAREDCACHE = SQLITE_OPEN_SHAREDCACHE,		/* Database is opened with shared cache. */
			PRIVATECACHE = SQLITE_OPEN_PRIVATECACHE,	/* Database is opened with private cache. */
			EXRESCODE = SQLITE_OPEN_EXRESCODE,			/* Use extended result codes. */
			NOFOLLOW = SQLITE_OPEN_NOFOLLOW				/* Database filename can't be a symbolic link. */
		};

		open_flags operator|(const open_flags& o1, const open_flags& o2) {
			return static_cast<open_flags>(
				static_cast<int>(o1) | static_cast<int>(o2)
				);
		}
		open_flags& operator|=(open_flags& o1, const open_flags& o2) {
			o1 = o1 | o2;
			return o1;
		}
	}


	struct database_options {
		// Flags for how the database should be opened. Default is set to match the legacy sqlite3_open and sqlite3_open16.
		constants::open_flags flags = constants::open_flags::CREATE | constants::open_flags::READWRITE;
		
		// Name of the sqlite3_vfs object.
		const char* vfs_object = NULL;

		void operator|(const constants::open_flags& of) {
			flags |= of;
		}
	};
}


#endif