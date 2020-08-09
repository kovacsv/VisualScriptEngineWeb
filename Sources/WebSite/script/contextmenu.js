var ContextMenu = function (commands, onCommand)
{
	this.commands = commands;
	this.onCommand = onCommand;
	this.contextMenuDiv = null;
};

ContextMenu.prototype.Open = function (positionX, positionY)
{
	var documentBody = $(document.body);
	
	this.contextMenuDiv = $('<div>').addClass ('contextmenu').appendTo (documentBody);
	this.contextMenuDiv.css ('left', positionX + 'px');
	this.contextMenuDiv.css ('top', positionY + 'px');
	
	var myThis = this;
	documentBody.bind ('mousedown', function (ev) {
		ev.preventDefault ();
		if (!myThis.IsItemInMenu ($(ev.target))) {
			myThis.Close ();
			myThis.onCommand (-1);
		}
	});
	
	this.AddCommands (this.contextMenuDiv, this.commands);
};

ContextMenu.prototype.IsItemInMenu = function (item)
{
	var parents = item.parents ();
	var i, curr;
	for (i = 0; i < parents.length; i++) {
		curr = parents[i];
		if (curr == this.contextMenuDiv[0]) {
			return true;
		}
	}
	return false;
};

ContextMenu.prototype.Close = function ()
{
	var documentBody = $(document.body);
	this.contextMenuDiv.remove ();
	documentBody.unbind ('mousedown');
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
		myThis.Close ();
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
