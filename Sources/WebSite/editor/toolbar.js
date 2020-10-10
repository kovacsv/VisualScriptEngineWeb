var ToolbarButton = function (parentDiv, icon, toolTipText, toolTipSubText)
{
	this.buttonDiv = $('<div>').addClass ('controlbutton').appendTo (parentDiv);
	this.icon = icon;
	this.iconName = 'images/command_icons/' + icon + '.png';
	this.whiteIconName = 'images/command_icons/' + icon + '_White.png';
	this.buttonImg = $('<img>').attr ('src', this.iconName).attr ('alt', toolTipText).appendTo (this.buttonDiv);
	
	this.tooltip = null;
	this.enabled = true;
	this.InitTooltip (toolTipText, toolTipSubText);
};

ToolbarButton.prototype.GetId = function ()
{
	return this.icon;
};

ToolbarButton.prototype.OnClick = function (onClick)
{
	var myThis = this;
	this.buttonDiv.click (function () {
		if (myThis.enabled) {
			onClick ();
		}
	});
};

ToolbarButton.prototype.SetEnabled = function (enabled)
{
	this.enabled = enabled;
	if (this.enabled) {
		this.buttonDiv.removeClass ('disabled');
		this.buttonImg.removeClass ('disabled');
	} else {
		this.buttonDiv.addClass ('disabled');
		this.buttonImg.addClass ('disabled');
		this.HideTooltip ();
	}
};

ToolbarButton.prototype.InitTooltip = function (toolTipText, toolTipSubText)
{
	var myThis = this;
	var hasHover = (window.matchMedia ('(hover : hover)').matches);
	if (hasHover) {
		myThis.buttonDiv.hover (
			function () {
				myThis.ShowTooltip (toolTipText, toolTipSubText);
			},
			function () {
				myThis.HideTooltip ();
			}
		);
	}
};

ToolbarButton.prototype.ShowTooltip = function (toolTipText, toolTipSubText)
{
	if (!this.enabled) {
		return;
	}
	var documentBody = $(document.body);
	this.buttonImg.attr ('src', this.whiteIconName);
	var buttonOffset = this.buttonDiv.offset ();
	this.tooltip = $('<div>').addClass ('tooltip').appendTo (documentBody);
	$('<div>').addClass ('tooltiptext').html (toolTipText).appendTo (this.tooltip);
	if (toolTipSubText != null) {
		$('<div>').addClass ('tooltipsubtext').html (toolTipSubText).appendTo (this.tooltip);
	}
	var topOffset = buttonOffset.top + this.buttonDiv.outerHeight () + 10;
	var leftOffset = buttonOffset.left + this.buttonDiv.outerWidth () / 2 - this.tooltip.outerWidth () / 2;
	if (leftOffset < 5) {
		leftOffset = 5;
	}
	this.tooltip.offset ({
		top : topOffset,
		left : leftOffset
	});
};

ToolbarButton.prototype.HideTooltip = function ()
{
	if (this.tooltip == null) {
		return;
	}				
	this.buttonImg.attr ('src', this.iconName);
	this.tooltip.remove ();	
};

var Toolbar = function (parentDiv)
{
	this.parentDiv = parentDiv;
	this.buttons = [];
};

Toolbar.prototype.AddButton = function (icon, toolTipText, toolTipSubText, onClick)
{
	var toolbarButton = new ToolbarButton (this.parentDiv, icon, toolTipText, toolTipSubText);
	toolbarButton.OnClick (onClick);
	this.buttons.push (toolbarButton);
};

Toolbar.prototype.AddCommandButton = function (app, icon, toolTipText, toolTipSubText, command)
{
	this.AddButton (icon, toolTipText, toolTipSubText, function () {
		app.ExecuteCommand (command);
	});
};

Toolbar.prototype.AddSeparator = function ()
{
	$('<div>').addClass ('controlseparator').appendTo (this.parentDiv);
};

Toolbar.prototype.SetButtonEnabled = function (id, enabled)
{
	var i, toolbarButton;
	for (i = 0; i < this.buttons.length; i++) {
		toolbarButton = this.buttons[i];
		if (toolbarButton.GetId () == id) {
			toolbarButton.SetEnabled (enabled);
		}
	}
};
