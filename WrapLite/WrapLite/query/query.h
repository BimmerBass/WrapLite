#ifndef QUERY_H
#define QUERY_H
#include <vector>
#include <tuple>
#include <functional>

#include <ConversionLayer/types.h>
#include "../session.h"


namespace wraplite::sql {
	namespace types = conversion_layer::types;

	/// <summary>
	/// A wrapper for the sqlite3_stmt
	/// </summary>
	class query {
	public:
		query() = delete; // No default construction.
		query(const std::string& query_str, std::shared_ptr<database_session> session_ptr);

		// The destructor is allowed to throw exceptions.
		~query() noexcept(false);

		// Stream operators for inserting data.
		template<typename T> requires types::sql_integral<T>
		query& operator<<(const T& integer) {
			conversion_layer::bind<T>(m_statement, next_idx(), integer);
			return *this;
		}

		template<typename T> requires types::sql_decimal<T>
		query& operator<<(const T& decimal) {
			conversion_layer::bind<T>(m_statement, next_idx(), decimal);
			return *this;
		}

		template<typename T> requires types::sql_text<T>
		query& operator<<(const T& str) {
			conversion_layer::bind<T>(m_statement, next_idx(), str);
			return *this;
		}

		template<typename T, typename _Alloc> requires types::sql_general_type<T>
		query& operator<<(const std::vector<T, _Alloc>& data) {
			conversion_layer::bind<T, _Alloc>(m_statement, next_idx(), data);
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
		types::statement_t m_statement;

		// Whether or not the query has run.
		bool has_run;

		// The current index we're at in the statement parameters.
		size_t current_idx;

		size_t next_idx() {
			return ++current_idx;
		}
	};

}



#endif