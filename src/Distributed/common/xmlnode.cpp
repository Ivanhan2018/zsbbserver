#include "xmlnode.hpp"

namespace xmlreader
{
	const char * XML_NOTE_BEGIN_TAG = "<!-- ";
	const char * XML_NOTE_END_TAG = " -->";
	const int XML_NOTE_BEGIN_LEN = 5;
	const int XML_NOTE_END_LEN = 4;

	std::string repeat_char(char chr, int num)
	{
		std::string temp = "";
		for (int i = 0; i < num; i++)
			temp += chr;
		return temp;
	}


	xmlnode::xmlnode(std::string node_text, int depth)
	{
		property_.clear();
		int len = (int)node_text.length();

		char * c_node_text = new char[ len + 1 ];
		memset(c_node_text, 0, len + 1);
		const char * node_temp = node_text.c_str();

		for (int i = 0, j = 0; i < len; i++)
		{
			if (!((node_temp[i] == '\t') || (node_temp[i] == '\r') || (node_temp[i] == '\n')))
				c_node_text[j++] = node_temp[i];
		}

		std::string str_node_text = c_node_text;
		delete [] c_node_text;

		int note_start = (int)str_node_text.find("<", 0);
		int note_end = 0;

		if (str_node_text.substr(note_start, XML_NOTE_BEGIN_LEN) == XML_NOTE_BEGIN_TAG)
		{
			note_end = (int)str_node_text.find(XML_NOTE_END_TAG, note_start) + XML_NOTE_END_LEN;

			if (note_end > 0)
			{
				note_ = str_node_text.substr(note_start + XML_NOTE_BEGIN_LEN, 
					note_end - XML_NOTE_BEGIN_LEN - XML_NOTE_END_LEN);
			}
		}

		int name_start = (int)str_node_text.find("<", note_end);
		int name_end_1 = (int)str_node_text.find(" ", name_start);
		int name_end_2 = (int)str_node_text.find(">", name_start);
		int name_end = 0;
		node_type_ = 0;

		if (name_end_1 == -1)
			name_end = name_end_2;
		else
			name_end = name_end_1 > name_end_2 ? name_end_2 : name_end_1;

		if (name_end_2 > 0)
		{
			if (str_node_text.substr(name_end_2 - 1, 1) == "/")
				node_type_ = 1;
		}

		if (node_type_ == 1 && name_end_1 == -1)
			node_name_ = str_node_text.substr(name_start + 1, name_end - name_start - 2);
		else
			node_name_ = str_node_text.substr(name_start + 1, name_end - name_start - 1);

		int content_start = (int)str_node_text.find(">", note_end);
		int content_end = (int)str_node_text.find("<", content_start);

		if (content_end - content_start == 1) content_ = "";

		content_ = str_node_text.substr(content_start + 1, content_end - content_start - 1);

		int property_start = (int)str_node_text.find("<", note_end);
		int property_end = (int)str_node_text.find(">", property_start);

		std::string str_property = str_node_text.substr(property_start, property_end - property_start + 1);

		int eq_pos = (int)str_property.find("=", 0);

		while(eq_pos > 0)
		{
			int space_pos = (int)str_property.rfind(" ", eq_pos);
			if (space_pos > 0)
			{
				std::string str_property_name = str_property.substr(space_pos + 1, eq_pos - space_pos -1);

				int point_start = (int)str_property.find("\"", eq_pos);
				int point_end = (int)str_property.find("\"", point_start + 1);

				if (point_start > 0 && point_end > 0)
				{
					std::string str_property_value = str_property.substr(point_start+1, point_end - point_start - 1);

					_PROPERTY temp;
					temp.property_name_ = str_property_name;
					temp.property_value_ = str_property_value;
					property_.push_back(temp);
				}
			}
			eq_pos = (int)str_property.find("=", eq_pos + 1);
		}

		int node_start = content_end;
		std::string s_node_text = "";
		while (node_start >= 0)
		{
			node_start = (int)str_node_text.find("<", node_start);
			if (node_start == -1)
				break;

			int node_end = 0;

			int note_start = node_start;
			int note_end = node_start;
			if (str_node_text.substr(node_start, XML_NOTE_BEGIN_LEN) == XML_NOTE_BEGIN_TAG)
			{
				note_end = (int)str_node_text.find(XML_NOTE_END_TAG, node_start);
				node_start = (int)str_node_text.find("<", note_end + 1);
			}

			int node_end_1 = (int)str_node_text.find(" ", node_start);
			int node_end_2 = (int)str_node_text.find(">", node_start);

			if (str_node_text.substr(node_end_2 - 1, 1) == "/")
				node_end = node_end_2 + 1;
			else
			{
				int name_start = node_start;
				int name_end = 0;
				if (node_end_1 == -1)
					name_end = node_end_2;
				else
					name_end = node_end_1 > node_end_2 ? node_end_2 : node_end_1;

				std::string strNodeName = str_node_text.substr(name_start + 1, 
					name_end - name_start - 1);

				std::string strEndTag = "</" + strNodeName + ">";
				node_end = (int)str_node_text.find(strEndTag, name_end);

				if (node_end > 0)
					node_end += (int)(strNodeName.length() + 3);
				else
				{
					node_start = -1;
					continue;
				}
			}
			s_node_text = str_node_text.substr(note_start, node_end - note_start);
			xmlnode* node = new xmlnode(s_node_text, depth_ + 1);
			nodes_.push_back(node);
			node_start = node_end;
		}
	}

	xmlnode::xmlnode(std::string note, std::string name, std::string content, int depth)
	{
		note_ = note;
		node_name_ = name;
		content_ = content;
		depth_ = depth;	
		node_type_ = content_ == "" ? 1 : 0;
	}

	xmlnode::~xmlnode()
	{
		property_.clear();
		remove_all_child_node();
	}

	std::string xmlnode::note()
	{
		return note_;
	}

	bool xmlnode::note(std::string name)
	{	
		note_ = name;
		return true;
	}

	std::string xmlnode::name()
	{
		return node_name_;
	}

	bool xmlnode::name(std::string name)
	{
		node_name_ = name;

		return true;
	}

	std::string xmlnode::content()
	{
		return content_;
	}

	bool xmlnode::content(std::string content)
	{
		content_ = content;
		if (content_ == "")
			node_type_ = 0;
		else
			node_type_ = 1;

		return true;
	}

	std::string xmlnode::property(std::string name)
	{
		std::vector<_PROPERTY>::iterator it;
		for (it = property_.begin(); it != property_.end(); it++)
		{
			if (it->property_name_ == name)
				return it->property_value_;
		}

		return "";
	}

	bool xmlnode::add_property(std::string name, 
		std::string value)
	{
		bool is_exist = false;
		std::vector<_PROPERTY>::iterator it;
		for (it = property_.begin(); it != property_.end(); it++)
		{
			if (it->property_name_ == name)
			{
				is_exist = true;
				break;
			}
		}

		if (is_exist == false)
		{
			_PROPERTY temp;
			temp.property_name_ = name;
			temp.property_value_ = value;
			property_.push_back(temp);
			return true;
		}

		return false;
	}

	bool xmlnode::modify_property(std::string name, 
		std::string value)
	{
		std::vector<_PROPERTY>::iterator it;
		for (it = property_.begin(); it != property_.end(); it++)
		{
			if (it->property_name_ == name)
			{
				it->property_value_ = value;
				return true;
			}
		}

		return false;
	}

	bool xmlnode::remove_property(std::string name)
	{
		std::vector<_PROPERTY>::iterator it;
		for (it = property_.begin(); it != property_.end(); it++)
		{
			if (it->property_name_ == name)
			{
				property_.erase(it);
				return true;
			}
		}

		return false;
	}

	std::string xmlnode::get_node_text()
	{
		std::string str_node_text = "";

		if (note_ != "")
		{
			str_node_text += repeat_char('\t', depth_);
			str_node_text += XML_NOTE_BEGIN_TAG + note_ + XML_NOTE_END_TAG + '\n';
		}

		str_node_text += repeat_char('\t', depth_);
		str_node_text += "<" + node_name_;

		std::vector<_PROPERTY>::iterator it;
		for (it = property_.begin(); it != property_.end(); it++)
		{
			str_node_text += " " + it->property_name_ + 
				"=\"" + it->property_value_ + "\"";		
		}

		if (node_type_ == 1)
			str_node_text += ">";
		else
			str_node_text += ">";	

		if (nodes_.size() != 0)
		{
			if (content_ != "")
			{
				str_node_text += '\n';
				str_node_text += repeat_char('\t',depth_ + 1);
				str_node_text += content_;
			}
		}
		else
		{
			str_node_text += content_;
		}

		if (nodes_.size() != 0)
		{
			str_node_text += '\n';
			for (int i = 0; i < (int)nodes_.size(); i++)
			{
				str_node_text += (nodes_[i])->get_node_text();
			}
		}	

		if (nodes_.size() != 0)
		{
			str_node_text += repeat_char('\t',depth_);
			str_node_text += "</" + node_name_ + ">\n";
		}
		else
		{	
			str_node_text += "</" + node_name_ + ">\n";
		}

		return str_node_text;
	}

	xmlnode * xmlnode::add_child_node(std::string note, 
		std::string name, 
		std::string content)
	{
		xmlnode * node = new xmlnode(note, name, content, depth_ + 1);
		if (node != 0)
		{
			nodes_.push_back(node);
			return node;
		}
		return 0;
	}

	bool xmlnode::remove_child_node(std::string name)
	{
		xmlnodes< xmlnode * >::iterator it;
		for (it = nodes_.begin(); it != nodes_.end(); it++)
		{		
			if ((*it)->name() == name)
			{
				delete *it;
				nodes_.erase(it);
				return true;
			}
		}
		return false;
	}

	bool xmlnode::remove_child_node(int index)
	{
		int i = 0;
		xmlnodes< xmlnode * >::iterator it;
		for (it = nodes_.begin(); it != nodes_.end(); it++, i++)
		{
			if (i == index)
			{
				delete *it;
				nodes_.erase(it);
				return true;
			}
		}
		return false;
	}

	bool xmlnode::remove_child_node(xmlnode * node)
	{
		xmlnodes< xmlnode * >::iterator it;
		for (it = nodes_.begin(); it != nodes_.end(); it++)
		{
			if (*it == node)
			{
				delete *it;
				nodes_.erase(it);
				return true;
			}
		}
		return false;
	}

	bool xmlnode::remove_all_child_node()
	{
		for (unsigned int i = 0; i < nodes_.size(); i++)
		{
			delete nodes_[i];
		}
		nodes_.clear();

		//xmlnodes< xmlnode * >::iterator it = nodes_.begin();
		//while (it != nodes_.end())
		//{
		//	delete *it;
		//	nodes_.erase(it);
		//}
		return true;
	}

	xmlnodes<xmlnode*> xmlnode::get_children()
	{
		return nodes_;
	}

	xmlnode* xmlnode::get_child(int index)
	{
		return nodes_[index];
	}

	xmlnode* xmlnode::get_child(std::string node_name)
	{
		return nodes_.get_node_by_name(node_name);
	}

}

