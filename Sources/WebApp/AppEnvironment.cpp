#include "AppEnvironment.hpp"

static const NUIE::BasicSkinParams& GetAppSkinParams ()
{
	static const NUIE::BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (255, 255, 255),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 14.0),
		/*nodeHeaderTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (41, 127, 255),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (199, 80, 80),
		/*nodeContentTextFont*/ NUIE::Font (L"Arial", 12.0),
		/*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*slotTextColor*/ NUIE::Color (0, 0, 0),
		/*slotTextBackgroundColor*/ NUIE::Color (246, 246, 246),
		/*needToDrawSlotCircles*/ false,
		/*slotCircleSize*/ NUIE::Size (8.0, 8.0),
		/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (41, 127, 255), 0.25),
		/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 1.0),
		/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 3.0),
		/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (146, 152, 155), 1.0),
		/*buttonBackgroundColor*/ NUIE::Color (217, 217, 217),
		/*textPanelTextColor*/ NUIE::Color (0, 0, 0),
		/*textPanelBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*groupNameFont*/ NUIE::Font (L"Arial", 14.0),
		/*groupNameColor*/ NUIE::Color (0, 0, 0),
		/*groupBackgroundColors*/ NUIE::NamedColorSet ({
			{ NE::LocalizeString (L"Blue"), NUIE::Color (160, 200, 240) },
			{ NE::LocalizeString (L"Green"), NUIE::Color (160, 239, 160) },
			{ NE::LocalizeString (L"Red"), NUIE::Color (239, 189, 160) }
			}),
		/*groupPadding*/ 10.0
	);
	return skinParams;
}

AppEventHandler::AppEventHandler (BrowserInterface* browserInterface) :
	browserInterface (browserInterface)
{

}

AppEventHandler::~AppEventHandler ()
{

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return browserInterface->ContextMenuRequest (position, commands);
}

void AppEventHandler::OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton)
{
	browserInterface->DoubleClickRequest (position);
}

bool AppEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodePtr&)
{
	return browserInterface->ParameterSettingsRequest (parameters);
}

bool AppEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr parameters, const NUIE::UINodeGroupPtr&)
{
	return browserInterface->ParameterSettingsRequest (parameters);
}

AppUIEnvironment::AppUIEnvironment (BrowserInterface* browserInterface) :
	NUIE::NodeUIEnvironment (),
	stringConverter (NE::GetDefaultStringConverter ()),
	skinParams (GetAppSkinParams ()),
	eventHandler (browserInterface),
	clipboardHandler (),
	drawingContext (nullptr),
	evaluationEnv (nullptr),
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
	return skinParams;
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
	return evaluationEnv;
}

void AppUIEnvironment::OnEvaluationBegin ()
{

}

void AppUIEnvironment::OnEvaluationEnd ()
{

}

void AppUIEnvironment::OnValuesRecalculated ()
{

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

double AppUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}
