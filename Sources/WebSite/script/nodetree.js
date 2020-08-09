var NodeTree = function (parentDiv, onNodeClick)
{
	this.parentDiv = parentDiv;
	this.onNodeClick = onNodeClick;
	this.tree = null;
};

NodeTree.prototype.Build = function ()
{
	this.tree = [];
	var inputs = this.AddNodeGroup ('Inputs');
	this.AddNode (inputs, 'Integer', 0);
	this.AddNode (inputs, 'Number', 1);
	this.AddNode (inputs, 'Integer Increment', 2);
	this.AddNode (inputs, 'Number Increment', 3);
	this.AddNode (inputs, 'Number Distribution', 4);
	
	var arithmetics = this.AddNodeGroup ('Arithmetics');
	this.AddNode (arithmetics, 'Addition', 5);
	this.AddNode (arithmetics, 'Subtraction', 6);
	this.AddNode (arithmetics, 'Multiplication', 7);
	this.AddNode (arithmetics, 'Division', 8);
	
	var other = this.AddNodeGroup ('Other');
	this.AddNode (other, 'Viewer', 9);	
};

NodeTree.prototype.AddNode = function (nodeGroup, nodeName, nodeIndex)
{
	var item = this.CreateItem ('images/plus.png', nodeName);
	item.mainItem.appendTo (nodeGroup.subItemsDiv);
	
	var myThis = this;
	item.mainItem.click (function () {
		myThis.onNodeClick (nodeIndex);
	});
	
	var node = {
		name : nodeName,
		nodeItem : item
	};
	return node;
};

NodeTree.prototype.AddNodeGroup = function (groupName)
{
	var item = this.CreateItem ('images/folder_opened.png', groupName);
	item.mainItem.appendTo (this.parentDiv);

	var subItemsDiv = $('<div>').addClass ('nodetreesubitems').appendTo (this.parentDiv);
	item.mainItem.click (function () {
		if (subItemsDiv.is (':visible')) {
			subItemsDiv.hide ();
			item.imgItem.attr ('src', 'images/folder_closed.png');
		} else {
			subItemsDiv.show ();
			item.imgItem.attr ('src', 'images/folder_opened.png');
		}
	});
	
	var nodeGroup = {
		name : groupName,
		subItemsDiv : subItemsDiv
	};
	this.tree.push (nodeGroup);
	return nodeGroup;
};

NodeTree.prototype.CreateItem = function (imgSrc, name)
{
	var mainItem = $('<div>').addClass ('nodetreeitem');
	var imgItem = $('<img>').addClass ('icon').attr ('src', imgSrc).appendTo (mainItem);
	var textItem = $('<span>').html (name).appendTo (mainItem);
	var result = {
		mainItem : mainItem,
		imgItem : imgItem,
		textItem : textItem
	};
	return result;
};
