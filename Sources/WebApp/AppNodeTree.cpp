#include "AppNodeTree.hpp"

AppNodeTree::AppNodeTree () :
	nodeTree (),
	nodeIconTree ()
{

}

AppNodeTree::~AppNodeTree ()
{

}

size_t AppNodeTree::AddGroup (const std::wstring& groupName)
{
	nodeIconTree.push_back ({});
	return nodeTree.AddGroup (groupName);
}

void AppNodeTree::AddItem (size_t groupIndex, const std::wstring& itemName, const std::wstring& iconName, const NUIE::CreatorFunction& creator)
{
	nodeTree.AddItem (groupIndex, itemName, creator);
	nodeIconTree[groupIndex].push_back (iconName);
}

std::wstring AppNodeTree::GetIcon (size_t groupIndex, size_t nodeIndex) const
{
	return nodeIconTree[groupIndex][nodeIndex];
}

NUIE::UINodePtr AppNodeTree::CreateNode (size_t groupIndex, size_t nodeIndex, const NUIE::Point& position) const
{
	const std::vector<NUIE::NodeTree::Group>& groups = nodeTree.GetGroups ();
	if (groupIndex >= groups.size ()) {
		return nullptr;
	}
	const NUIE::NodeTree::Group& group = groups[groupIndex];
	const std::vector<NUIE::NodeTree::Item>& items = group.GetItems ();
	if (nodeIndex >= items.size ()) {
		return nullptr;
	}
	const NUIE::NodeTree::Item& item = items[nodeIndex];
	NUIE::CreatorFunction creator = item.GetCreator ();
	return creator (position);
}

const NUIE::NodeTree& AppNodeTree::GetNodeTree () const
{
	return nodeTree;
}
