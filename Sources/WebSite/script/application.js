var Application = function ()
{
	this.canvas = null;
	this.module = null;
	this.appInterface = null;
};

Application.prototype.InitCanvas = function (canvas)
{
	this.canvas = canvas;
};

Application.prototype.InitDragAndDrop = function ()
{
	var myThis = this;
	this.canvas.on ('dragover', function (ev) {
		ev.preventDefault ();
	});
	this.canvas.on ('drop', function (ev) {
		ev.preventDefault ();
		var mouseX = ev.clientX - myThis.canvas.offset ().left;
		var mouseY = ev.clientY - myThis.canvas.offset ().top;
		var data = ev.originalEvent.dataTransfer.getData ('nodeindex');
		if (data.length > 0) {
			var nodeIndex = parseInt (data);
			myThis.CreateNode (nodeIndex, mouseX, mouseY);
		}
	});
};

Application.prototype.InitModule = function (module)
{
	this.module = module;
	this.appInterface = new AppInterface (this.module);
	this.InitDragAndDrop ();
	this.InitKeyboardEvents ();	
};

Application.prototype.InitKeyboardEvents = function ()
{
	var overCanvas = false;
	
	var myThis = this;
	this.canvas.on ('mouseenter', function (ev) {
		overCanvas = true;
	});
	this.canvas.on ('mouseleave', function (ev) {
		overCanvas = false;
	});
	
	$(window).keydown (function (ev) {
		if (!overCanvas) {
			return;
		}
		var command = null;
		var isControlPressed = ev.ctrlKey;
		if (isControlPressed) {
			if (ev.which == 65) {
				command = 'SelectAll';
			} else if (ev.which == 67) {
				command = 'Copy';
			} else if (ev.which == 86) {
				command = 'Paste';
			} else if (ev.which == 71) {
				command = 'Group';
			} else if (ev.which == 90) {
				command = 'Undo';
			} else if (ev.which == 89) {
				command = 'Redo';
			}
		} else {
			if (ev.which == 8 || ev.which == 46) {
				command = 'Delete';
			} else if (ev.which == 27) {
				command = 'Escape';
			}
		}
		if (command != null) {
			myThis.ExecuteCommand (command);
			ev.preventDefault();
		}
	});
};

Application.prototype.InitControls = function (controlsDivName)
{
	function AddControl (app, parentDiv, icon, title, command)
	{
		var buttonDiv = $('<div>').addClass ('controlbutton').attr ('title', title).appendTo (parentDiv);
		if (icon != null) {
			$('<img>').attr ('src', 'images/command_icons/' + icon).attr ('alt', title).appendTo (buttonDiv);
		} else {
			buttonDiv.html (title);
		}
		buttonDiv.click (function () {
			app.ExecuteCommand (command);
		});
	}
	
	var controlsDiv = $('#' + controlsDivName);
	AddControl (this, controlsDiv, 'New.png', 'New', 'New');
	AddControl (this, controlsDiv, 'Undo.png', 'Undo', 'Undo');
	AddControl (this, controlsDiv, 'Redo.png', 'Redo', 'Redo');
	AddControl (this, controlsDiv, null, 'Copy', 'Copy');
	AddControl (this, controlsDiv, null, 'Paste', 'Paste');
	AddControl (this, controlsDiv, null, 'Delete', 'Delete');
};

Application.prototype.InitNodeTree = function (nodeTreeDivName)
{
	var menuDiv = $('#' + nodeTreeDivName);
	var myThis = this;
	var nodeTree = new NodeTree (menuDiv, function (nodeIndex) {
		var positionX = myThis.canvas.width () / 2.0;
		var positionY = myThis.canvas.height () / 2.0;
		myThis.appInterface.CreateNode (nodeIndex, positionX, positionY);
	});
	nodeTree.BuildAsMenu ();
};

Application.prototype.ExecuteCommand = function (command)
{
	this.appInterface.ExecuteCommand (command);
};

Application.prototype.ResizeCanvas = function (width, height)
{
	this.appInterface.ResizeWindow (width, height);
};

Application.prototype.OpenContextMenu = function (mouseX, mouseY, commands)
{
	var positionX = this.canvas.offset ().left + mouseX;
	var positionY = this.canvas.offset ().top + mouseY;	
	
	var myThis = this;
	var contextMenu = new ContextMenu (this.canvas, commands.commands, function (commandId) {
		myThis.appInterface.ContextMenuResponse (commandId);			
	});
	contextMenu.Open (positionX, positionY);
};

Application.prototype.OpenSettingsDialog = function (parameters)
{
	var positionX = this.canvas.offset ().left + this.canvas.width () / 2;
	var positionY = this.canvas.offset ().top + this.canvas.height () / 2;
	
	var myThis = this;
	var parameterSettings = new ParameterSettings (this.canvas, parameters.parameters, function (changedParameters) {
		var responseString = '';
		if (changedParameters != null) {
			responseString = JSON.stringify (changedParameters);
		}
		myThis.appInterface.ParameterSettingsResponse (responseString);
	});
	parameterSettings.Open (positionX, positionY);
};

Application.prototype.OpenNodeTreePopUp = function (mouseX, mouseY)
{
	var positionX = this.canvas.offset ().left + mouseX;
	var positionY = this.canvas.offset ().top + mouseY;		
	
	var myThis = this;
	var nodeTreePopUp = new NodeTreePopUp (this.canvas, function (nodeIndex) {
		myThis.appInterface.CreateNode (nodeIndex, mouseX, mouseY);
	});
	nodeTreePopUp.Open (positionX, positionY);
};
