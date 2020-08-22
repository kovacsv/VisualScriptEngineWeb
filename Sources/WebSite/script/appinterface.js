var AppInterface = function (module)
{
	this.resizeWindowFunc = module.cwrap ('ResizeWindow', null, ['number', 'number']);
	this.executeCommandFunc = module.cwrap ('ExecuteCommand', null, ['string']);
	this.openFileFunc = module.cwrap ('OpenFile', null, ['number', 'number']);
	this.createNodeFunc = module.cwrap ('CreateNode', null, ['number', 'number', 'number']);
	this.contextMenuResponseFunc = module.cwrap ('ContextMenuResponse', null, ['number']);
	this.parameterSettingsResponseFunc = module.cwrap ('ParameterSettingsResponse', null, ['string']);
};

AppInterface.prototype.ResizeWindow = function (width, height)
{
	this.resizeWindowFunc (width, height);
};

AppInterface.prototype.ExecuteCommand = function (command)
{
	this.executeCommandFunc (command);
};

AppInterface.prototype.OpenFile = function (buffer, size)
{
	this.openFileFunc (buffer, size);
};

AppInterface.prototype.CreateNode = function (nodeIndex, positionX, positionY)
{
	this.createNodeFunc (nodeIndex, positionX, positionY);
};

AppInterface.prototype.ContextMenuResponse = function (commandId)
{
	this.contextMenuResponseFunc (commandId);
};

AppInterface.prototype.ParameterSettingsResponse = function (responseStr)
{
	this.parameterSettingsResponseFunc (responseStr);
};
