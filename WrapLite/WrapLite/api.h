#ifndef API_H
#define API_H
#include "session.h"
#include "query/query.h"

namespace wraplite {

	class sqlite_api final {
	public:
		sqlite_api() = delete;
		sqlite_api(const std::string& filepath);

		impl::sql::query operator<<(const std::string& sql);
		impl::sql::query operator<<(const char* sql);

		auto connection_object() const { return m_session->get_connection(); }
	private:
		std::shared_ptr<impl::database_session> m_session;
	};

}

#endif