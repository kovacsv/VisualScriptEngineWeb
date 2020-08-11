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
		var positionX = myThis.canvas.width () / 2.0;
		var positionY = myThis.canvas.height () / 2.0;
		myThis.CreateNode (nodeIndex, positionX, positionY);
	});
	nodeTree.BuildWithSearch ();
};

Application.prototype.ResizeCanvas = function (width, height)
{
	var resizeWindowFunc = Module.cwrap ('ResizeWindow', null, ['number', 'number']);
	resizeWindowFunc (width, height);
};

Application.prototype.CreateNode = function (nodeIndex, positionX, positionY)
{
	var createNodeFunc = this.module.cwrap ('CreateNode', null, ['number', 'number', 'number']);
	createNodeFunc (nodeIndex, positionX, positionY);
};

Application.prototype.OpenContextMenu = function (mouseX, mouseY, commands)
{
	var positionX = this.canvas.offset ().left + mouseX
	var positionY = this.canvas.offset ().top + mouseY;	
	
	var module = this.module;
	var contextMenu = new ContextMenu (this.canvas, commands.commands, function (commandId) {
		var contextMenuResponseFunc = module.cwrap ('ContextMenuResponse', null, ['number']);
		contextMenuResponseFunc (commandId);			
	});
	contextMenu.Open (positionX, positionY);
};

Application.prototype.OpenNodeTreePopUp = function (mouseX, mouseY)
{
	var positionX = this.canvas.offset ().left + mouseX
	var positionY = this.canvas.offset ().top + mouseY;		
	
	var myThis = this;
	var nodeTreePopUp = new NodeTreePopUp (this.canvas, function (nodeIndex) {
		myThis.CreateNode (nodeIndex, mouseX, mouseY);
	});
	nodeTreePopUp.Open (positionX, positionY);
};
