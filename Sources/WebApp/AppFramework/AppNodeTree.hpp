#ifndef APP_NODE_TREE_HPP
#define APP_NODE_TREE_HPP

#include "NUIE_NodeTree.hpp"

class AppNodeTree
{
public:
	AppNodeTree ();
	~AppNodeTree ();

	size_t					AddGroup (const std::wstring& groupName);
	void					AddItem (size_t groupIndex, const std::wstring& itemName, const std::wstring& iconName, const NUIE::CreatorFunction& creator);
	
	std::wstring			GetIcon (size_t groupIndex, size_t nodeIndex) const;
	NUIE::UINodePtr			CreateNode (size_t groupIndex, size_t nodeIndex, const NUIE::Point& position) const;

	const NUIE::NodeTree&	GetNodeTree () const;

private:
	NUIE::NodeTree							nodeTree;
	std::vector<std::vector<std::wstring>>	nodeIconTree;
};

#endif
