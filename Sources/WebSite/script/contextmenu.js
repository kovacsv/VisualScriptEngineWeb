var ContextMenu = function (parentElement, commands, onCommand)
{
	this.parentElement = parentElement;
	this.commands = commands;
	this.onCommand = onCommand;
	this.selectedCommandId = -1;
	
	var myThis = this;
	this.popUpDiv = new PopUpDiv ({
		onOpen : function () {
		},
		onClose : function () {
			myThis.onCommand (myThis.selectedCommandId);
		}
	});
};

ContextMenu.prototype.Open = function (positionX, positionY)
{
	this.popUpDiv.Open (positionX, positionY);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('contextmenu');
	this.AddCommands (popUpDivElem, this.commands);
	this.popUpDiv.FitToElement (this.parentElement);
};

ContextMenu.prototype.AddCommands = function (parentDiv, commands)
{
	var i, command, itemDiv, subItemsDiv;
	for (i = 0; i < commands.length; i++) {
		command = commands[i];
		if (command.commands === undefined) {
			this.AddCommand (parentDiv, command);
		} else {
			this.AddGroupCommand (parentDiv, command);
		}
	}
};

ContextMenu.prototype.AddCommand = function (parentDiv, command)
{
	var itemDiv = $('<div>').addClass ('contextmenuitem');
	var imgItem = $('<img>').addClass ('contextmenuicon').appendTo (itemDiv);
	if (command.isChecked) {
		imgItem.attr ('src', 'images/check.png');
	} else {
		imgItem.attr ('src', 'images/nocheck.png');
	}
	var textItem = $('<span>').html (command.name).appendTo (itemDiv);
	itemDiv.appendTo (parentDiv);
	var myThis = this;
	itemDiv.click (function () {
		myThis.selectedCommandId = command.id;
		myThis.popUpDiv.Close ();
	});
};

ContextMenu.prototype.AddGroupCommand = function (parentDiv, command)
{
	var itemDiv = $('<div>').addClass ('contextmenugroupitem');
	var imgItem = $('<img>').addClass ('contextmenuicon').attr ('src', 'images/arrow.png').appendTo (itemDiv);
	var textItem = $('<span>').html (command.name).appendTo (itemDiv);
	itemDiv.appendTo (parentDiv);
	
	var subItemsDiv = $('<div>').addClass ('contextmenusubitems').appendTo (parentDiv);
	this.AddCommands (subItemsDiv, command.commands);
};
