var SplashScreen = function (mainDivName, imagePath)
{
	this.mainDivName = mainDivName;
	this.imagePath = imagePath;
	this.mainDiv = null;
	this.splashDiv = null;
	this.timeout = null;
};

SplashScreen.prototype.Show = function (mainDiv)
{
	this.mainDiv = $('#' + this.mainDivName);
	this.mainDiv.hide ();

	var myThis = this;
	this.timeout = window.setTimeout (function () {
		var documentBody = $(document.body);
		myThis.splashDiv = $('<div>').addClass ('splashscreen').appendTo (documentBody);
		$('<img>').attr ('src', myThis.imagePath).appendTo (myThis.splashDiv);
	}, 500);
	
};

SplashScreen.prototype.Hide = function ()
{
	window.clearTimeout (this.timeout);
	if (this.splashDiv != null) {
		this.splashDiv.remove ();
	}
	this.mainDiv.show ();
};
