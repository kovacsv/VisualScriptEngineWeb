var EditorInterface = function (module)
{
	this.resizeWindowFunc = module.cwrap ('ResizeWindow', null, ['number', 'number']);
	this.executeCommandFunc = module.cwrap ('ExecuteCommand', null, ['string']);
	this.openFileFunc = module.cwrap ('OpenFile', null, ['number', 'number']);
	this.createNodeFunc = module.cwrap ('CreateNode', null, ['number', 'number', 'number']);
	this.contextMenuResponseFunc = module.cwrap ('ContextMenuResponse', null, ['number']);
	this.parameterSettingsResponseFunc = module.cwrap ('ParameterSettingsResponse', null, ['string']);
	this.needToSaveFunc = module.cwrap ('NeedToSave', 'bool', []);
};

EditorInterface.prototype.ResizeWindow = function (width, height)
{
	this.resizeWindowFunc (width, height);
};

EditorInterface.prototype.ExecuteCommand = function (command)
{
	this.executeCommandFunc (command);
};

EditorInterface.prototype.OpenFile = function (buffer, size)
{
	this.openFileFunc (buffer, size);
};

EditorInterface.prototype.CreateNode = function (nodeIndex, positionX, positionY)
{
	this.createNodeFunc (nodeIndex, positionX, positionY);
};

EditorInterface.prototype.ContextMenuResponse = function (commandId)
{
	this.contextMenuResponseFunc (commandId);
};

EditorInterface.prototype.ParameterSettingsResponse = function (responseStr)
{
	this.parameterSettingsResponseFunc (responseStr);
};

EditorInterface.prototype.NeedToSave = function ()
{
	return this.needToSaveFunc ();
};
