#pragma once
#include<string>
#include<list>
using namespace std;
class FileManager
{
public:
	struct FileDetails
	{
		string name;
		string body;
		string fullpath;
	};
private:
	string rootPath="C:/temp";
public: 
	FileManager(const string&rootPath= "C:/temp"):rootPath(rootPath){}
	//if a file with name=fileName exists the function return his fileDetails
	//else the functoin will throw exception
	FileDetails getFileDetails(const string& fileName);
	//the function get the last date file with name=fileName  as been modified (the date is in GMT format)
	//if the file doesnt exists an exception will be thrown
	string getModificationDate(const string& fileName);
	//the function get file_details and create a file if not exists or create file with the name
	//specified in file_details and uptate his content to file_details content
	int updateOrCreate(const FileDetails&file_details);
	//the function get a file name and deletes him
	void deleteFile(const string& fileName){ std::remove((rootPath+"/"+fileName).c_str()); }
	//the function return true if file with name=fileName exists otherwise false
	bool isFileExists(const string& fileName);
	static constexpr int CREATE = 1, UPDATE = 2;
	

	

};

