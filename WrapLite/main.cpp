#include "WrapLite/api.h"

#include <fstream>
#include <iostream>

int main() {

	using namespace wraplite;

	//std::fstream s("test.db");
	//s.close();

	try {
		
		sqlite_api api_connection("test.db");

		int result = 0;

		api_connection << "create table hello_world (hello text, world text);";

		for (int i = 0; i < 100; i++) {
			api_connection << "insert into hello_world values (?, ?)" << "Hello my dear pt" + std::to_string(i) << "Beloved world pt" + std::to_string(i);
			std::cout << "INSERTED ROW " << i + 1 << std::endl;
		}

		api_connection << "select count(*) from hello_world;" >> result;

		std::cout << "ROWS: " << result << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}