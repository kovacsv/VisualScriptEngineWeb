var editor = new NodeEditor ();

function OnDoubleClick (mouseX, mouseY)
{
	editor.OpenNodeTreePopUp (mouseX, mouseY);
}

function SaveFile (data, size)
{
	editor.SaveFile (data, size);
}		

function ContextMenuRequest (mouseX, mouseY, commandsJsonFromCpp)
{
	var commandsJsonStr = UTF8ToString (commandsJsonFromCpp);
	var commandJson = JSON.parse (commandsJsonStr);
	editor.OpenContextMenu (mouseX, mouseY, commandJson);
}

function ParameterSettingsRequest (parametersJsonFromCpp)
{
	var parametersJsonStr = UTF8ToString (parametersJsonFromCpp);
	var parametersJson = JSON.parse (parametersJsonStr);
	editor.OpenSettingsDialog (parametersJson);
}
