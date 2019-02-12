#ifndef __XMLNODE_HPP__
#define __XMLNODE_HPP__

#include <vector>
#include <string>

namespace xmlreader
{
	template <class T> class xmlnodes : public std::vector< T >
	{ public: T get_node_by_name(std::string node_name); };

	class xmlnode
	{
		friend class xmldoc;

		struct _PROPERTY 
		{
			std::string property_name_;
			std::string property_value_;
		};

	protected:
		explicit xmlnode(std::string, int);
		xmlnode(std::string, std::string, std::string, int);
		~xmlnode();

	public:
		std::string content();
		bool content(std::string);

		std::string name();
		bool name(std::string);

		std::string note();
		bool note(std::string);

		std::string property(std::string);
		bool add_property(std::string name, std::string);
		bool modify_property(std::string, std::string);
		bool remove_property(std::string);

		std::string get_node_text();

	public:
		xmlnode * add_child_node(std::string, std::string, std::string);

		bool remove_child_node(std::string);
		bool remove_child_node(int);
		bool remove_child_node(xmlnode *);
		bool remove_all_child_node();

		xmlnodes< xmlnode * > get_children();
		xmlnode * get_child(int index);
		xmlnode * get_child(std::string);

	protected:
		xmlnodes< xmlnode * > nodes_;
		std::vector< _PROPERTY > property_;
		std::string note_;
		std::string node_name_;
		std::string content_;
		int node_type_;
		int depth_;
	};

	template <class T> T 
		xmlnodes< T >::get_node_by_name(std::string node_name)
	{
		int count = (int)std::vector<T>::size();
		for (int i = 0; i < count; i++)
		{
			T node = std::vector<T>::at(i);
			if (node->name() == node_name)
				return node;
		}
		return 0;
	}
}

#endif // __XMLNODE_HPP__

