var PopUpDiv = function (eventHandlers)
{
	this.eventHandlers = eventHandlers;
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
	documentBody.bind ('mousedown', function (ev) {
		ev.preventDefault ();
		if (!myThis.IsItemInDiv ($(ev.target))) {
			myThis.Close ();
		}
	});
	
	if (this.eventHandlers.onOpen) {
		this.eventHandlers.onOpen ();
	}
};

PopUpDiv.prototype.Close = function ()
{
	var documentBody = $(document.body);
	this.popUpDiv.remove ();
	documentBody.unbind ('mousedown');
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
	var parentOffset = elem.offset ();
	var parentRight = parentOffset.left + elem.outerWidth ();
	var parentBottom = parentOffset.top + elem.outerHeight ();
	if (offset.left + width > parentRight) {
		offset.left = parentRight - width;
		repairOffset = true;
	}
	if (offset.top + height > parentBottom) {
		offset.top = parentBottom - height;
		repairOffset = true;
	}
	if (repairOffset) {
		this.popUpDiv.offset (offset);
	}	
};

PopUpDiv.prototype.IsItemInDiv = function (item)
{
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
