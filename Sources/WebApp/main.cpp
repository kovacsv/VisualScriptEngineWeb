#include "Application.hpp"
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

	virtual NUIE::UINodePtr CreateNodeByIndex (int nodeIndex, const NUIE::Point& position) const override
	{
		NUIE::UINodePtr uiNode = nullptr;
		switch (nodeIndex) {
			case 0: uiNode = NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true)); break;
			case 1: uiNode = NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1)); break;
			case 2: uiNode = NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0)); break;
			case 3: uiNode = NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position)); break;
			case 4: uiNode = NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position)); break;
			case 5: uiNode = NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position)); break;
			case 6: uiNode = NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position)); break;
			case 7: uiNode = NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position)); break;
			case 8: uiNode = NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position)); break;
			case 9: uiNode = NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position)); break;
			case 10: uiNode = NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position)); break;
			case 11: uiNode = NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5)); break;
		}
		return uiNode;
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
	app.CreateNode (1, 100, 100);
	app.CreateNode (11, 400, 300);
#endif

	app.Start ();
	app.Shut ();

	return EXIT_SUCCESS;
}
