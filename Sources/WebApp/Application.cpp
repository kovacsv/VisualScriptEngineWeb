#include "Application.hpp"
#include "BI_BuiltInNodes.hpp"

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

AppEventHandler::AppEventHandler ()
{

}

AppEventHandler::~AppEventHandler ()
{

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point&, const NUIE::MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point&, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point&, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point&, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu (const NUIE::Point&, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure&)
{
	return nullptr;
}

void AppEventHandler::OnDoubleClick (NUIE::MouseButton, const NUIE::Point&)
{
	return;
}

bool AppEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr, const NUIE::UINodePtr&)
{
	return false;
}

bool AppEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr, const NUIE::UINodeGroupPtr&)
{
	return false;
}

AppUIEnvironment::AppUIEnvironment (SDL_Renderer* renderer) :
	NUIE::NodeUIEnvironment (),
	stringConverter (NE::GetDefaultStringConverter ()),
	skinParams (GetAppSkinParams ()),
	drawingContext (renderer, "Assets/OpenSans-Regular.ttf"),
	eventHandler (),
	clipboardHandler (),
	evaluationEnv (nullptr),
	nodeEditor (nullptr)
{

}

void AppUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr)
{
	nodeEditor = nodeEditorPtr;
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
	return drawingContext;
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

Application::Application (SDL_Renderer* renderer) :
	renderer (renderer),
	uiEnvironment (renderer),
	nodeEditor (uiEnvironment)
{
	uiEnvironment.Init (&nodeEditor);
	nodeEditor.Update ();

#ifndef EMSCRIPTEN
	nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), NUIE::Point (100.0, 100.0), 0, 1)));
	nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), NUIE::Point (400.0, 300.0), 5)));
#endif
}

SDL_Renderer* Application::GetRenderer ()
{
	return renderer;
}

NUIE::NodeEditor& Application::GetNodeEditor ()
{
	return nodeEditor;
}

BrowserInterface::BrowserInterface ()
{

}

bool BrowserInterface::IsInitialized () const
{
	return application != nullptr;
}

void BrowserInterface::Init (Application* applicationPtr)
{
	application = applicationPtr;
}

void BrowserInterface::Shut ()
{
	application = nullptr;
}

void BrowserInterface::AddNode (int nodeIndex)
{
	SDL_Renderer* renderer = application->GetRenderer ();
	NUIE::NodeEditor& nodeEditor = application->GetNodeEditor ();

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
