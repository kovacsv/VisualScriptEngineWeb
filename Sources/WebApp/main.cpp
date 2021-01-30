#include "Application.hpp"
#include "NE_MemoryStream.hpp"
#include "BI_BuiltInNodes.hpp"

#include <locale>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

static const NUIE::BasicSkinParams& GetAppSkinParams ()
{
	static const NUIE::BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (255, 255, 255),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*connectionMarker */ NUIE::SkinParams::ConnectionMarker::None,
		/*connectionMarkerSize*/ NUIE::Size (8.0, 8.0),
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
		/*slotMarker*/ NUIE::SkinParams::SlotMarker::None,
		/*hiddenSlotMarker*/ NUIE::SkinParams::HiddenSlotMarker::Arrow,
		/*slotMarkerSize*/ NUIE::Size (8.0, 8.0),
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
		/*groupPadding*/ 12.0
	);
	return skinParams;
}

class MyAppInterface : public CustomAppInterface
{
public:
	MyAppInterface () :
		evaluationEnv (nullptr)
	{

	}

	virtual void BuildNodeTree (AppNodeTree& nodeTree) const override
	{
		size_t inputs = nodeTree.AddGroup (L"Inputs");
		nodeTree.AddItem (inputs, L"Boolean", L"Boolean.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true));
		});
		nodeTree.AddItem (inputs, L"Integer", L"Integer.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1));
		});
		nodeTree.AddItem (inputs, L"Number", L"Double.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0));
		});
		nodeTree.AddItem (inputs, L"Integer Increment", L"IntegerIncremented.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
		});
		nodeTree.AddItem (inputs, L"Number Increment", L"DoubleIncremented.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
		});
		nodeTree.AddItem (inputs, L"Number Distribution", L"DoubleDistributed.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position));
		});

		size_t arithmetics = nodeTree.AddGroup (L"Arithmetics");
		nodeTree.AddItem (arithmetics, L"Addition", L"Addition.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
		});
		nodeTree.AddItem (arithmetics, L"Subtraction", L"Subtraction.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
		});
		nodeTree.AddItem (arithmetics, L"Multiplication", L"Multiplication.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
		});
		nodeTree.AddItem (arithmetics, L"Division", L"Division.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
		});
		nodeTree.AddItem (arithmetics, L"Floor", L"Floor.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::FloorNode (NE::LocString (L"Floor"), position));
		});
		nodeTree.AddItem (arithmetics, L"Ceil", L"Ceil.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::CeilNode (NE::LocString (L"Ceil"), position));
		});
		nodeTree.AddItem (arithmetics, L"Abs", L"Abs.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AbsNode (NE::LocString (L"Abs"), position));
		});
		nodeTree.AddItem (arithmetics, L"Negative", L"Negative.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::NegativeNode (NE::LocString (L"Negative"), position));
		});
		nodeTree.AddItem (arithmetics, L"Sqrt", L"Sqrt.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SqrtNode (NE::LocString (L"Sqrt"), position));
		});

		size_t other = nodeTree.AddGroup (L"Other");
		nodeTree.AddItem (other, L"List Builder", L"ListBuilder.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position));
		});
		nodeTree.AddItem (other, L"Viewer", L"Viewer.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5));
		});
	}

	virtual const NUIE::SkinParams& GetSkinParams () const override
	{
		return GetAppSkinParams ();
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnValuesRecalculated () override
	{
#ifdef EMSCRIPTEN
		std::string resultString = "Values Recalculated";
		EM_ASM ({
			OnValuesRecalculated ($0);
		}, resultString.c_str ());
#endif
	}

	virtual void OnSelectionChanged (bool hasSelection) override
	{
#ifdef EMSCRIPTEN
		EM_ASM ({
			OnSelectionChanged ($0);
		}, hasSelection);
#else
		(void) hasSelection;
#endif
	}

	virtual void OnUndoStateChanged (bool canUndo, bool canRedo) override
	{
#ifdef EMSCRIPTEN
		EM_ASM ({
			OnUndoStateChanged ($0, $1);
		}, canUndo, canRedo);
#else
		(void) canUndo;
		(void) canRedo;
#endif
	}

	virtual void OnClipboardStateChanged (bool hasContent) override
	{
#ifdef EMSCRIPTEN
		EM_ASM ({
			OnClipboardStateChanged ($0);
		}, hasContent);
#else
		(void) hasContent;
#endif
	}

	virtual bool SaveFile (NUIE::NodeEditor& nodeEditor, std::vector<char>& buffer) const override
	{
		NE::MemoryOutputStream outputStream;
		if (!nodeEditor.Save (outputStream)) {
			return false;
		}
		buffer = outputStream.GetBuffer ();
		return true;
	}

	virtual bool OpenFile (const std::vector<char>& buffer, NUIE::NodeEditor& nodeEditor) override
	{
		NE::MemoryInputStream inputStream (buffer);
		return nodeEditor.Open (inputStream);
	}

	virtual bool ConvertParameterValueToJson (const NUIE::ParameterInterfacePtr&, size_t, rapidjson::Value&, rapidjson::Document::AllocatorType&) const override
	{
		return false;
	}

	virtual NE::ValuePtr ConvertParameterJsonToValue (const rapidjson::Value&, NUIE::ParameterInterfacePtr&, size_t) const override
	{
		return nullptr;
	}

private:
	NE::EvaluationEnv evaluationEnv;
};

int main (int, char**)
{
	MyAppInterface appInterface;
	Application app (appInterface);
	app.Init ("VisualScriptEngineWeb");

#ifndef EMSCRIPTEN
	app.CreateNode (0, 4, 100, 100);
	app.CreateNode (2, 1, 400, 300);
#endif

	app.Start ();
	app.Shut ();

	return EXIT_SUCCESS;
}
