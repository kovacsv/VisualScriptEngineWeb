#ifndef BROWSER_INTERFACE_HPP
#define BROWSER_INTERFACE_HPP

#include "NUIE_NodeEditor.hpp"
#include "AppNodeTree.hpp"

class Application;
void SetAppForBrowser (Application* application);

class BrowserInterface
{
public:
	enum class State
	{
		Normal,
		WaitingForContextMenuResponse,
		WaitingForParametersResponse
	};

	class ContextMenuData
	{
	public:
		ContextMenuData ();

		NUIE::MenuCommandStructure		commandStructure;
		int								selectedCommandId;
	};

	class ParameterSettingsData
	{
	public:
		ParameterSettingsData ();

		NUIE::ParameterInterfacePtr		paramInterface;
	};

	BrowserInterface (NUIE::NodeEditor& nodeEditor);

	bool					AreEventsSuspended () const;

	void					OnAppInitialized (const AppNodeTree& appNodeTree) const;
	void					SaveFile (const std::vector<char>& buffer) const;

	NUIE::MenuCommandPtr	ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	void					ContextMenuResponse (int commandId);

	void					DoubleClickRequest (const NUIE::Point& position);

	bool					ParameterSettingsRequest (NUIE::ParameterInterfacePtr parameters);
	void					ParameterSettingsResponse (const std::string& changedParametersJsonStr);

private:
	NUIE::NodeEditor&		nodeEditor;

	State					state;
	ContextMenuData			contextMenuData;
	ParameterSettingsData	paramSettingsData;
};

#endif
