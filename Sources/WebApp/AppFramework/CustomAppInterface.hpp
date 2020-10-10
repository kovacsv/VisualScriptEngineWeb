#ifndef CUSTOM_APP_INTERFACE_HPP
#define CUSTOM_APP_INTERFACE_HPP

#include "NUIE_NodeEditor.hpp"
#include "JSONConversion.hpp"
#include "AppNodeTree.hpp"

class CustomAppInterface : public ParameterJsonInterface
{
public:
	CustomAppInterface ();
	virtual ~CustomAppInterface ();

	virtual void						BuildNodeTree (AppNodeTree& nodeTree) const = 0;
	virtual const NUIE::SkinParams&		GetSkinParams () const = 0;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () = 0;
	virtual void						OnValuesRecalculated () = 0;
	virtual void						OnSelectionChanged (bool hasSelection) = 0;
	virtual void						OnUndoStateChanged (bool canUndo, bool canRedo) = 0;

	virtual bool						SaveFile (NUIE::NodeEditor& nodeEditor, std::vector<char>& buffer) const = 0;
	virtual bool						OpenFile (const std::vector<char>& buffer, NUIE::NodeEditor& nodeEditor) = 0;
};

#endif
