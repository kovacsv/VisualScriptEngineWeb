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

Application.prototype.ContextMenuRequest = function (commandsJson)
{
	var commandsJsonStr = UTF8ToString (commandsJson);
	var commandJson = JSON.parse (commandsJsonStr);
	alert (JSON.stringify (commandJson, '\n', 4));
	var contextMenuResponseFunc = this.module.cwrap ('ContextMenuResponse', null, ['number']);
	contextMenuResponseFunc (42);
}
