var NodeTree = function (parentDiv, nodeList, settings, onNodeClick)
{
	this.parentDiv = parentDiv;
	this.nodeList = nodeList;
	this.settings = settings;
	this.onNodeClick = onNodeClick;
	
	this.nodeGroups = null;
	this.searchInput = null;
	this.selectedItem = null;
};

NodeTree.prototype.Build = function ()
{
	this.BuildTree ();
	if (this.settings.searchDiv) {
		this.InitSearchField ();
		if (this.settings.selection) {
			this.InitSelection ();
		}
	}
};

NodeTree.prototype.FocusSearchInput = function ()
{
	this.searchInput.focus ();
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

NodeTree.prototype.InitSearchField = function ()
{
	var myThis = this;
	this.searchInput = $('<input>').attr ('type', 'text').attr ('placeholder', 'Search Nodes...').addClass ('nodetreesearch').appendTo (this.settings.searchDiv);
	this.searchInput.on ('input', function () {
		var searchText = myThis.searchInput.val ().toLowerCase ();
		myThis.selectedItem = null;
		var i, j, group, node, foundInGroup;
		for (i = 0; i < myThis.nodeGroups.length; i++) {
			group = myThis.nodeGroups[i];
			foundInGroup = false;
			for (j = 0; j < group.items.length; j++) {
				node = group.items[j];
				node.nodeItem.visible = (node.name.toLowerCase ().indexOf (searchText) != -1);
				if (node.nodeItem.visible) {
					node.nodeItem.mainItem.show ();
					if (myThis.selectedItem == null) {
						myThis.selectedItem = { group : i, item : j };
					}
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
		myThis.UpdateSelection ();
	});
};

NodeTree.prototype.InitSelection = function ()
{
	function IncreaseSelection (nodeGroups, selectedItem, diff)
	{
		var newSelectedItem = { group : selectedItem.group, item : selectedItem.item };
		newSelectedItem.item += diff;
		
		var group, item;
		while (newSelectedItem.group >= 0 && newSelectedItem.group < nodeGroups.length) {
			group = nodeGroups[newSelectedItem.group];
			if (newSelectedItem.item < 0) {
				if (newSelectedItem.group == 0) {
					return selectedItem;
				}
				newSelectedItem.group -= 1;
				newSelectedItem.item = nodeGroups[newSelectedItem.group].items.length - 1;
				continue;
			} else if (newSelectedItem.item >= group.items.length) {
				if (newSelectedItem.group >= nodeGroups.length) {
					return selectedItem;
				}
				newSelectedItem.group += 1;
				newSelectedItem.item = 0;
				continue;
			}
			item = group.items[newSelectedItem.item];
			if (item.nodeItem.visible) {
				return newSelectedItem;
			}
			newSelectedItem.item += diff;
		}
		
		return selectedItem;
	}
	
	this.selectedItem = { group : 0, item : 0 };
	
	var myThis = this;
	this.searchInput.keydown (function (ev) {
		if (myThis.selectedItem == null) {
			return;
		}
		
		if (ev.which == 13) {
			myThis.onNodeClick (myThis.selectedItem.group, myThis.selectedItem.item);
		} else if (ev.which == 38) {
			myThis.selectedItem = IncreaseSelection (myThis.nodeGroups, myThis.selectedItem, -1);
			myThis.UpdateSelection ();
		} else if (ev.which == 40) {
			myThis.selectedItem = IncreaseSelection (myThis.nodeGroups, myThis.selectedItem, 1);
			myThis.UpdateSelection ();
		}
	});
	myThis.UpdateSelection ();
};

NodeTree.prototype.UpdateSelection = function ()
{
	if (!this.settings.selection || this.selectedItem == null) {
		return;
	}
	
	var i, j, group, node;
	for (i = 0; i < this.nodeGroups.length; i++) {
		group = this.nodeGroups[i];
		for (j = 0; j < group.items.length; j++) {
			node = group.items[j];
			if (i == this.selectedItem.group && j == this.selectedItem.item) {
				node.nodeItem.mainItem.addClass ('selected');
			} else {
				node.nodeItem.mainItem.removeClass ('selected');
			}
		}
	}
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
	if (this.settings.dragDrop) {
		item.mainItem.attr ('draggable', 'true');
		item.mainItem.on ('dragstart', function (ev) {
			ev.originalEvent.dataTransfer.setData ('groupid', groupId.toString ());
			ev.originalEvent.dataTransfer.setData ('nodeid', nodeId.toString ());
		});
	}
	
	nodeGroup.items.push (node);
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
		items : []
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
		textItem : textItem,
		visible : true
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
	
	var searchDiv = $('<div>').addClass ('nodetreepopupsearchdiv').appendTo (popUpDivElem);
	
	var myThis = this;
	var nodeTreeSettings = {
		dragDrop : false,
		selection : true,
		searchDiv : searchDiv
	};
	var nodeTree = new NodeTree (popUpDivElem, this.nodeList, nodeTreeSettings, function (groupId, nodeId) {
		myThis.popUpDiv.Close ();
		myThis.onNodeClick (groupId, nodeId);
	});
	nodeTree.Build (false);
	nodeTree.FocusSearchInput ();
	this.popUpDiv.FitToElement (this.parentElement);
};
