var SplashScreen = function (mainDivName, imagePath)
{
	this.mainDivName = mainDivName;
	this.imagePath = imagePath;
	this.executor = new DelayedExecutor ();
	this.mainDiv = null;
	this.splashDiv = null;
};

SplashScreen.prototype.Show = function (mainDiv)
{
	this.mainDiv = $('#' + this.mainDivName);
	this.mainDiv.hide ();

	var myThis = this;
	this.executor.Start (function () {
		var windowObj = $(window);
		var bodyObj = $(document.body);
		myThis.splashDiv = $('<div>').addClass ('splashscreen').appendTo (bodyObj);
		$('<img>').attr ('src', myThis.imagePath).appendTo (myThis.splashDiv);
		myThis.splashDiv.offset ({
			left : windowObj.width () / 2 - myThis.splashDiv.outerWidth () / 2,
			top : windowObj.height () / 3 - myThis.splashDiv.outerHeight () / 2
		});
	}, 500);
	
};

SplashScreen.prototype.Hide = function ()
{
	this.executor.Stop ();
	if (this.splashDiv != null) {
		this.splashDiv.remove ();
	}
	this.mainDiv.show ();
};
