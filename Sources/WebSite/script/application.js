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

Application.prototype.InitModule = function (module)
{
	this.module = module;
	this.appInterface = new AppInterface (this.module);
	this.InitDragAndDrop ();
	this.InitKeyboardEvents ();	
	this.InitFileInput ();
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
			myThis.appInterface.CreateNode (nodeIndex, mouseX, mouseY);
		}
	});
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
		var isShiftPressed = ev.shiftKey;
		if (isControlPressed) {
			if (ev.which == 65) {
				command = 'SelectAll';
			} else if (ev.which == 67) {
				command = 'Copy';
			} else if (ev.which == 86) {
				command = 'Paste';
			} else if (ev.which == 71) {
				if (isShiftPressed) {
					command = 'Ungroup';
				} else {
					command = 'Group';
				}
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

Application.prototype.InitFileInput = function ()
{
	var myThis = this;
	var file = $('#file');
	file.on ('change', function () {
		var files = file.prop('files');
		if (files.length == 0) {
			return;
		}
		var reader = new FileReader ();
		reader.onloadend = function (ev) {
			if (ev.target.readyState == FileReader.DONE) {
				myThis.OpenFile (ev.target.result);
			}
		};
		reader.readAsArrayBuffer (files[0]);		
		
	});
};

Application.prototype.InitControls = function (controlsDivName)
{
	function AddControl (parentDiv, icon, title, onClick)
	{
		var buttonDiv = $('<div>').addClass ('controlbutton').attr ('title', title).appendTo (parentDiv);
		if (icon != null) {
			$('<img>').attr ('src', 'images/command_icons/' + icon).attr ('alt', title).appendTo (buttonDiv);
		} else {
			buttonDiv.html (title);
		}
		buttonDiv.click (function () {
			onClick ();
		});
	}	
	
	function AddCommandControl (app, parentDiv, icon, title, command)
	{
		AddControl (parentDiv, icon, title, function () {
			app.ExecuteCommand (command);
		});
	}
	
	function AddSeparator (parentDiv)
	{
		$('<div>').addClass ('controlseparator').appendTo (parentDiv);
	}	
	
	var myThis = this;
	var controlsDiv = $('#' + controlsDivName);
	AddCommandControl (this, controlsDiv, 'New.png', 'New \n(Ctrl+N)', 'New');
	AddControl (controlsDiv, 'Open.png', 'Open \n(Ctrl+O)', function () {
		myThis.ShowOpenFileDialog ();
	});
	AddCommandControl (this, controlsDiv, 'Save.png', 'Save \n(Ctrl+S)', 'Save');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'Undo.png', 'Undo \n(Ctrl+Z)', 'Undo');
	AddCommandControl (this, controlsDiv, 'Redo.png', 'Redo \n(Ctrl+Shift+Z)', 'Redo');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'Copy.png', 'Copy\n(Ctrl+C)', 'Copy');
	AddCommandControl (this, controlsDiv, 'Paste.png', 'Paste\n(Ctrl+V)', 'Paste');
	AddCommandControl (this, controlsDiv, 'Delete.png', 'Delete', 'Delete');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'Group.png', 'Group\n(Ctrl+G)', 'Group');
	AddCommandControl (this, controlsDiv, 'Ungroup.png', 'Ungroup\n(Ctrl+Shift+G)', 'Ungroup');
};

Application.prototype.InitNodeTree = function (nodeTreeDivName, searchInputName)
{
	var menuDiv = $('#' + nodeTreeDivName);
	var searchInput = $('#' + searchInputName);
	var myThis = this;
	var nodeTree = new NodeTree (menuDiv, function (nodeIndex) {
		var positionX = myThis.canvas.width () / 2.0;
		var positionY = myThis.canvas.height () / 2.0;
		myThis.appInterface.CreateNode (nodeIndex, positionX, positionY);
	});
	nodeTree.BuildAsMenu (searchInput);
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

Application.prototype.ShowOpenFileDialog = function ()
{
	var file = document.getElementById ('file');
	file.click ();
};

Application.prototype.OpenFile = function (fileBuffer)
{
	var buffer = new Int8Array (fileBuffer);
	var heapPtr = this.module._malloc (buffer.length);
	var heapBuffer = new Int8Array (HEAP8.buffer, heapPtr, buffer.length);
	heapBuffer.set(buffer);
	this.appInterface.OpenFile (heapBuffer.byteOffset, buffer.length);
	this.module._free (heapBuffer.byteOffset);
};

Application.prototype.SaveFile = function (data, size)
{
	var dataArr = new Int8Array (size);
	var i;
	for (i = 0; i < size; i++) {
		dataArr[i] = HEAP8[data + i];
	}
	
	var blob = new Blob([dataArr], {type: "octet/stream"});
	var url = window.URL.createObjectURL (blob);

	var link = document.createElement ('a');
	document.body.appendChild (link);
	link.href = url;
	link.download = 'Untitled.ne';
	link.click ();
	window.URL.revokeObjectURL (url);
	document.body.removeChild (link);	
};
