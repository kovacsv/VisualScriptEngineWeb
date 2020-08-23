#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"

#include "Application.hpp"

// #define ENABLE_EVENT_LOGGING

static Application* gAppForBrowser = nullptr;

static NUIE::ModifierKeys GetModifierKeys ()
{
	NUIE::ModifierKeys keys;
	const Uint8* keyboardState = SDL_GetKeyboardState (nullptr);
	if (keyboardState[SDL_SCANCODE_LCTRL] || keyboardState[SDL_SCANCODE_RCTRL]) {
		keys.Insert (NUIE::ModifierKeyCode::Control);
	} else if (keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT]) {
		keys.Insert (NUIE::ModifierKeyCode::Shift);
	}
	return keys;
}

static NUIE::MouseButton GetMouseButtonFromEvent (const SDL_Event& sdlEvent)
{
	if (sdlEvent.button.button == 1) {
		return NUIE::MouseButton::Left;
	} else if (sdlEvent.button.button == 2) {
		return NUIE::MouseButton::Middle;
	} else if (sdlEvent.button.button == 3) {
		return NUIE::MouseButton::Right;
	}
	return NUIE::MouseButton::Left;
}

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

static bool MainLoop (Application* app)
{
	BrowserAsyncInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	bool enableEvents = !browserInteface.AreEventsSuspended ();
	NUIE::NodeEditor& nodeEditor = app->GetNodeEditor ();

	SDL_Event sdlEvent;
	if (SDL_PollEvent (&sdlEvent) && enableEvents) {
#if defined (EMSCRIPTEN) && defined (ENABLE_EVENT_LOGGING)
		EM_ASM ({
			console.log ($0);
		}, sdlEvent.type);
#endif
		switch (sdlEvent.type) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
				{
					NUIE::ModifierKeys modifierKeys = GetModifierKeys ();
					NUIE::MouseButton button = GetMouseButtonFromEvent (sdlEvent);
					if (sdlEvent.button.clicks == 2) {
						nodeEditor.OnMouseDoubleClick (modifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
					} else {
						Uint8 clicks = sdlEvent.button.clicks;
						if (clicks > 2) {
							clicks -= 2;
						}
						for (Uint8 clickIndex = 0; clickIndex < clicks; clickIndex++) {
							nodeEditor.OnMouseDown (modifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				{
					NUIE::ModifierKeys modifierKeys = GetModifierKeys ();
					NUIE::MouseButton button = GetMouseButtonFromEvent (sdlEvent);
					nodeEditor.OnMouseUp (modifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
				}
				break;
			case SDL_MOUSEMOTION:
				{
					NUIE::ModifierKeys modifierKeys = GetModifierKeys ();
					nodeEditor.OnMouseMove (modifierKeys, sdlEvent.motion.x, sdlEvent.motion.y);
				}
				break;
			case SDL_MOUSEWHEEL:
				{
					NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
					if (sdlEvent.wheel.x + sdlEvent.wheel.y < 0) {
						rotation = NUIE::MouseWheelRotation::Backward;
					}
					int mouseX = 0;
					int mouseY = 0;
					SDL_GetMouseState (&mouseX, &mouseY);
					NUIE::ModifierKeys modifierKeys = GetModifierKeys ();
					double currentScale = nodeEditor.GetViewBox ().GetScale ();
					bool preventZoom = (rotation == NUIE::MouseWheelRotation::Forward && currentScale > 5.0);
					if (!preventZoom) {
						nodeEditor.OnMouseWheel (modifierKeys, rotation, mouseX, mouseY);
					}
				}
				break;
		}
	}

	return true;
}

#ifdef EMSCRIPTEN
static void EmscriptenMainLoop (void* arg)
{
	Application* app = (Application*) arg;
	MainLoop (app);
}
#endif

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

	BrowserAsyncInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	if (browserInteface.AreEventsSuspended ()) {
		return;
	}

	std::string commandStr (command);
	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();
	if (commandStr == "New") {
		nodeEditor.New ();
	} else if (commandStr == "Save") {
#ifdef EMSCRIPTEN
		NE::MemoryOutputStream outputStream;
		nodeEditor.Save (outputStream);
		const std::vector<char>& buffer = outputStream.GetBuffer ();
		EM_ASM ({
			SaveFile ($0, $1);
		}, buffer.data (), buffer.size ());
#endif
	} else if (commandStr == "SelectAll") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::SelectAll);
	} else if (commandStr == "Copy") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Copy);
	} else if (commandStr == "Paste") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Paste);
	} else if (commandStr == "Group") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Group);
	} else if (commandStr == "Ungroup") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Ungroup);
	} else if (commandStr == "Undo") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Undo);
	} else if (commandStr == "Redo") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Redo);
	} else if (commandStr == "Escape") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Escape);
	} else if (commandStr == "Delete") {
		nodeEditor.OnKeyPress (NUIE::KeyCode::Delete);
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

	BrowserAsyncInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
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
	BrowserAsyncInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	browserInteface.ContextMenuResponse (commandId);
}

void ParameterSettingsResponse (char* changedParametersJson)
{
	std::string changedParametersJsonStr (changedParametersJson);
	BrowserAsyncInterface& browserInteface = gAppForBrowser->GetBrowserInterface ();
	browserInteface.ParameterSettingsResponse (changedParametersJson);
}

};

int EventFilter (void*, SDL_Event* sdlEvent)
{
	// filter finger events, because on mobile both the mouse and the finger events arrive
	// filter key down event because it arrives during mouse drag&drop as well
	if (sdlEvent->type == SDL_FINGERDOWN ||
		sdlEvent->type == SDL_FINGERUP ||
		sdlEvent->type == SDL_FINGERMOTION ||
		sdlEvent->type == SDL_KEYDOWN ||
		sdlEvent->type == SDL_KEYUP)
	{
		return 0;
	}
	return 1;
}

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_SetEventFilter (EventFilter, nullptr);
	SDL_EventState (SDL_TEXTINPUT, SDL_DISABLE);
	SDL_EventState (SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState (SDL_KEYUP, SDL_DISABLE);
	TTF_Init ();

	SDL_Window *window;
	SDL_Renderer *renderer;

	static short InitialWindowWidth = 700;
	static short InitialWindowHeight = 500;
#ifdef EMSCRIPTEN
	// let the browser do the resize logic
	InitialWindowWidth = 10;
	InitialWindowHeight = 10;
#endif

	window = SDL_CreateWindow ("VisualScriptEngineWeb", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, InitialWindowWidth, InitialWindowHeight, 0);
	renderer = SDL_CreateRenderer (window, -1, 0);

	{
		Application app (window, renderer);
		gAppForBrowser = &app;

#ifdef EMSCRIPTEN
		EM_ASM (
			OnWindowCreated ();
		);
		emscripten_set_main_loop_arg (EmscriptenMainLoop, &app, 0, true);
#else
		NUIE::NodeEditor& nodeEditor = app.GetNodeEditor ();
		AddNode (nodeEditor, NUIE::Point (100.0, 100.0), 1);
		AddNode (nodeEditor, NUIE::Point (400.0, 300.0), 11);

		while (true) {
			if (!MainLoop (&app)) {
				break;
			}
		}
#endif

		gAppForBrowser = nullptr;
	}

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);

	TTF_Quit ();
	SDL_Quit ();

	std::cout << ("hello") << std::endl;
	return EXIT_SUCCESS;
}
