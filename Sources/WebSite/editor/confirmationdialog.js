var ConfirmationDialog = function (parentElement, settings)
{
	this.parentElement = parentElement;
	this.settings = settings;
	
	var myThis = this;
	var keyHandler = function (ev) {
		if (ev.which == 13) {
			myThis.popUpDiv.Close ();
			myThis.settings.onOk ();
		} else if (ev.which == 27) {
			myThis.popUpDiv.Close ();
		}
	};

	var windowObj = $(window);
	this.popUpDiv = new PopUpDiv ({
		onOpen : function () {
			windowObj.bind ('keyup', keyHandler);
		},
		onClose : function () {
			windowObj.unbind ('keyup', keyHandler);
		}
	});
};

ConfirmationDialog.prototype.Open = function ()
{
	this.popUpDiv.Open (0, 0);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('dialog');
	var titleDiv = $('<div>').addClass ('dialogtitle').appendTo (popUpDivElem);
	$('<span>').html (this.settings.title).appendTo (titleDiv);
	$('<div>').addClass ('dialogbody').html (this.settings.text).appendTo (popUpDivElem);
	this.GenerateButtons ();
	this.popUpDiv.CenterToElement (this.parentElement);
};

ConfirmationDialog.prototype.GenerateButtons = function ()
{
	var myThis = this;

	var popUpDivElem = this.popUpDiv.GetDiv ();
	var buttonsDiv = $('<div>').addClass ('dialogbuttons').appendTo (popUpDivElem);
	var okButton = $('<div>').addClass ('dialogbutton').addClass ('ok').html (this.settings.okButtonText).appendTo (buttonsDiv);
	if (this.settings.cancelButtonText) {
		var cancelButton = $('<div>').addClass ('dialogbutton').addClass ('cancel').html (this.settings.cancelButtonText).appendTo (buttonsDiv);
		cancelButton.click (function () {
			myThis.popUpDiv.Close ();
		});	
	}
	
	okButton.click (function () {
		myThis.popUpDiv.Close ();
		myThis.settings.onOk ();
	});
};
