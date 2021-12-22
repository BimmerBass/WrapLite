#ifndef SESSION_H
#define SESSION_H
#include <memory>
#include <string>

#include <Legacy/sqlite3.h>

namespace WrapLite {
	using session_t = std::shared_ptr<sqlite3>;

	/// <summary>
	/// Wrapper for the sqlite3* object used to establish and break the connection to the database.
	/// </summary>
	class DatabaseSession {
	public:
		// No default construction
		DatabaseSession() = delete;
		DatabaseSession(const std::string& filepath);

		operator sqlite3* () const {
			return m_session.get();
		}
	private:
		session_t m_session = nullptr;
	};
}

#endif