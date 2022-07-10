#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include <TitiCplusplus/pynja.hpp>

int main() {
	TitiCplusplus::PynJa::init();

	nlohmann::json r = {
		{"First", "Hi! EveryBody"},
		{"ex_dict", {
			{"r", 3},
			{"k", 4},
		}},
		{"lst", {"a", 8, 65, true}},
	};

	TitiCplusplus::PynJa reader;

	std::cout << reader.getResult("", "index.thi", r, true);

	TitiCplusplus::PynJa::end();

	return 0;
}
