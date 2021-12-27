#include "WrapLite/query/query.h"

#include <fstream>
#include <iostream>

int main() {

	using namespace wraplite;

	//std::fstream s("test.db");
	//s.close();

	try {
		database_session sess("test.db");

		sql::query q("create table hello_world (hello text, world text);", sess);
		q.execute({});

		//sql::query q1("insert into hello_world values (?, ?)", sess);
		//q1 << std::string("Hello my dear pt2");
		//q1 << std::string("Beloved world pt2");

		//q1.execute({});
		for (int i = 0; i < 10; i++) {
			sql::query q1("insert into hello_world values (?, ?)", sess);
			q1 << ("Hello my dear pt" + std::to_string(i));
			q1 << ("Beloved world pt" + std::to_string(i));

			q1.execute({});
		}


		std::vector<std::string> h;
		std::vector<std::string> w;
		sql::query q2("select * from hello_world;", sess);
		q2 >> [&](std::string s1, std::string s2) {h.push_back(s1); w.push_back(s2); };

		for (size_t i = 0; i < h.size(); i++) {
			std::cout << h[i] << ": " << w[i] << std::endl;
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...) { exit(1); }
}