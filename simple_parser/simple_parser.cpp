#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <exception>

class MyExceptionNotOpenFile : public std::exception {
private:
	std::string msg_;
public:
	explicit MyExceptionNotOpenFile(const char* messege) : msg_(messege) {};
	explicit MyExceptionNotOpenFile(std::string  messege) : msg_(std::move(messege)) {};
	virtual ~MyExceptionNotOpenFile() {};
	[[nodiscard]] const char* what() const noexcept override {
		return msg_.c_str();
	}
};

class MyException : public std::exception {
private:
	std::string msg_;
public:
	explicit MyException(const char* messege) : msg_(messege) {};
	explicit MyException(const std::string& messege) : msg_(messege) {};
	virtual ~MyException() {};
	[[nodiscard]] const char* what() const noexcept override {
		return msg_.c_str();
	}
};

class MyParsser {
private:
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> section_storage_;
public:
	MyParsser() = delete;
	MyParsser(const MyParsser& other) = delete;
	MyParsser& operator= (const MyParsser& other) = delete;
	virtual ~MyParsser() = default;
	explicit MyParsser(const std::string& name_file) {
		std::fstream in{ name_file };
		if (!in.is_open()) {
			throw MyExceptionNotOpenFile{ "The file was not opened" };
		}
		std::string section_name;
		std::vector<std::pair<std::string, std::string>> name_value_data_value;
		bool flag_entering_data = false;
		while (!in.eof()) {
			std::string str_get_line;
			in >> str_get_line[0];
			if (str_get_line[0] == '[') {
				if (flag_entering_data) {
					section_storage_[section_name] = std::move(name_value_data_value);
				}
				std::getline(in, str_get_line);
				section_name.assign(str_get_line.begin(), std::prev(str_get_line.end()));
				section_storage_[section_name];
				str_get_line.clear();
			}
			else if (str_get_line[0] == ';') {
				std::getline(in, str_get_line);
				continue;
			}
			else {
				std::string name_value;
				std::string var_value;
                auto var_char = str_get_line[0];
				std::getline(in, str_get_line);
                str_get_line.insert(str_get_line.begin(), var_char);
				auto iter_equal = std::find(str_get_line.begin(), str_get_line.end(), '=');
				name_value.assign(str_get_line.begin(), iter_equal);
				var_value.assign(std::next(iter_equal), str_get_line.end());
				name_value.erase(std::remove(name_value.begin(), name_value.end(), ' '), name_value.end());
				var_value.erase(std::find(var_value.begin(), var_value.end(), ';'), var_value.end());
				name_value_data_value.emplace_back(name_value,var_value);
				flag_entering_data = true;
			}
		}
		section_storage_[section_name] = std::move(name_value_data_value);
		in.close();
	}
};



int main()
{
	try {
		MyParsser t("../in.txt");
	}
	catch (const MyExceptionNotOpenFile& e) {
		std::cout << e.what() << '\n';
	}
	catch (const MyException& e) {
		std::cout << e.what() << '\n';
	}
}

