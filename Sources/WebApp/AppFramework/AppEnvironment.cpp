#include "AppEnvironment.hpp"

AppEventHandler::AppEventHandler (BrowserInterface* browserInterface) :
	browserInterface (browserInterface)
{

}

AppEventHandler::~AppEventHandler ()
{

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (ContextMenuType, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

bool AppEventHandler::OnParameterSettings (ParameterSettingsType, NUIE::ParameterInterfacePtr parameters)
{
	return browserInterface->ParameterSettingsRequest (parameters);
}

void AppEventHandler::OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton)
{
	browserInterface->DoubleClickRequest (position);
}

AppUIEnvironment::AppUIEnvironment (CustomAppInterface& customAppInterface, BrowserInterface* browserInterface) :
	NUIE::NodeUIEnvironment (),
	customAppInterface (customAppInterface),
	stringConverter (NE::GetDefaultStringConverter ()),
	eventHandler (browserInterface),
	clipboardHandler (),
	drawingContext (nullptr),
	nodeEditor (nullptr)
{

}

void AppUIEnvironment::Init (SDL_Renderer* renderer, NUIE::NodeEditor* nodeEditorPtr)
{
	drawingContext.reset (new SDL2Context (renderer, "Assets/OpenSans-Regular.ttf"));
	nodeEditor = nodeEditorPtr;
}

void AppUIEnvironment::Shut ()
{
	drawingContext.reset (nullptr);
}

const NE::StringConverter& AppUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const NUIE::SkinParams& AppUIEnvironment::GetSkinParams ()
{
	return customAppInterface.GetSkinParams ();
}

NUIE::DrawingContext& AppUIEnvironment::GetDrawingContext ()
{
	return *drawingContext;
}

double AppUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv& AppUIEnvironment::GetEvaluationEnv ()
{
	return customAppInterface.GetEvaluationEnv ();
}

void AppUIEnvironment::OnEvaluationBegin ()
{

}

void AppUIEnvironment::OnEvaluationEnd ()
{

}

void AppUIEnvironment::OnValuesRecalculated ()
{
	customAppInterface.OnValuesRecalculated ();
}

void AppUIEnvironment::OnRedrawRequested ()
{
	nodeEditor->Draw ();
}

NUIE::EventHandler& AppUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler& AppUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

void AppUIEnvironment::OnSelectionChanged (const NUIE::Selection& selection)
{
	bool hasSelection = !selection.GetNodes ().IsEmpty ();
	customAppInterface.OnSelectionChanged (hasSelection);
}

void AppUIEnvironment::OnUndoStateChanged (const NUIE::UndoState& undoState)
{
	customAppInterface.OnUndoStateChanged (undoState.CanUndo (), undoState.CanRedo ());
}

void AppUIEnvironment::OnClipboardStateChanged (const NUIE::ClipboardState& clipboardState)
{
	customAppInterface.OnClipboardStateChanged (clipboardState.HasContent ());
}
