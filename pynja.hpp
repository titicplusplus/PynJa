#pragma once

#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace TitiCplusplus {

enum LT : char {
	LINE = 0,
	MULTI_LINES = 1,
	VAL = 2,
	LNONE = 3,
	INCLUDE_F = 4,
};

class PynJa {
	const std::string m_tab = "\t";
	const std::string m_vName {"routput"};

	public:
		PynJa(const std::string tab = "\t", const std::string vName = "routput");
		~PynJa();

		static void init();
		static void end();

		std::string renderFile(const std::string working_directory, const char *filePath, bool respectInd = true);
		std::string getResult(const std::string working_directory, const char *filePath, const nlohmann::json &param, bool respectInd = true);
		std::string execute(const std::string m_code, const nlohmann::json &param);

		long getTypeLine(LT &l, const std::string &line);

		PyObject *setParameterObj(const nlohmann::json &param);
		PyObject *setParameterLst(const nlohmann::json &param);

		std::string updateIndentation(int nbrTab);

		PyObject *jsonToElement(const nlohmann::json &value);

		std::string removeExpand(std::string line, const std::string &currentTab);
};

}; // namespace TitiCplusplus
