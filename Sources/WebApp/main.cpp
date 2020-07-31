#include <iostream>
#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>

#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"
#include "SDL2Context.hpp"

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment (SDL_Renderer* renderer) :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::GetDefaultStringConverter ()),
		skinParams (NUIE::GetDefaultSkinParams ()),
		drawingContext (renderer, "Assets/OpenSans-Regular.ttf"),
		eventHandler (),
		clipboardHandler (),
		evaluationEnv (nullptr),
		nodeEditor (nullptr)
	{

	}

	void Init (NUIE::NodeEditor* nodeEditorPtr)
	{
		nodeEditor = nodeEditorPtr;
	}
	
	virtual const NE::StringConverter& GetStringConverter () override
	{
		return stringConverter;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return drawingContext;
	}

	virtual double GetWindowScale () override
	{
		return 1.0;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnEvaluationBegin () override
	{

	}

	virtual void OnEvaluationEnd () override
	{

	}

	virtual void OnValuesRecalculated () override
	{

	}

	virtual void OnRedrawRequested () override
	{
		nodeEditor->Draw ();
	}

	virtual NUIE::EventHandler& GetEventHandler () override
	{
		return eventHandler;
	}

	virtual NUIE::ClipboardHandler& GetClipboardHandler () override
	{
		return clipboardHandler;
	}

	virtual double GetMouseMoveMinOffset () override
	{
		return 2.0;
	}

private:
	NE::BasicStringConverter		stringConverter;
	NUIE::BasicSkinParams			skinParams;
	SDL2Context						drawingContext;
	NUIE::NullEventHandler			eventHandler;
	NUIE::MemoryClipboardHandler	clipboardHandler;
	NE::EvaluationEnv				evaluationEnv;
	NUIE::NodeEditor*				nodeEditor;
};

class Application
{
public:
	Application (SDL_Renderer* renderer) :
		renderer (renderer),
		uiEnvironment (renderer),
		nodeEditor (uiEnvironment)
	{
		uiEnvironment.Init (&nodeEditor);
		nodeEditor.Update ();
	}

	SDL_Renderer* GetRenderer ()
	{
		return renderer;
	}

	NUIE::NodeEditor& GetNodeEditor ()
	{
		return nodeEditor;
	}

private:
	SDL_Renderer*			renderer;

	MyNodeUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
};

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
	NUIE::NodeEditor& nodeEditor = app->GetNodeEditor ();

	SDL_Event sdlEvent;
	if (SDL_PollEvent (&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
				{
					NUIE::ModifierKeys modifierKeys = GetModifierKeys ();
					NUIE::MouseButton button = GetMouseButtonFromEvent (sdlEvent);
					nodeEditor.OnMouseDown (modifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
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
			case SDL_KEYDOWN:
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

Application* gAppForBrowser = nullptr;

extern "C"
{

void CreateNode (int nodeIndex)
{
	if (gAppForBrowser == nullptr) {
		return;
	}

	SDL_Renderer* renderer = gAppForBrowser->GetRenderer ();
	NUIE::NodeEditor& nodeEditor = gAppForBrowser->GetNodeEditor ();

	SDL_Rect renderRect;
	SDL_RenderGetViewport (renderer, &renderRect);

	NUIE::Rect viewRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (0.0, 0.0), NUIE::Size (renderRect.w, renderRect.h));
	NUIE::Point viewCenter = viewRect.GetCenter ();
	NUIE::Point position = nodeEditor.ViewToModel (viewCenter);

	switch (nodeIndex) {
		case 0: nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1))); break;
		case 1: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0))); break;
		case 2: nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position))); break;
		case 3: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position))); break;
		case 4: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position))); break;
		case 5: nodeEditor.AddNode (NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position))); break;
		case 6: nodeEditor.AddNode (NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position))); break;
		case 7: nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position))); break;
		case 8: nodeEditor.AddNode (NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position))); break;
		case 9: nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5))); break;
	}
}

};

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	TTF_Init ();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer (640, 480, 0, &window, &renderer);
	
	{
		Application app (renderer);
		gAppForBrowser = &app;

#ifdef EMSCRIPTEN
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
