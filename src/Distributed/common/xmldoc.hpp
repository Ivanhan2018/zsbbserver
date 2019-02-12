#ifndef __XMLDOC_HPP__
#define __XMLDOC_HPP__

#include "xmlnode.hpp"

namespace xmlreader
{
	class xmldoc
	{
	public:
		explicit xmldoc(const char *);
		~xmldoc();

		bool load_xml_doc();
		xmlnode * get_root_node(const char *);

	private:
		xmlnode * root_node_;
		std::string buffer_;
		std::string file_name_;
	};
}

#endif // __XMLDOC_HPP__

