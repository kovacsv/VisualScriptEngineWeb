#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "BrowserAsyncInterface.hpp"
#include "SDL2Context.hpp"

#include "NUIE_NodeEditor.hpp"

class AppEventHandler : public NUIE::EventHandler
{
public:
	AppEventHandler (BrowserAsyncInterface* browserInterface);
	virtual ~AppEventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& outputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& uiGroup, const NUIE::MenuCommandStructure& commands) override;
	
	virtual void					OnDoubleClick (NUIE::MouseButton mouseButton, const NUIE::Point& position) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodePtr& uiNode) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodeGroupPtr& uiGroup) override;

private:
	BrowserAsyncInterface*			browserInterface;
};

class AppUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	AppUIEnvironment (SDL_Renderer* renderer, BrowserAsyncInterface* browserInterface);
	
	void								Init (NUIE::NodeEditor* nodeEditorPtr);

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;

	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;

	virtual NUIE::EventHandler&			GetEventHandler () override;
	virtual NUIE::ClipboardHandler&		GetClipboardHandler () override;

	virtual double						GetMouseMoveMinOffset () override;

private:
	NE::BasicStringConverter		stringConverter;
	NUIE::BasicSkinParams			skinParams;
	SDL2Context						drawingContext;
	AppEventHandler					eventHandler;
	NUIE::MemoryClipboardHandler	clipboardHandler;
	NE::EvaluationEnv				evaluationEnv;
	NUIE::NodeEditor*				nodeEditor;
};

class Application
{
public:
	Application (SDL_Window* window, SDL_Renderer* renderer);

	SDL_Rect				GetWindowRect () const;
	void					ResizeWindow (int width, int height);

	NUIE::NodeEditor&		GetNodeEditor ();
	BrowserAsyncInterface&	GetBrowserInterface ();

private:
	SDL_Window*				window;
	SDL_Renderer*			renderer;

	AppUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
	BrowserAsyncInterface	browserInterface;
};

#endif
