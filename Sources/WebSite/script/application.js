var Application = function ()
{
	this.canvas = null;
	this.module = null;
};

Application.prototype.InitCanvas = function (canvas)
{
	this.canvas = canvas;
	var myThis = this;
	this.canvas.on ('mouseenter', function () {
		myThis.canvas.focus ();
	});
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
	var createNodeFunc = this.module.cwrap ('CreateNode', null, ['number', 'number', 'number']);
	var positionX = this.canvas.width () / 2.0;
	var positionY = this.canvas.height () / 2.0;
	createNodeFunc (nodeIndex, positionX, positionY);
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
