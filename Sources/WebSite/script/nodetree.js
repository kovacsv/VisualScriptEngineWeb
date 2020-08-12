var NodeTree = function (parentDiv, onNodeClick)
{
	this.parentDiv = parentDiv;
	this.onNodeClick = onNodeClick;
	this.isDragDropEnabled = false;
	this.nodeGroups = null;
};

NodeTree.prototype.BuildAsMenu = function ()
{
	this.isDragDropEnabled = true;
	this.AddSearchField ();
	this.BuildTree ();
};

NodeTree.prototype.BuildAsPopUp = function ()
{
	this.isDragDropEnabled = false;
	this.BuildTree ();
};

NodeTree.prototype.BuildTree = function ()
{
	this.nodeGroups = [];
	var inputs = this.AddNodeGroup ('Inputs');
	this.AddNode (inputs, 'Boolean', 0);
	this.AddNode (inputs, 'Integer', 1);
	this.AddNode (inputs, 'Number', 2);
	this.AddNode (inputs, 'Integer Increment', 3);
	this.AddNode (inputs, 'Number Increment', 4);
	this.AddNode (inputs, 'Number Distribution', 5);
	
	var arithmetics = this.AddNodeGroup ('Arithmetics');
	this.AddNode (arithmetics, 'Addition', 6);
	this.AddNode (arithmetics, 'Subtraction', 7);
	this.AddNode (arithmetics, 'Multiplication', 8);
	this.AddNode (arithmetics, 'Division', 9);
	
	var other = this.AddNodeGroup ('Other');
	this.AddNode (other, 'List Builder', 10);	
	this.AddNode (other, 'Viewer', 11);	
};

NodeTree.prototype.AddSearchField = function ()
{
	var searchField = $('<input>').attr ('type', 'text').addClass ('nodetreesearch').attr ('placeholder', 'Search Nodes...').appendTo (this.parentDiv);
	var myThis = this;
	searchField.on ('input', function () {
		var searchText = searchField.val ().toLowerCase ();
		var i, j, group, node, found, foundInGroup;
		for (i = 0; i < myThis.nodeGroups.length; i++) {
			group = myThis.nodeGroups[i];
			foundInGroup = false;
			for (j = 0; j < group.nodes.length; j++) {
				node = group.nodes[j];
				found = (node.name.toLowerCase ().indexOf (searchText) != -1);
				if (found) {
					node.nodeItem.mainItem.show ();
					foundInGroup = true;
				} else {
					node.nodeItem.mainItem.hide ();
				}
			}
			if (foundInGroup) {
				myThis.OpenGroup (group);
				group.groupItem.mainItem.show ();
			} else {
				group.groupItem.mainItem.hide ();
			}
		}
	});
};

NodeTree.prototype.AddNode = function (nodeGroup, nodeName, nodeIndex)
{
	var item = this.CreateItem ('images/plus.png', nodeName);
	item.mainItem.appendTo (nodeGroup.subItemsDiv);

	var node = {
		name : nodeName,
		nodeItem : item
	};
	
	var myThis = this;
	item.mainItem.click (function () {
		myThis.onNodeClick (nodeIndex);
	});
	if (this.isDragDropEnabled) {
		item.mainItem.attr ('draggable', 'true');
		item.mainItem.on ('dragstart', function (ev) {
			ev.originalEvent.dataTransfer.setData ('nodeindex', nodeIndex.toString ());
		});
	}
	
	nodeGroup.nodes.push (node);
	return node;
};

NodeTree.prototype.AddNodeGroup = function (groupName)
{
	var item = this.CreateItem ('images/folder_opened.png', groupName);
	item.mainItem.appendTo (this.parentDiv);

	var subItemsDiv = $('<div>').addClass ('nodetreesubitems').appendTo (this.parentDiv);
	var nodeGroup = {
		name : groupName,
		groupItem : item,
		subItemsDiv : subItemsDiv,
		nodes : []
	};

	var myThis = this;
	item.mainItem.click (function () {
		if (subItemsDiv.is (':visible')) {
			myThis.CloseGroup (nodeGroup);
		} else {
			myThis.OpenGroup (nodeGroup);
		}
	});
	
	this.nodeGroups.push (nodeGroup);
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

NodeTree.prototype.OpenGroup = function (nodeGroup)
{
	nodeGroup.subItemsDiv.show ();
	nodeGroup.groupItem.imgItem.attr ('src', 'images/folder_opened.png');	
};

NodeTree.prototype.CloseGroup = function (nodeGroup)
{
	nodeGroup.subItemsDiv.hide ();
	nodeGroup.groupItem.imgItem.attr ('src', 'images/folder_closed.png');	
};

var NodeTreePopUp = function (parentElement, onNodeClick)
{
	this.parentElement = parentElement;
	this.onNodeClick = onNodeClick;
	
	var myThis = this;
	this.popUpDiv = new PopUpDiv ({
		onOpen : function () {
		},
		onClose : function () {
		}
	});
};

NodeTreePopUp.prototype.Open = function (positionX, positionY)
{
	this.popUpDiv.Open (positionX, positionY);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('nodetreepopup');
	
	var myThis = this;
	var nodeTree = new NodeTree (popUpDivElem, function (nodeIndex) {
		myThis.popUpDiv.Close ();
		myThis.onNodeClick (nodeIndex);
	});
	nodeTree.BuildAsPopUp ();
	this.popUpDiv.FitToElement (this.parentElement);
};
