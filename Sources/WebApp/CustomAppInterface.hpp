#ifndef CUSTOM_APP_INTERFACE_HPP
#define CUSTOM_APP_INTERFACE_HPP

#include "NUIE_NodeEditor.hpp"

class CustomAppInterface
{
public:
	CustomAppInterface ();
	virtual ~CustomAppInterface ();

	virtual NE::EvaluationEnv&	GetEvaluationEnv () = 0;
	virtual void				OnValuesRecalculated () = 0;
	virtual NUIE::UINodePtr		CreateNodeByIndex (int nodeIndex, const NUIE::Point& position) const = 0;

	virtual bool				SaveFile (NUIE::NodeEditor& nodeEditor, std::vector<char>& buffer) const = 0;
	virtual bool				OpenFile (const std::vector<char>& buffer, NUIE::NodeEditor& nodeEditor) const = 0;
};

#endif
