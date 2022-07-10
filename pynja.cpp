#include "pynja.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace TitiCplusplus {

void PynJa::init() {Py_Initialize();}

void PynJa::end() {Py_Finalize();}


PynJa::PynJa(const std::string tab, const std::string vName) : m_tab {tab}, m_vName {vName} {}

PynJa::~PynJa() {}

std::string PynJa::renderFile(const std::string working_directory, const char *filePath, bool respectInd) {
	std::string result {""};
	std::string line {""};

	std::ifstream flux { working_directory + filePath };

	if (!flux) {
		flux.close();
		return m_vName + " += f\"No file found at: " + working_directory + filePath + "\"" + "\n";
	}

	int nbrTab {0};

	std::string currentTab = "";

	while (std::getline(flux, line)) {
		LT l {LT::LNONE};

		long i = getTypeLine(l, line);

		if (i == -1) {
			if (respectInd)
				line = removeExpand(line, currentTab);
			if (line.size() && line[line.size() - 1] == '\\') {
				result += currentTab + m_vName + " += f\"" + line.substr(0, line.size() - 1) + "\"" + "\n";
			} else {
				result += currentTab + m_vName + " += f\"" + line + "\\n\"" + "\n";
			}
		} else if (l == LT::MULTI_LINES) {
			result += currentTab + "\n";

			while (std::getline(flux, line)) {
				long i = getTypeLine(l, line);

				if (l == LT::MULTI_LINES) {
					break;
				}

				result += currentTab + line + "\n";
			}
		} else if (l == LT::LINE) {
			const std::string aa {line.substr(i)};
			if (aa.size() > 2) {
				if (aa.substr(0, 3) == "end") {
					nbrTab--;
					currentTab = updateIndentation(nbrTab);

					if (nbrTab < 0) {
						std::cerr << "ERROR: indentation" << "\n";
					}
				} else if (aa.size() > 4 && aa.substr(0, 5) == "else:") {
					nbrTab--;
					currentTab = updateIndentation(nbrTab);
					result +=  currentTab + aa + '\n';
				} else {
					result +=  currentTab + aa + '\n';
				}
			} else {
				result += currentTab +  aa + '\n';
			}

			if (aa[aa.size() - 1] == ':') {
				nbrTab++;
				currentTab = updateIndentation(nbrTab);
			}
		} else if (l == LT::INCLUDE_F) {
			PynJa cr;

			const std::string rline = line.substr(i+2);

			auto it = rline.rfind("/");

			if (it == std::string::npos) {
				result += cr.renderFile(working_directory, rline.c_str());
			} else {
				result += cr.renderFile(working_directory + rline.substr(0, it+1), rline.substr(it+1).c_str());
			}
		}
	}

	flux.close();

	return result;
}

std::string PynJa::getResult(const std::string working_directory, const char *filePath, const nlohmann::json &param, bool respectInd) {
	const std::string result = renderFile(working_directory, filePath, respectInd);
    	
	PyObject *m_pGlobal = PyModule_GetDict(PyImport_AddModule("__main__")); 
	PyObject *m_pLocal = setParameterObj(param);

	PyDict_SetItemString(m_pLocal, m_vName.c_str(), PyUnicode_FromString(""));

	PyObject *pstr;
	if (!(pstr = 
        	PyRun_String(result.c_str(),
            	Py_file_input, m_pGlobal, m_pLocal))) {
            	PyErr_Print();
        	return "The python code is:\n" + result;
    	}

	const std::string rr = PyUnicode_AsUTF8(PyDict_GetItemString(m_pLocal, m_vName.c_str()));
		
	Py_DECREF(m_pLocal);
	Py_DECREF(m_pGlobal);

	return rr;
}

PyObject* PynJa::setParameterObj(const nlohmann::json &param) {
	PyObject *pLocal = PyDict_New();
	for (const auto [key, value] : param.items()) {
		if (value.is_string()) {
			PyDict_SetItemString(pLocal, key.c_str(), PyUnicode_FromString(value.get<std::string>().c_str()));
		} else if (value.is_number_integer()) {
			PyDict_SetItemString(pLocal, key.c_str(), PyLong_FromLong(value.get<long>()));
		} else if (value.is_number_float()) {
			PyDict_SetItemString(pLocal, key.c_str(), PyFloat_FromDouble(value.get<double>()));
		} else if (value.is_boolean()) {
			PyDict_SetItemString(pLocal, key.c_str(), PyBool_FromLong(value.get<bool>()));
		} else if (value.is_array()) {
			PyDict_SetItemString(pLocal, key.c_str(), setParameterLst(value));
		} else if (value.is_object()) {
			PyDict_SetItemString(pLocal, key.c_str(), setParameterObj(value));
		} else if (value.is_number_unsigned()) {
			PyDict_SetItemString(pLocal, key.c_str(), PyLong_FromUnsignedLong(value.get<unsigned long>()));
		} else {
			std::cout << "The type of " << key << " is not suppported\n";
		}
	}

	return pLocal;
}

PyObject* PynJa::setParameterLst(const nlohmann::json &param) {
	PyObject *pLocal = PyList_New(param.size());
	for (std::size_t i {0}; i < param.size(); i++) {
		const nlohmann::json &value = param[i];
		if (value.is_string()) {
			PyList_SetItem(pLocal, i, PyUnicode_FromString(value.get<std::string>().c_str()));
		} else if (value.is_number_integer()) {
			PyList_SetItem(pLocal, i, PyLong_FromLong(value.get<long>()));
		} else if (value.is_number_float()) {
			PyList_SetItem(pLocal, i, PyFloat_FromDouble(value.get<double>()));
		} else if (value.is_boolean()) {
			PyList_SetItem(pLocal, i, PyBool_FromLong(value.get<bool>()));
		} else if (value.is_array()) {
			//PyObject* lst = PyList_New(value.size());

			//setParameterLst(lst, value);
			PyList_SetItem(pLocal, i, setParameterLst(value));
		} else if (value.is_object()) {
			PyList_SetItem(pLocal, i, setParameterObj(value));
		} else if (value.is_number_unsigned()) {
			PyList_SetItem(pLocal, i, PyLong_FromUnsignedLong(value.get<unsigned long>()));
		} else {
			std::cout << "The type at " << i << " is not suppported\n";
		}
	}

	return pLocal;
}

long PynJa::getTypeLine(LT &l, const std::string &line) {
	for (std::size_t i {0}; i < line.size(); ++i) {
		if (line[i] == '$') {
			if (i+2 < line.size() && line[i+1] == '$') {
			       	if (line[i+2] == '$') {
					l = LT::MULTI_LINES;
					return i+2;
				} 

				l = LT::INCLUDE_F;
				return i+1;
			} /**else if (i+1 < line.size() && line[i+1] == '{' && line.substr(i+1).find('}') != std::string::npos) {
				l = LT::VAL;
				return i+2;
			}**/ else {
				l = LT::LINE;
				return i+1;
			}
		}
	}

	l = LT::LNONE;
	return -1;
}

std::string PynJa::updateIndentation(int nbrTab) {
	if (nbrTab < 0) {
		std::cerr << "ERROR INDENTATION" << std::endl;
		return "";
	}
	std::string result;
       	//result.resize(nbrTab*m_tab.size());

	for (int i = 0; i < nbrTab; ++i) {
		result.insert(result.begin() + i, m_tab.begin(), m_tab.end());
	}

	return result;
}

std::string PynJa::removeExpand(std::string line, const std::string &currentTab) {
	if (line.substr(0, currentTab.size()) == currentTab) {
		return line.substr(currentTab.size());
	}

	std::cerr << "ERROR: String doesn't respect indentation: " << line << "\n";

	return line;
}

}; // namespace TitiCplusplus
