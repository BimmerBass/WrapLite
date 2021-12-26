#ifndef SESSION_H
#define SESSION_H
#include <string>

#include <ConversionLayer/conversion.h>

namespace wraplite {
	using session_t = conversion_layer::types::session_t;

	/// <summary>
	/// Wrapper for the sqlite3* object used to establish and break the connection to the database.
	/// </summary>
	class database_session final {
	public:
		// No default construction
		database_session() = delete;
		database_session(const std::string& filepath);

		operator sqlite3* () const {
			return m_session.get();
		}
	private:
		session_t m_session = nullptr;
	};
}

#endif