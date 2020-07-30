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
	MyNodeUIEnvironment (int width, int height, SDL_Renderer* renderer) :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::GetDefaultStringConverter ()),
		skinParams (NUIE::GetDefaultSkinParams ()),
		drawingContext (width, height, renderer),
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
		uiEnvironment (640, 480, renderer),
		nodeEditor (uiEnvironment)
	{
		uiEnvironment.Init (&nodeEditor);
		nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), NUIE::Point (100.0, 100.0), 0, 1)));
		nodeEditor.AddNode (NUIE::UINodePtr (new BI::ViewerNode (NE::LocString (L"Viewer"), NUIE::Point (300.0, 200.0))));
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
					NUIE::MouseButton button = GetMouseButtonFromEvent (sdlEvent);
					nodeEditor.OnMouseDown (NUIE::EmptyModifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				{
					NUIE::MouseButton button = GetMouseButtonFromEvent (sdlEvent);
					nodeEditor.OnMouseUp (NUIE::EmptyModifierKeys, button, sdlEvent.button.x, sdlEvent.button.y);
				}
				break;
			case SDL_MOUSEMOTION:
				{
					nodeEditor.OnMouseMove (NUIE::EmptyModifierKeys, sdlEvent.motion.x, sdlEvent.motion.y);
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

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	TTF_Init ();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer (640, 480, 0, &window, &renderer);
	
	Application app (renderer);

#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg (EmscriptenMainLoop, &app, 0, true);
#else
	while (true) {
		if (!MainLoop (&app)) {
			break;
		}
	}
#endif

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);

	TTF_Quit ();
	SDL_Quit ();

	std::cout << ("hello") << std::endl;
	return EXIT_SUCCESS;
}
