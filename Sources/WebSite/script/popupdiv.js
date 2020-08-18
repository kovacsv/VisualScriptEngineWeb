var PopUpDiv = function (eventHandlers)
{
	this.eventHandlers = eventHandlers;
	this.mouseDownHandler = null;
	this.popUpDiv = null;
};

PopUpDiv.prototype.Open = function (positionX, positionY)
{
	var documentBody = $(document.body);
	
	this.popUpDiv = $('<div>').addClass ('popup').appendTo (documentBody);
	this.popUpDiv.offset ({
		left : positionX,
		top : positionY
	});
	
	var myThis = this;
	this.mouseDownHandler = function (ev) {
		if (!myThis.IsItemInDiv ($(ev.target))) {
			myThis.Close ();
		}
	};
	documentBody.bind ('mousedown', this.mouseDownHandler);
	
	if (this.eventHandlers.onOpen) {
		this.eventHandlers.onOpen ();
	}
};

PopUpDiv.prototype.Close = function ()
{
	var documentBody = $(document.body);
	documentBody.unbind ('mousedown', this.mouseDownHandler);
	this.popUpDiv.remove ();
	if (this.eventHandlers.onClose) {
		this.eventHandlers.onClose ();
	}	
};

PopUpDiv.prototype.GetDiv = function ()
{
	return this.popUpDiv;
};

PopUpDiv.prototype.FitToElement = function (elem)
{
	var repairOffset = false;
	
	var offset = this.popUpDiv.offset ();
	var width = this.popUpDiv.outerWidth ();
	var height = this.popUpDiv.outerHeight ();
	
	var elemOffset = elem.offset ();
	var elemRight = elemOffset.left + elem.outerWidth ();
	var elemBottom = elemOffset.top + elem.outerHeight ();
	
	if (offset.left + width > elemRight) {
		offset.left = elemRight - width;
		repairOffset = true;
	}
	if (offset.top + height > elemBottom) {
		offset.top = elemBottom - height;
		repairOffset = true;
	}
	if (repairOffset) {
		this.popUpDiv.offset (offset);
	}	
};

PopUpDiv.prototype.CenterToElement = function (elem)
{
	var offset = this.popUpDiv.offset ();
	var width = this.popUpDiv.outerWidth ();
	var height = this.popUpDiv.outerHeight ();
	
	var elemOffset = elem.offset ();
	this.popUpDiv.offset ({
		left : elemOffset.left + (elem.outerWidth () - width) / 2,
		top : elemOffset.top + (elem.outerHeight () - height) / 2,
	});
};

PopUpDiv.prototype.IsItemInDiv = function (item)
{
	if (item[0] == this.popUpDiv[0]) {
		return true;
	}
	
	var parents = item.parents ();
	var i, curr;
	for (i = 0; i < parents.length; i++) {
		curr = parents[i];
		if (curr == this.popUpDiv[0]) {
			return true;
		}
	}
	
	return false;
};
