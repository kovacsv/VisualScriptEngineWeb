#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

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
			case SDL_KEYUP:
				{
					NUIE::Key pressedKey (NUIE::KeyCode::Undefined);
					bool isControlPressed = (sdlEvent.key.keysym.mod & SDL_SCANCODE_LCTRL);
					if (isControlPressed) {
						switch (sdlEvent.key.keysym.sym) {
							case SDLK_a:
								pressedKey.SetKeyCode (NUIE::KeyCode::SelectAll);
								break;
							case SDLK_c:
								pressedKey.SetKeyCode (NUIE::KeyCode::Copy);
								break;
							case SDLK_v:
								pressedKey.SetKeyCode (NUIE::KeyCode::Paste);
								break;
							case SDLK_g:
								pressedKey.SetKeyCode (NUIE::KeyCode::Group);
								break;
							case SDLK_z:
								pressedKey.SetKeyCode (NUIE::KeyCode::Undo);
								break;
							case SDLK_y:
								pressedKey.SetKeyCode (NUIE::KeyCode::Redo);
								break;
						}
					} else {
						switch (sdlEvent.key.keysym.sym) {
							case SDLK_ESCAPE:
								pressedKey.SetKeyCode (NUIE::KeyCode::Escape);
								break;
							case SDLK_DELETE:
							case SDLK_BACKSPACE:
								pressedKey.SetKeyCode (NUIE::KeyCode::Delete);
								break;
						}
					}
					if (pressedKey.IsValid ()) {
						nodeEditor.OnKeyPress (pressedKey);
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

	switch (nodeIndex) {
		case 0: nodeEditor.AddNode (NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true))); break;
		case 1: nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1))); break;
		case 2: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0))); break;
		case 3: nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position))); break;
		case 4: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position))); break;
		case 5: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position))); break;
		case 6: nodeEditor.AddNode (NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position))); break;
		case 7: nodeEditor.AddNode (NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position))); break;
		case 8: nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position))); break;
		case 9: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position))); break;
		case 10: nodeEditor.AddNode (NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position))); break;
		case 11: nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5))); break;
	}
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
		sdlEvent->type == SDL_KEYDOWN)
	{
		return 0;
	}
	return 1;
}

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_SetEventFilter (EventFilter, nullptr);
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

	SDL_SetHint (SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
	SDL_CreateWindowAndRenderer (InitialWindowWidth, InitialWindowHeight, 0, &window, &renderer);

	{
		Application app (window, renderer);
		gAppForBrowser = &app;

#ifdef EMSCRIPTEN
		EM_ASM (
			OnWindowCreated ();
		);
		emscripten_set_main_loop_arg (EmscriptenMainLoop, &app, 0, true);
#else
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
