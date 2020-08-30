#include "Application.hpp"
#include "NE_MemoryStream.hpp"
#include "BI_BuiltInNodes.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

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

		size_t other = nodeTree.AddGroup (L"Other");
		nodeTree.AddItem (other, L"List Builder", L"ListBuilder.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position));
		});
		nodeTree.AddItem (other, L"Viewer", L"Viewer.png", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5));
		});
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

	virtual bool SaveFile (NUIE::NodeEditor& nodeEditor, std::vector<char>& buffer) const override
	{
		NE::MemoryOutputStream outputStream;
		if (!nodeEditor.Save (outputStream)) {
			return false;
		}
		buffer = outputStream.GetBuffer ();
		return true;
	}

	virtual bool OpenFile (const std::vector<char>& buffer, NUIE::NodeEditor& nodeEditor) const override
	{
		NE::MemoryInputStream inputStream (buffer);
		return nodeEditor.Open (inputStream);
	}

private:
	NE::EvaluationEnv evaluationEnv;
};

int main (int, char**)
{
	MyAppInterface appInterface;
	Application app (appInterface);
	app.Init ();

#ifndef EMSCRIPTEN
	app.CreateNode (0, 4, 100, 100);
	app.CreateNode (2, 1, 400, 300);
#endif

	app.Start ();
	app.Shut ();

	return EXIT_SUCCESS;
}
