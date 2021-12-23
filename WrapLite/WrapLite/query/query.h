#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <tuple>
#include <functional>

#include "../session.h"
#include "../types.h"
#include "../exceptions/sqlite3_exception.h"

namespace wraplite::sql {

	using stmt_t = std::unique_ptr<sqlite3_stmt, decltype([=](sqlite3_stmt* st) {sqlite3_finalize(st); })>;

	/// <summary>
	/// A wrapper for the sqlite3_stmt
	/// </summary>
	class query {
	public:
		query() = delete; // No default construction.
		query(const std::string& query_str);

		// The destructor is allowed to throw exceptions.
		~query() noexcept(false);

		// Stream operators for inserting data.
		template<typename T> requires types::sql_integral<T>
		query& operator<<(const T& integer) {
			if constexpr (sizeof(T) > 4) {
				// 64-bits.
				auto insert_val = static_cast<sqlite3_int64>(integer);
				if (int result = sqlite3_bind_int64(m_statement.get(), next_idx(), insert_val); result != SQLITE_OK) {
					throw exceptions::sqlite_exception((sqlite3*)m_session.get(), "sqlite3_bind_int64");
				}
			}
			else {
				// 32-bits.
				auto insert_val = static_cast<int>(integer);
				if (int result = sqlite3_bind_int(m_statement.get(), next_idx(), insert_val); result != SQLITE_OK) {
					throw exceptions::sqlite_exception((sqlite3*)m_session.get(), "sqlite3_bind_int");
				}
			}
			return *this;
		}

		template<typename T> requires types::sql_decimal<T>
		query& operator<<(const T& decimal) {
			// Convert to double and bind.
			auto insert_val = static_cast<double>(decimal);
			if (int result = sqlite3_bind_double(m_statement.get(), next_idx(), insert_val); result != SQLITE_OK) {
				throw exceptions::sqlite_exception((sqlite3*)m_session.get(), "sqlite3_bind_double");
			}

			return *this;
		}

		template<typename T> requires types::sql_text<T>
		query& operator<<(const T& str) {
			if (int result = sqlite3_bind_text(m_statement.get(), next_idx(), str.c_str(), -1, SQLITE_TRANSIENT); result != SQLITE_OK) {
				throw exceptions::sqlite_exception((sqlite3*)m_session.get(), "sqlite3_bind_text");
			}
			return *this;
		}

		template<typename T, typename _Alloc> requires types::sql_general_type<T>
		query& operator<<(const std::vector<T, _Alloc>& data) {
			// Reinterpret the vector.
			int blob_size = data.size() * sizeof(T);
			auto blob_data = static_cast<const void*>(data.data());

			if (int result = sqlite3_bind_blob(m_statement.get(), next_idx(), blob_data, blob_size, SQLITE_TRANSIENT); result != SQLITE_OK) {
				throw exceptions::sqlite_exception((sqlite3*)m_session.get(), "sqlite3_bind_blob");
			}
			return *this;
		}


		// Operators for result extraction.
		template<typename T> requires types::sql_integral<T>
			void operator>>(T& out_val);
		
		template<typename ... args> requires types::sql_general_type<args...>
		void operator>>(std::tuple<args...>& out_vals);

		template<typename ... args> requires types::sql_general_type<args...>
		void operator>>(std::function<void(args...)>& func);

		// Public member methods.
		void execute();
	private:
		// The session that allows us to communicate with the database.
		std::shared_ptr<database_session> m_session;

		// The statement itself.
		stmt_t m_statement;

		// Whether or not the query has run.
		bool has_run;

		// The current index we're at in the statement parameters.
		size_t current_idx = 0;

		size_t next_idx() {
			return ++current_idx;
		}
	};

}



#endif