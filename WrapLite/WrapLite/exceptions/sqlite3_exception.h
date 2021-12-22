#ifndef SQLITE3_EXCEPTION_H
#define SQLITE3_EXCEPTION_H
#include <exception>

namespace WrapLite::Exceptions {

	class sqlite_exception : public std::exception {
	public:
		sqlite_exception(const char* what) : std::exception(what) {

		}

	};

}

#endif