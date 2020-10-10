function IsOnMac ()
{
	return (window.navigator.platform.indexOf ('Mac') != -1);
}

var NodeEditor = function ()
{
	this.module = null;
	this.canvas = null;
	this.settings = null;
	this.toolbar = null;
	this.editorInterface = null;
};

NodeEditor.prototype.Init = function (module, settings, uiElements)
{
	this.module = module;
	this.canvas = uiElements.canvas;
	this.settings = settings;
	this.editorInterface = new EditorInterface (this.module);

	this.InitToolbar (uiElements.controls);
	this.InitNodeTree (uiElements.nodeTree, uiElements.searchDiv);
	this.InitDragAndDrop ();
	this.InitKeyboardEvents ();	
	this.InitFileInput (uiElements.fileInput);
};

NodeEditor.prototype.InitToolbar = function (controlsDiv)
{
	var controlKeyText = 'Ctrl';
	if (IsOnMac ()) {
		controlKeyText = 'Cmd';
	}

	this.toolbar = new Toolbar (controlsDiv);
	this.toolbar.AddButton ('New', 'New', null, function () {
		window.open ('.', '_blank');
	});
	this.toolbar.AddButton ('Open', 'Open', null, function () {
		myThis.ShowOpenFileDialog ();
	});
	this.toolbar.AddCommandButton (this, 'Save', 'Save', null, 'Save');
	this.toolbar.AddSeparator ();
	this.toolbar.AddCommandButton (this, 'Undo', 'Undo', controlKeyText + '+Z', 'Undo');
	this.toolbar.AddCommandButton (this, 'Redo', 'Redo', controlKeyText + '+Shift+Z', 'Redo');
	this.toolbar.AddSeparator ();
	this.toolbar.AddCommandButton (this, 'NodeSettings', 'Node Settings', null, 'SetParameters');
	this.toolbar.AddCommandButton (this, 'Copy', 'Copy', controlKeyText + '+C', 'Copy');
	this.toolbar.AddCommandButton (this, 'Paste', 'Paste', controlKeyText + '+V', 'Paste');
	this.toolbar.AddCommandButton (this, 'Delete', 'Delete', 'Delete Key', 'Delete');
	this.toolbar.AddSeparator ();
	this.toolbar.AddCommandButton (this, 'Group', 'Group', controlKeyText + '+G', 'Group');
	this.toolbar.AddCommandButton (this, 'Ungroup', 'Ungroup', controlKeyText + '+Shift+G', 'Ungroup');
	
	this.OnSelectionChanged (false);
	this.OnUndoStateChanged (false, false);

	var myThis = this;
	if (this.settings.customCommandCreator) {
		this.settings.customCommandCreator (
			function () {
				myThis.toolbar.AddSeparator ();
			},
			function (icon, toolTipText, onClick) {
				myThis.toolbas.addButton (icon, toolTipText, null, onClick);
			}
		);
	}
	
	window.onbeforeunload = function (ev) {
		if (myThis.editorInterface.NeedToSave ()) {
			event.preventDefault ();
			event.returnValue = '';
		}
	};
};

NodeEditor.prototype.InitNodeTree = function (nodeTreeDiv, searchDiv)
{
	var myThis = this;
	var nodeTreeSettings = {
		groups : true,
		dragDrop : true,
		selection : false,
		searchDiv : searchDiv
	};
	var nodeTree = new NodeTree (nodeTreeDiv, this.settings.nodeTree, nodeTreeSettings, function (groupId, nodeId) {
		var positionX = myThis.canvas.width () / 2.0;
		var positionY = myThis.canvas.height () / 2.0;
		myThis.editorInterface.CreateNode (groupId, nodeId, positionX, positionY);
	});
	nodeTree.Build ();
};

NodeEditor.prototype.InitDragAndDrop = function ()
{
	var myThis = this;
	this.canvas.on ('dragover', function (ev) {
		ev.preventDefault ();
	});
	this.canvas.on ('drop', function (ev) {
		ev.preventDefault ();
		var mouseX = ev.clientX - myThis.canvas.offset ().left;
		var mouseY = ev.clientY - myThis.canvas.offset ().top;
		var groupIdData = ev.originalEvent.dataTransfer.getData ('groupid');
		var nodeIdData = ev.originalEvent.dataTransfer.getData ('nodeid');
		if (groupIdData.length > 0 && nodeIdData.length > 0) {
			var groupId = parseInt (groupIdData);
			var nodeId = parseInt (nodeIdData);
			myThis.editorInterface.CreateNode (groupId, nodeId, mouseX, mouseY);
		}
	});
};

NodeEditor.prototype.InitKeyboardEvents = function ()
{
	var overCanvas = false;
	
	var myThis = this;
	this.canvas.on ('mouseenter', function (ev) {
		overCanvas = true;
	});
	this.canvas.on ('mouseleave', function (ev) {
		overCanvas = false;
	});
	
	var isMac = IsOnMac ();
	$(window).keydown (function (ev) {
		if (!overCanvas) {
			return;
		}
		var command = null;
		var isControlPressed = ev.ctrlKey;
		if (isMac) {
			isControlPressed = ev.metaKey;
		}
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
				if (isShiftPressed) {
					command = 'Redo';
				} else {
					command = 'Undo';
				}
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

NodeEditor.prototype.InitFileInput = function (fileInput)
{
	var myThis = this;
	fileInput.attr ('accept', this.settings.fileExtension);
	fileInput.on ('change', function () {
		var files = fileInput.prop('files');
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
		fileInput.val ('');
	});
};

NodeEditor.prototype.ExecuteCommand = function (command)
{
	this.editorInterface.ExecuteCommand (command);
};

NodeEditor.prototype.ResizeCanvas = function (width, height)
{
	this.editorInterface.ResizeWindow (width, height);
};

NodeEditor.prototype.OnSelectionChanged = function (hasSelection)
{
	this.toolbar.SetButtonEnabled ('NodeSettings', hasSelection);
	this.toolbar.SetButtonEnabled ('Copy', hasSelection);
	this.toolbar.SetButtonEnabled ('Delete', hasSelection);
	this.toolbar.SetButtonEnabled ('Group', hasSelection);
	this.toolbar.SetButtonEnabled ('Ungroup', hasSelection);
};

NodeEditor.prototype.OnUndoStateChanged = function (canUndo, canRedo)
{
	this.toolbar.SetButtonEnabled ('Undo', canUndo);
	this.toolbar.SetButtonEnabled ('Redo', canRedo);
};

NodeEditor.prototype.OpenContextMenu = function (mouseX, mouseY, commands)
{
	var positionX = this.canvas.offset ().left + mouseX;
	var positionY = this.canvas.offset ().top + mouseY;	
	
	var myThis = this;
	var contextMenu = new ContextMenu (this.canvas, commands.commands, function (commandId) {
		myThis.editorInterface.ContextMenuResponse (commandId);			
	});
	contextMenu.Open (positionX, positionY);
};

NodeEditor.prototype.OpenSettingsDialog = function (parameters)
{
	var myThis = this;
	var parameterSettings = new ParameterSettings (this.canvas, parameters.parameters, this.settings.customControlCreator, function (changedParameters) {
		var responseString = '';
		if (changedParameters != null) {
			responseString = JSON.stringify (changedParameters);
		}
		myThis.editorInterface.ParameterSettingsResponse (responseString);
	});
	parameterSettings.Open ();
};

NodeEditor.prototype.OpenNodeTreePopUp = function (mouseX, mouseY)
{
	var positionX = this.canvas.offset ().left + mouseX;
	var positionY = this.canvas.offset ().top + mouseY;		
	
	var myThis = this;
	var nodeTreePopUp = new NodeTreePopUp (this.canvas, this.settings.nodeTree, function (groupId, nodeId) {
		myThis.editorInterface.CreateNode (groupId, nodeId, mouseX, mouseY);
	});
	nodeTreePopUp.Open (positionX, positionY);
};

NodeEditor.prototype.ShowOpenFileDialog = function ()
{
	function ShowOpenFileDialog ()
	{
		var file = document.getElementById ('file');
		file.click ();			
	}
	
	if (this.editorInterface.NeedToSave ()) {
		var confirmation = new ConfirmationDialog (this.canvas, {
			title : 'Open New File?',
			text : 'Changes you\'ve made may not be saved.',
			okButtonText : 'Open',
			cancelButtonText : 'Cancel',
			onOk : function () {
				ShowOpenFileDialog ();
			}
		});
		confirmation.Open ();
	} else {
		ShowOpenFileDialog ();
	}
};

NodeEditor.prototype.OpenFile = function (fileBuffer)
{
	var buffer = new Int8Array (fileBuffer);
	var heapPtr = this.module._malloc (buffer.length);
	var heapBuffer = new Int8Array (HEAP8.buffer, heapPtr, buffer.length);
	heapBuffer.set (buffer);
	this.editorInterface.OpenFile (heapBuffer.byteOffset, buffer.length);
	this.module._free (heapBuffer.byteOffset);
};

NodeEditor.prototype.SaveFile = function (data, size)
{
	var dataArr = new Int8Array (size);
	var i;
	for (i = 0; i < size; i++) {
		dataArr[i] = HEAP8[data + i];
	}
	
	var blob = new Blob ([dataArr], {type: "octet/stream"});
	var url = window.URL.createObjectURL (blob);
	DownloadFile (url, 'Untitled' + this.settings.fileExtension);
	window.URL.revokeObjectURL (url);
};
