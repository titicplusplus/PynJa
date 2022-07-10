#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>

#include <chrono>

//#define PY_SSIZE_T_CLEAN
//#include <Python.h>

#include <TitiCplusplus/pynja.hpp>

int main() {
	TitiCplusplus::PynJa::init();
	TitiCplusplus::PynJa reader;

	std::ifstream flux {"data.json"};

	nlohmann::json r;

	flux >> r;

	flux.close();


	//const std::string rf = reader.renderFile("./Example/", "index.thi");
	std::cout << reader.getResult("", "index.thi", r, true);
	//std::cout << ;

	TitiCplusplus::PynJa::end();

	return 0;
}
