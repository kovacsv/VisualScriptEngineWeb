var ContextMenu = function (commands, onCommand)
{
	this.commands = commands;
	this.onCommand = onCommand;
	this.parentMenu = null;
	this.contextMenuDiv = null;
};

ContextMenu.prototype.SetParentMenu = function (parentMenu)
{
	this.parentMenu = parentMenu;
}

ContextMenu.prototype.Open = function (positionX, positionY)
{
	var documentBody = $(document.body);
	
	this.contextMenuDiv = $('<div>').addClass ('contextmenu').appendTo (documentBody);
	this.contextMenuDiv.css ('left', positionX + 'px');
	this.contextMenuDiv.css ('top', positionY + 'px');
	
	if (this.parentMenu == null) {
		var myThis = this;
		documentBody.bind ('mousedown', function (ev) {
			ev.preventDefault ();
			if ($(ev.target).parents ('.contextmenu').length == 0) {
				myThis.Close ();
				myThis.onCommand (-1);
			}
		});
	}
	
	this.AddCommands (this.contextMenuDiv, this.commands);
};

ContextMenu.prototype.Close = function ()
{
	if (this.parentMenu == null) {
		var documentBody = $(document.body);
		documentBody.unbind ("mousedown");
	} else {
		this.parentMenu.Close ();
	}
	this.contextMenuDiv.remove ();
};

ContextMenu.prototype.Hide = function ()
{
	this.contextMenuDiv.remove ();
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
}

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
}

ContextMenu.prototype.AddGroupCommand = function (parentDiv, command)
{
	var itemDiv = $('<div>').addClass ('contextmenugroupitem');
	var imgItem = $('<img>').addClass ('icon').attr ('src', 'images/nocheck.png').appendTo (itemDiv);
	var textItem = $('<span>').html (command.name).appendTo (itemDiv);
	itemDiv.appendTo (parentDiv);
	
	var subItemsDiv = $('<div>').addClass ('contextmenusubitems').appendTo (parentDiv);
	this.AddCommands (subItemsDiv, command.commands);
}
