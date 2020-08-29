#include "Application.hpp"

#include "NE_MemoryStream.hpp"
#include "BI_BuiltInNodes.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

static int EventFilter (void*, SDL_Event* sdlEvent)
{
	// filter finger events, because on mobile both the mouse and the finger events arrive
	// filter key down event because it arrives during mouse drag&drop as well
	if (sdlEvent->type == SDL_FINGERDOWN ||
		sdlEvent->type == SDL_FINGERUP ||
		sdlEvent->type == SDL_FINGERMOTION ||
		sdlEvent->type == SDL_KEYDOWN ||
		sdlEvent->type == SDL_KEYUP) {
		return 0;
	}
	return 1;
}

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

static bool MainLoop (Application* app)
{
	BrowserInterface& browserInteface = app->GetBrowserInterface ();
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

Application::Application () :
	window (nullptr),
	renderer (nullptr),
	uiEnvironment (&browserInterface),
	nodeEditor (uiEnvironment),
	browserInterface (nodeEditor)
{

}

Application::~Application ()
{

}

void Application::Init ()
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_SetEventFilter (EventFilter, nullptr);
	SDL_EventState (SDL_TEXTINPUT, SDL_DISABLE);
	SDL_EventState (SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState (SDL_KEYUP, SDL_DISABLE);
	TTF_Init ();

	static short InitialWindowWidth = 700;
	static short InitialWindowHeight = 500;
#ifdef EMSCRIPTEN
	// let the browser do the resize logic
	InitialWindowWidth = 10;
	InitialWindowHeight = 10;
#endif

	window = SDL_CreateWindow ("VisualScriptEngineWeb", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, InitialWindowWidth, InitialWindowHeight, 0);
	renderer = SDL_CreateRenderer (window, -1, 0);
	uiEnvironment.Init (renderer, &nodeEditor);
	browserInterface.OnWindowCreated ();
}

void Application::Start ()
{
#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg (EmscriptenMainLoop, this, 0, true);
#else
	while (true) {
		if (!MainLoop (this)) {
			break;
		}
	}
#endif
}

void Application::Shut ()
{
	uiEnvironment.Shut ();
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);

	TTF_Quit ();
	SDL_Quit ();
}

void Application::ResizeWindow (int width, int height)
{
	SDL_SetWindowSize (window, width, height);
	nodeEditor.OnResize (width, height);
}

void Application::ExecuteCommand (const char* command)
{
	if (browserInterface.AreEventsSuspended ()) {
		return;
	}
	std::string commandStr (command);
	if (commandStr == "New") {
		nodeEditor.New ();
	} else if (commandStr == "Save") {
		NE::MemoryOutputStream outputStream;
		nodeEditor.Save (outputStream);
		const std::vector<char>& buffer = outputStream.GetBuffer ();
		browserInterface.SaveFile (buffer);
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

void Application::CreateNode (int nodeIndex, int xPosition, int yPosition)
{
	if (browserInterface.AreEventsSuspended ()) {
		return;
	}

	NUIE::Point viewPosition (xPosition, yPosition);
	NUIE::Point position = nodeEditor.ViewToModel (viewPosition);
	NUIE::UINodePtr uiNode = GetNodeByIndex (nodeIndex, position);
	if (uiNode != nullptr) {
		if (NE::Node::IsType<BI::BasicUINode> (uiNode)) {
			BI::BasicUINodePtr basicUINode = NE::Node::Cast<BI::BasicUINode> (uiNode);
			basicUINode->SetIconId (NUIE::IconId (nodeIndex));
		}
		nodeEditor.AddNode (uiNode);
	}
}

void Application::OpenFile (const char* buffer, int size)
{
	std::vector<char> bufferVec;
	bufferVec.assign (buffer, buffer + size);
	NE::MemoryInputStream inputStream (bufferVec);
	nodeEditor.Open (inputStream);
	nodeEditor.CenterToWindow ();
}

bool Application::NeedToSave () const
{
	return nodeEditor.NeedToSave ();
}

void Application::ContextMenuResponse (int commandId)
{
	browserInterface.ContextMenuResponse (commandId);
}

void Application::ParameterSettingsResponse (const char* changedParametersJson)
{
	std::string changedParametersJsonStr (changedParametersJson);
	browserInterface.ParameterSettingsResponse (changedParametersJson);
}

NUIE::NodeEditor& Application::GetNodeEditor ()
{
	return nodeEditor;
}

BrowserInterface& Application::GetBrowserInterface ()
{
	return browserInterface;
}
