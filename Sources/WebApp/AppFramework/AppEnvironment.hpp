#ifndef APP_ENVIRONMENT_HPP
#define APP_ENVIRONMENT_HPP

#include "SDL2Context.hpp"
#include "CustomAppInterface.hpp"
#include "BrowserInterface.hpp"
#include "NUIE_NodeEditor.hpp"

class AppEventHandler : public NUIE::EventHandler
{
public:
	AppEventHandler (BrowserInterface* browserInterface);
	virtual ~AppEventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual bool					OnParameterSettings (ParameterSettingsType type, NUIE::ParameterInterfacePtr parameters) override;
	virtual void					OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton mouseButton) override;

private:
	BrowserInterface*			browserInterface;
};

class AppUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	AppUIEnvironment (CustomAppInterface& customAppInterface, BrowserInterface* browserInterface);
	
	void								Init (SDL_Renderer* renderer, NUIE::NodeEditor* nodeEditorPtr);
	void								Shut ();

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
	virtual void						OnSelectionChanged (const NUIE::Selection& selection) override;
	virtual void						OnUndoStateChanged (const NUIE::UndoState& undoState) override;
	virtual void						OnClipboardStateChanged (const NUIE::ClipboardState& clipboardState) override;
	virtual void						OnIncompatibleVersionPasted (const NUIE::Version&) override;

private:
	CustomAppInterface&				customAppInterface;
	NE::BasicStringConverter		stringConverter;
	AppEventHandler					eventHandler;
	NUIE::MemoryClipboardHandler	clipboardHandler;
	std::unique_ptr<SDL2Context>	drawingContext;
	NUIE::NodeEditor*				nodeEditor;
};

#endif
