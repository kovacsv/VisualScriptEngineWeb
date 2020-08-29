#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"

#include "Application.hpp"

// #define ENABLE_EVENT_LOGGING

static Application* gAppForBrowser = nullptr;

static void AddNode (NUIE::NodeEditor& nodeEditor, const NUIE::Point& position, int nodeIndex)
{
	NUIE::UINodePtr uiNode = nullptr;
	switch (nodeIndex) {
		case 0: uiNode = NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true)); break;
		case 1: uiNode = NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1)); break;
		case 2: uiNode = NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0)); break;
		case 3: uiNode = NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position)); break;
		case 4: uiNode = NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position)); break;
		case 5: uiNode = NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position)); break;
		case 6: uiNode = NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position)); break;
		case 7: uiNode = NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position)); break;
		case 8: uiNode = NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position)); break;
		case 9: uiNode = NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position)); break;
		case 10: uiNode = NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position)); break;
		case 11: uiNode = NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5)); break;
	}
	if (uiNode != nullptr) {
		if (NE::Node::IsType<BI::BasicUINode> (uiNode)) {
			BI::BasicUINodePtr basicUINode = NE::Node::Cast<BI::BasicUINode> (uiNode);
			basicUINode->SetIconId (NUIE::IconId (nodeIndex));
		}
		nodeEditor.AddNode (uiNode);
	}
}

extern "C"
{

void ResizeWindow (int width, int height)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->ResizeWindow (width, height);
}

void ExecuteCommand (char* command)
{
	if (gAppForBrowser == nullptr) {
		return;
	}

	BrowserInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	if (browserInteface.AreEventsSuspended ()) {
		return;
	}

	std::string commandStr (command);
	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();
	if (commandStr == "New") {
		nodeEditor.New ();
	} else if (commandStr == "Save") {
		NE::MemoryOutputStream outputStream;
		nodeEditor.Save (outputStream);
		const std::vector<char>& buffer = outputStream.GetBuffer ();
		browserInteface.SaveFile (buffer);
	} else if (commandStr == "SelectAll") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::SelectAll);
	} else if (commandStr == "Copy") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Copy);
	} else if (commandStr == "Paste") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Paste);
	} else if (commandStr == "Group") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Group);
	} else if (commandStr == "Ungroup") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Ungroup);
	} else if (commandStr == "Undo") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Undo);
	} else if (commandStr == "Redo") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Redo);
	} else if (commandStr == "Escape") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Escape);
	} else if (commandStr == "Delete") {
		nodeEditor.ExecuteCommand (NUIE::CommandCode::Delete);
	}
}

void OpenFile (char* charBuffer, int size)
{
	if (gAppForBrowser == nullptr) {
		return;
	}

	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();
	std::vector<char> buffer;
	buffer.assign (charBuffer, charBuffer + size);
	NE::MemoryInputStream inputStream (buffer);
	nodeEditor.Open (inputStream);
	nodeEditor.CenterToWindow ();
}

void CreateNode (int nodeIndex, int xPosition, int yPosition)
{
	if (gAppForBrowser == nullptr) {
		return;
	}

	BrowserInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	if (browserInteface.AreEventsSuspended ()) {
		return;
	}

	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();
	NUIE::Point viewPosition (xPosition, yPosition);
	NUIE::Point position = nodeEditor.ViewToModel (viewPosition);
	AddNode (nodeEditor, position, nodeIndex);
}

void ContextMenuResponse (int commandId)
{
	BrowserInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	browserInteface.ContextMenuResponse (commandId);
}

void ParameterSettingsResponse (char* changedParametersJson)
{
	std::string changedParametersJsonStr (changedParametersJson);
	BrowserInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	browserInteface.ParameterSettingsResponse (changedParametersJson);
}

bool NeedToSave ()
{
	if (gAppForBrowser == nullptr) {
		return false;
	}

	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();
	return nodeEditor.NeedToSave ();
}

};

int main (int, char**)
{
	Application app;
	gAppForBrowser = &app;
	app.Init ();

#ifndef EMSCRIPTEN
	AddNode (app.GetNodeEditor (), NUIE::Point (100.0, 100.0), 1);
	AddNode (app.GetNodeEditor (), NUIE::Point (400.0, 300.0), 11);
#endif

	app.Start ();
	app.Shut ();

	return EXIT_SUCCESS;
}
