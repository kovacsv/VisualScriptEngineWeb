var Application = function ()
{
	this.module = null;
};

Application.prototype.InitModule = function (module)
{
	this.module = module;
};

Application.prototype.InitMenu = function (menuDivName)
{
	function CreateItem (imgSrc, text)
	{
		var mainItem = $('<div>').addClass ('menuitem');
		var imgItem = $('<img>').attr ('src', imgSrc).appendTo (mainItem);
		var textItem = $('<span>').html (text).appendTo (mainItem);
		var result = {
			mainItem : mainItem,
			imgItem : imgItem,
			textItem : textItem
		};
		return result;
	}

	function AddTitleItem (menuDiv, text)
	{
		var item = CreateItem ('images/folder_opened.png', text);
		item.mainItem.appendTo (menuDiv);
		var menuItems = $('<div>').addClass ('subitems').appendTo (menuDiv);
		
		item.mainItem.click (function () {
			if (menuItems.is (':visible')) {
				menuItems.hide ();
				item.imgItem.attr ('src', 'images/folder_closed.png');
			} else {
				menuItems.show ();
				item.imgItem.attr ('src', 'images/folder_opened.png');
			}
		});
		
		return menuItems;
	}
	
	function AddMenuItem (app, menuDiv, text, nodeIndex)
	{
		var item = CreateItem ('images/plus.png', text);
		item.mainItem.appendTo (menuDiv);
		item.mainItem.click (function () {
			app.CreateNode (nodeIndex);
		});
	}			
	
	var menuDiv = $('#' + menuDivName);
	
	var inputs = AddTitleItem (menuDiv, 'Inputs');
	AddMenuItem (this, inputs, 'Integer', 0);
	AddMenuItem (this, inputs, 'Number', 1);
	AddMenuItem (this, inputs, 'Integer Increment', 2);
	AddMenuItem (this, inputs, 'Number Increment', 3);
	AddMenuItem (this, inputs, 'Number Distribution', 4);
	
	var arithmetics = AddTitleItem (menuDiv, 'Arithmetics');
	AddMenuItem (this, arithmetics, 'Addition', 5);
	AddMenuItem (this, arithmetics, 'Subtraction', 6);
	AddMenuItem (this, arithmetics, 'Multiplication', 7);
	AddMenuItem (this, arithmetics, 'Division', 8);
	
	var other = AddTitleItem (menuDiv, 'Other');
	AddMenuItem (this, other, 'Viewer', 9);
}

Application.prototype.CreateNode = function (nodeIndex)
{
	var createNodeFunc = this.module.cwrap ('CreateNode', null, ['number']);
	createNodeFunc (nodeIndex);
}

Application.prototype.ContextMenuRequest = function (mouseX, mouseY, commandsJson, canvasName)
{
	function ShowContextMenu (module, mouseX, mouseY, commandsJson, canvasName)
	{
		function SendResponse (module, response)
		{
			var contextMenuResponseFunc = module.cwrap ('ContextMenuResponse', null, ['string']);
			contextMenuResponseFunc (response);			
		}
		
		function OpenContextMenu (module, mouseX, mouseY)
		{
			var documentBody = $(document.body);
			var contextMenuDiv = $('<div>').addClass ('contextmenu').appendTo (documentBody);
			contextMenuDiv.css ('left', (canvasDiv.offset ().left + mouseX) + 'px');
			contextMenuDiv.css ('top', (canvasDiv.offset ().top + mouseY) + 'px');
			documentBody.bind ("mousedown", function (event) {
				event.preventDefault ();
				if ($(event.target).parents ('.contextmenu').length == 0) {
					CloseContextMenu ();
					SendResponse (module, '');
				}
			});
			return contextMenuDiv;
		}	
		
		function CloseContextMenu ()
		{
			var documentBody = $(document.body);
			documentBody.unbind ("mousedown");
			contextMenuDiv.remove ();
		}
		
		function AddCommandItems (module, contextMenuDiv, commands)
		{
			function AddCommandItem (module, contextMenuDiv, commandName)
			{
				var itemDiv = $('<div>').addClass ('contextmenuitem').html (commandName)
				itemDiv.appendTo (contextMenuDiv);
				itemDiv.click (function () {
					SendResponse (module, commandName);
					CloseContextMenu ();
				});
				return itemDiv;
			}
			
			function AddGroupItem (module, contextMenuDiv, commandName)
			{
				var itemDiv = $('<div>').addClass ('contextmenugroupitem').html (commandName)
				itemDiv.appendTo (contextMenuDiv);
				return itemDiv;
			}			
			
			var i, command, itemDiv, subItemsDiv;
			for (i = 0; i < commands.length; i++) {
				command = commands[i];
				if (command.commands === undefined) {
					itemDiv = AddCommandItem (module, contextMenuDiv, command.name);
				} else {
					itemDiv = AddGroupItem (module, contextMenuDiv, command.name);
					subItemsDiv = $('<div>').addClass ('contextmenusubitems').appendTo (contextMenuDiv);
					AddCommandItems (module, subItemsDiv, command.commands);
				}
			}
		}
		
		var canvasDiv = $('#' + canvasName);
		var contextMenuDiv = OpenContextMenu (module, mouseX, mouseY);
		AddCommandItems (module, contextMenuDiv, commandsJson.commands);
	}
	
	ShowContextMenu (this.module, mouseX, mouseY, commandsJson, canvasName);
}
