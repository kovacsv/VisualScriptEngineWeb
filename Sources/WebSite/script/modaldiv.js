var ModalDiv = function (eventHandlers)
{
	this.eventHandlers = eventHandlers;
	this.modalDiv = null;
};

ModalDiv.prototype.Open = function (positionX, positionY)
{
	var documentBody = $(document.body);
	
	this.modalDiv = $('<div>').addClass ('modal').appendTo (documentBody);
	this.modalDiv.offset ({
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

ModalDiv.prototype.Close = function ()
{
	var documentBody = $(document.body);
	this.modalDiv.remove ();
	documentBody.unbind ('mousedown');
	if (this.eventHandlers.onClose) {
		this.eventHandlers.onClose ();
	}	
};

ModalDiv.prototype.GetDiv = function ()
{
	return this.modalDiv;
};

ModalDiv.prototype.FitToElement = function (elem)
{
	var repairOffset = false;
	var offset = this.modalDiv.offset ();
	var width = this.modalDiv.outerWidth ();
	var height = this.modalDiv.outerHeight ();
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
		this.modalDiv.offset (offset);
	}	
};

ModalDiv.prototype.IsItemInDiv = function (item)
{
	var parents = item.parents ();
	var i, curr;
	for (i = 0; i < parents.length; i++) {
		curr = parents[i];
		if (curr == this.modalDiv[0]) {
			return true;
		}
	}
	return false;
};
