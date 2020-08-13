var ParameterSettings = function (parentElement, parameters)
{
	this.parentElement = parentElement;
	this.parameters = parameters;
	
	var myThis = this;
	this.popUpDiv = new PopUpDiv ({
		onOpen : function () {
		},
		onClose : function () {
		}
	});
};

ParameterSettings.prototype.Open = function (positionX, positionY)
{
	this.popUpDiv.Open (positionX, positionY);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('parametersettings');
	this.GenerateTable ();
	this.GenerateButtons ();
	this.popUpDiv.CenterToElement (this.parentElement);

};

ParameterSettings.prototype.GenerateTable = function ()
{
	function AddControl (controlColumn, parameter)
	{
		if (parameter.type == 'string') {
			$('<input>').attr ('type', 'text').appendTo (controlColumn);
			
		}
	}
	
	var popUpDivElem = this.popUpDiv.GetDiv ();
	var table = $('<table>').addClass ('parametertable').appendTo (popUpDivElem);
	var i, parameter, row, nameColumn, controlColumn;
	for (i = 0; i < this.parameters.length; i++) {
		parameter = this.parameters[i];
		row = $('<tr>').appendTo (table);
		nameColumn = $('<td>').html (parameter.name).appendTo (row);
		controlColumn = $('<td>').appendTo (row);
		AddControl (controlColumn, parameter);
	}
};

ParameterSettings.prototype.GenerateButtons = function ()
{
	var popUpDivElem = this.popUpDiv.GetDiv ();
	var buttonsDiv = $('<div>').addClass ('parameterbuttons').appendTo (popUpDivElem);
	var okButton = $('<div>').addClass ('parameterbutton').html ('OK').appendTo (buttonsDiv);
	var cancelButton = $('<div>').addClass ('parameterbutton').html ('Cancel').appendTo (buttonsDiv);
	
	var myThis = this;
	cancelButton.click (function () {
		myThis.popUpDiv.Close ();
	});	
	okButton.click (function () {
		myThis.popUpDiv.Close ();
	});
};
