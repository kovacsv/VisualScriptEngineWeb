var Application = function ()
{
	this.module = null;
};

Application.prototype.InitModule = function (module)
{
	this.module = module;
};

Application.prototype.InitMenu = function (menuDivName)
{
	var menuDiv = $('#' + menuDivName);
	var myThis = this;
	var nodeTree = new NodeTree (menuDiv, function (nodeIndex) {
		myThis.CreateNode (nodeIndex);
	});
	nodeTree.Build ();
}

Application.prototype.ResizeCanvas = function (width, height)
{
	var resizeWindowFunc = Module.cwrap ('ResizeWindow', null, ['number', 'number']);
	resizeWindowFunc (width, height);
}

Application.prototype.CreateNode = function (nodeIndex)
{
	var createNodeFunc = this.module.cwrap ('CreateNode', null, ['number']);
	createNodeFunc (nodeIndex);
}

Application.prototype.ContextMenuRequest = function (positionX, positionY, commands)
{
	var module = this.module;
	var contextMenu = new ContextMenu (commands.commands, function (commandId) {
		var contextMenuResponseFunc = module.cwrap ('ContextMenuResponse', null, ['number']);
		contextMenuResponseFunc (commandId);			
	});
	contextMenu.Open (positionX, positionY);
}
