var DelayedExecutor = function ()
{
	this.timeout = null;
};

DelayedExecutor.prototype.Start = function (callbackFunction, milliSeconds)
{
	if (this.timeout == null) {
		this.timeout = setTimeout (callbackFunction, milliSeconds);
	}
};

DelayedExecutor.prototype.Stop = function ()
{
	if (this.timeout != null) {
		clearTimeout (this.timeout);
		this.timeout = null;
	}
};
