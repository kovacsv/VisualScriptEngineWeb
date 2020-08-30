var NodeTree = function (parentDiv, nodeList, onNodeClick)
{
	this.parentDiv = parentDiv;
	this.nodeList = nodeList;
	this.onNodeClick = onNodeClick;
	this.isDragDropEnabled = false;
	this.nodeGroups = null;
};

NodeTree.prototype.BuildAsMenu = function (searchInput)
{
	this.isDragDropEnabled = true;
	this.InitSearchField (searchInput);
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
	var groupId, groupObj, nodeId, nodeObj;
	for (groupId = 0; groupId < this.nodeList.length; groupId++) {
		groupObj = this.nodeList[groupId];
		group = this.AddNodeGroup (groupObj.name);
		for (nodeId = 0; nodeId < groupObj.nodes.length; nodeId++) {
			nodeObj = groupObj.nodes[nodeId];
			this.AddNode (group, nodeObj.icon, nodeObj.name, nodeObj.groupId, nodeObj.nodeId);
		}
	}
};

NodeTree.prototype.InitSearchField = function (searchInput)
{
	var myThis = this;
	searchInput.on ('input', function () {
		var searchText = searchInput.val ().toLowerCase ();
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

NodeTree.prototype.AddNode = function (nodeGroup, nodeIcon, nodeName, groupId, nodeId)
{
	var item = this.CreateItem ('images/node_icons/' + nodeIcon, nodeName);
	item.mainItem.appendTo (nodeGroup.subItemsDiv);

	var node = {
		name : nodeName,
		nodeItem : item
	};
	
	var myThis = this;
	item.mainItem.click (function () {
		myThis.onNodeClick (groupId, nodeId);
	});
	if (this.isDragDropEnabled) {
		item.mainItem.attr ('draggable', 'true');
		item.mainItem.on ('dragstart', function (ev) {
			ev.originalEvent.dataTransfer.setData ('groupid', groupId.toString ());
			ev.originalEvent.dataTransfer.setData ('nodeid', nodeId.toString ());
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
	var imgItem = $('<img>').addClass ('nodetreeicon').attr ('src', imgSrc).appendTo (mainItem);
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

var NodeTreePopUp = function (parentElement, nodeList, onNodeClick)
{
	this.parentElement = parentElement;
	this.nodeList = nodeList;
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
	popUpDivElem.addClass ('nodetreepopup thinscrollbar');
	
	var myThis = this;
	var nodeTree = new NodeTree (popUpDivElem, this.nodeList, function (groupId, nodeId) {
		myThis.popUpDiv.Close ();
		myThis.onNodeClick (groupId, nodeId);
	});
	nodeTree.BuildAsPopUp ();
	this.popUpDiv.FitToElement (this.parentElement);
};
