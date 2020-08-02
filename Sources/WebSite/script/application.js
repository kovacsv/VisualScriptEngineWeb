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
		var mainItem = document.createElement ('div');
		mainItem.className = 'menuitem';
		var imgItem = document.createElement ('img');
		imgItem.src = imgSrc;
		mainItem.appendChild (imgItem);
		var textItem = document.createElement ('span');
		textItem.innerHTML = text;
		mainItem.appendChild (textItem);
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
		menuDiv.appendChild (item.mainItem);
		var menuItems = document.createElement ('div');
		menuItems.className = 'subitems';
		menuDiv.appendChild (menuItems);
		
		item.mainItem.onclick = function () {
			if (menuItems.style.display != 'none') {
				menuItems.style.display = 'none';
				item.imgItem.src = 'images/folder_closed.png';
			} else {
				menuItems.style.display = 'block';
				item.imgItem.src = 'images/folder_opened.png';
			}
		}
		
		return menuItems;
	}
	
	function AddMenuItem (app, menuDiv, text, nodeIndex)
	{
		var item = CreateItem ('images/plus.png', text);
		menuDiv.appendChild (item.mainItem);
		item.mainItem.onclick = function () {
			app.CreateNode (nodeIndex);
		}
	}			
	
	var menuDiv = document.getElementById (menuDivName);
	
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

Application.prototype.ContextMenuRequest = function ()
{
	var contextMenuResponseFunc = this.module.cwrap ('ContextMenuResponse', null, ['number']);
	contextMenuResponseFunc (42);
}
