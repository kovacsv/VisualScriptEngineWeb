#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "NUIE_NodeEditor.hpp"
#include "SDL2Context.hpp"

class AppEventHandler : public NUIE::EventHandler
{
public:
	AppEventHandler ();
	virtual ~AppEventHandler ();

	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& outputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& uiGroup, const NUIE::MenuCommandStructure& commands) override;
	
	virtual void OnDoubleClick (NUIE::MouseButton mouseButton, const NUIE::Point& position) override;
	
	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodePtr& uiNode) override;
	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodeGroupPtr& uiGroup) override;
};


class AppUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	AppUIEnvironment (SDL_Renderer* renderer);
	
	void Init (NUIE::NodeEditor* nodeEditorPtr);

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
	Application (SDL_Renderer* renderer);

	SDL_Renderer*		GetRenderer ();
	NUIE::NodeEditor&	GetNodeEditor ();
private:
	SDL_Renderer*		renderer;

	AppUIEnvironment	uiEnvironment;
	NUIE::NodeEditor	nodeEditor;
};

class BrowserInterface
{
public:
	BrowserInterface ();

	bool	IsInitialized () const;

	void	Init (Application* applicationPtr);
	void	Shut ();

	void	AddNode (int nodeIndex);

private:
	Application* application;
};

#endif
