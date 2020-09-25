var NodeEditor = function ()
{
	this.module = null;
	this.canvas = null;
	this.settings = null;
	this.editorInterface = null;
};

NodeEditor.prototype.Init = function (module, settings, uiElements)
{
	this.module = module;
	this.canvas = uiElements.canvas;
	this.settings = settings;
	this.editorInterface = new EditorInterface (this.module);

	this.InitCommands (uiElements.controls);
	this.InitNodeTree (uiElements.nodeTree, uiElements.searchDiv);
	this.InitDragAndDrop ();
	this.InitKeyboardEvents ();	
	this.InitFileInput (uiElements.fileInput);
};

NodeEditor.prototype.InitCommands = function (controlsDiv)
{
	function AddControl (parentDiv, icon, toolTipText, toolTipSubText, onClick)
	{
		var buttonDiv = $('<div>').addClass ('controlbutton').appendTo (parentDiv);
		var iconName = 'images/command_icons/' + icon + '.png';
		var whiteIconName = 'images/command_icons/' + icon + '_White.png';
		var buttonImg = $('<img>').attr ('src', iconName).attr ('alt', toolTipText).appendTo (buttonDiv);
		var documentBody = $(document.body);
		var hasHover = (window.matchMedia ('(hover : hover)').matches);
		if (hasHover) {
			var toolTip = null;
			buttonDiv.hover (
				function () {
					buttonImg.attr ('src', whiteIconName);
					var buttonOffset = buttonDiv.offset ();
					toolTip = $('<div>').addClass ('tooltip').appendTo (documentBody);
					$('<div>').addClass ('tooltiptext').html (toolTipText).appendTo (toolTip);
					if (toolTipSubText != null) {
						$('<div>').addClass ('tooltipsubtext').html (toolTipSubText).appendTo (toolTip);
					}
					var topOffset = buttonOffset.top + buttonDiv.outerHeight () + 10;
					var leftOffset = buttonOffset.left + buttonDiv.outerWidth () / 2 - toolTip.outerWidth () / 2;
					if (leftOffset < 5) {
						leftOffset = 5;
					}
					toolTip.offset ({
						top : topOffset,
						left : leftOffset
					});
				},
				function () {
					buttonImg.attr ('src', iconName);
					toolTip.remove ();
				}
			);
		}
		
		buttonDiv.click (function () {
			onClick ();
		});
	}	
	
	function AddCommandControl (app, parentDiv, icon, toolTipText, toolTipSubText, command)
	{
		AddControl (parentDiv, icon, toolTipText, toolTipSubText, function () {
			app.ExecuteCommand (command);
		});
	}
	
	function AddSeparator (parentDiv)
	{
		$('<div>').addClass ('controlseparator').appendTo (parentDiv);
	}

	function IsOnMac ()
	{
		return (window.navigator.platform.indexOf ('Mac') != -1);
	}
	
	var controlKeyText = 'Ctrl';
	if (IsOnMac ()) {
		controlKeyText = 'Cmd';
	}

	var myThis = this;
	AddControl (controlsDiv, 'New', 'New', null, function () {
		window.open ('.', '_blank');
	});
	AddControl (controlsDiv, 'Open', 'Open', null, function () {
		myThis.ShowOpenFileDialog ();
	});
	AddCommandControl (this, controlsDiv, 'Save', 'Save', null, 'Save');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'Undo', 'Undo', controlKeyText + '+Z', 'Undo');
	AddCommandControl (this, controlsDiv, 'Redo', 'Redo', controlKeyText + '+Shift+Z', 'Redo');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'NodeSettings', 'Node Settings', null, 'SetParameters');
	AddCommandControl (this, controlsDiv, 'Copy', 'Copy', controlKeyText + '+C', 'Copy');
	AddCommandControl (this, controlsDiv, 'Paste', 'Paste', controlKeyText + '+V', 'Paste');
	AddCommandControl (this, controlsDiv, 'Delete', 'Delete', 'Delete Key', 'Delete');
	AddSeparator (controlsDiv);
	AddCommandControl (this, controlsDiv, 'Group', 'Group', controlKeyText + '+G', 'Group');
	AddCommandControl (this, controlsDiv, 'Ungroup', 'Ungroup', controlKeyText + '+Shift+G', 'Ungroup');
	
	if (this.settings.customCommandCreator) {
		this.settings.customCommandCreator (
			function () {
				AddSeparator (controlsDiv);
			},
			function (icon, toolTipText, onClick) {
				AddControl (controlsDiv, icon, toolTipText, null, onClick);
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
	
	$(window).keydown (function (ev) {
		if (!overCanvas) {
			return;
		}
		var command = null;
		var isControlPressed = ev.ctrlKey;
		var isCommandPressed = ev.metaKey;
		var isShiftPressed = ev.shiftKey;
		if (isControlPressed || isCommandPressed) {
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
			okButtonText : 'OK',
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
