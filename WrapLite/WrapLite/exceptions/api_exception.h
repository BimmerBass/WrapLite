#ifndef API_EXCEPTION_H
#define API_EXCEPTION_H
#include <exception>

namespace WrapLite::Exceptions {

	class api_exception : public std::exception {
	public:
		api_exception(const char* what) : std::exception(what) {
			
		}

	};

}

#endif