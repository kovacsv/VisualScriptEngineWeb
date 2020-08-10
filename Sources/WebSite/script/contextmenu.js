var ContextMenu = function (parentElement, commands, onCommand)
{
	this.parentElement = parentElement;
	this.commands = commands;
	this.onCommand = onCommand;
	
	var myThis = this;
	this.modalDiv = new ModalDiv ({
		onOpen : function () {
		},
		onClose : function () {
			myThis.onCommand (-1);
		}
	});
};

ContextMenu.prototype.Open = function (positionX, positionY)
{
	this.modalDiv.Open (positionX, positionY);
	var modalDivElem = this.modalDiv.GetDiv ();
	modalDivElem.addClass ('contextmenu');
	this.AddCommands (modalDivElem, this.commands);
	this.modalDiv.FitToElement (this.parentElement);
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
	var imgItem = $('<img>').addClass ('icon').appendTo (itemDiv);
	if (command.isChecked) {
		imgItem.attr ('src', 'images/check.png');
	} else {
		imgItem.attr ('src', 'images/nocheck.png');
	}
	var textItem = $('<span>').html (command.name).appendTo (itemDiv);
	itemDiv.appendTo (parentDiv);
	
	var myThis = this;
	itemDiv.click (function () {
		myThis.modalDiv.Close ();
		myThis.onCommand (command.id);
	});
};

ContextMenu.prototype.AddGroupCommand = function (parentDiv, command)
{
	var itemDiv = $('<div>').addClass ('contextmenugroupitem');
	var imgItem = $('<img>').addClass ('icon').attr ('src', 'images/arrow.png').appendTo (itemDiv);
	var textItem = $('<span>').html (command.name).appendTo (itemDiv);
	itemDiv.appendTo (parentDiv);
	
	var subItemsDiv = $('<div>').addClass ('contextmenusubitems').appendTo (parentDiv);
	this.AddCommands (subItemsDiv, command.commands);
};
