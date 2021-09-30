#include "FileManager.h"
#include<iostream>
#include<fstream>
#include<exception>
#include"Date.h"
FileManager::FileDetails FileManager::getFileDetails(const string& fileName)
{
	ifstream file(rootPath+"/"+fileName);
	if (!file.is_open())
	{
		throw logic_error("file doesnt exists");
	}
	string body = "";
	string line;
	bool first = true;
	while (getline(file, line))
	{
		if (!first)
			body += "\n";
		body += line;
		first = false;
	}
	file.close();
	return { fileName,body,rootPath+"/"+fileName };
	
}

string FileManager::getModificationDate(const string& fileName)
{
	string path= rootPath + "/" + fileName;
	if (!isFileExists(fileName))
		throw logic_error("file doesnt exists");
	struct stat result;
	if (stat(path.c_str(), &result) == 0)
	{
		auto mod_time = result.st_mtime;
		return Date::getGMT_Time(&mod_time);

	}
	return "bad date!!!";
}

int FileManager::updateOrCreate(const FileDetails& file_details)
{
	string filePath = rootPath + "/" + file_details.name;
	int action = isFileExists(file_details.name) ? UPDATE : CREATE;
	ofstream file(filePath);
		if (!file)
		{
			throw logic_error("error at opening file");
	    }
		file << file_details.body;
		file.close();
		return action;
}

bool FileManager::isFileExists(const string& fileName)
{
	bool exists = false;
	ifstream file(rootPath + "/" + fileName);
	exists = file.is_open();
	file.close();
	return exists;
	
}
