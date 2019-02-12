#include "xmldoc.hpp"

namespace xmlreader
{
	xmldoc::xmldoc(const char * file_name)
	{
		file_name_ = file_name;
		root_node_ = 0;
	}

	xmldoc::~xmldoc()
	{
		if (root_node_) 
			delete root_node_;
	}

	bool xmldoc::load_xml_doc()
	{
		if (file_name_ == "" ) 
			return false;

		buffer_ = "";
		FILE * handle = ::fopen(file_name_.c_str(),"r");
		if (handle == 0) return false;

		::fseek(handle, 0L, SEEK_END);
		int len = ::ftell(handle);
		::fseek(handle, 0L, SEEK_SET);

		char * buff = new char[len + 1];
		memset(buff, 0, len + 1);

		::fread(buff, 1, len + 1, handle);
		::fclose(handle);

		buffer_ = buff;
		delete [] buff;
		delete root_node_;
		root_node_ = 0;

		return true;
	}

	xmlnode* xmldoc::get_root_node(const char * name)
	{
		if (root_node_)
			return root_node_;

		std::string str_start = "<";
		str_start += name;

		std::string str_end = "</";
		str_end += name;
		str_end += ">";

		int start = (int)buffer_.find(str_start);
		int end = (int)buffer_.find(str_end);

		if (end < 0) return 0;

		end = end + (int)str_end.length();
		std::string node_text = buffer_.substr(start,end - start);

		root_node_ = new xmlnode(node_text.c_str(), 0);
		return root_node_;
	}
}


