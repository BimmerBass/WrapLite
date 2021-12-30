#include "WrapLite/api.h"

#include <fstream>
#include <iostream>

int main() {

	using namespace wraplite;

	//std::ofstream s("test.db");
	//s.close();

	try {
		
		sqlite_api api_connection("test.db");

		//int result = 0;
		//
		//api_connection << "create table hello_world (hello text, world text);";
		//
		//for (int i = 0; i < 100; i++) {
		//	api_connection << "insert into hello_world values (?, ?)" << "Hello my dear pt" + std::to_string(i) << "Beloved world pt" + std::to_string(i);
		//	std::cout << "INSERTED ROW " << i + 1 << std::endl;
		//}
		//
		//api_connection << "select count(*) from hello_world;" >> result;
		//
		//std::cout << "ROWS: " << result << std::endl;

		api_connection << "begin;";

		//for (int i = 0; i < 10; i++) {
		//	api_connection << "create table tbl" + std::to_string(i + 1) + " (col1 text, col2 text);";
		//}


		std::string y1 = "";
		std::string y2 = "";

		api_connection << "select * from tbl2 limit 1;" >> std::tie(y1, y2);
		
		for (int i = 0; i < 100; i++) {
			api_connection << "insert into tbl2 values (?, ?);" << "Test1" << "Test2";
		}

		std::string column_one = "";
		std::string column_two = "";

		api_connection << "select * from tbl2 limit 1;" >> std::tie(column_one, column_two);

		std::cout << "Before commit: " << std::endl;
		std::cout << "1) " << column_one << std::endl;
		std::cout << "2) " << column_two << std::endl;

		api_connection << "commit;";

		std::cout << "After commit: " << std::endl;
		std::cout << "1) " << column_one << std::endl;
		std::cout << "2) " << column_two << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}