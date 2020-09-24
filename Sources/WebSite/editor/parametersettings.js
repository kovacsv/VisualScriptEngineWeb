function CreateControl (parentDiv, parameter, onChange)
{
	function CreateBoolControl (parentDiv, paramValue, onChange)
	{
		var control = $('<select>').appendTo (parentDiv);
		$('<option>').val (0).html ('true').appendTo (control);
		$('<option>').val (1).html ('false').appendTo (control);
		control.val (paramValue.boolVal ? 0 : 1);
		control.on ('input', function () {
			onChange ({ boolVal : (control.val () == 0 ? true : false) });
		});
	}

	function CreateIntegerControl (parentDiv, paramValue, onChange)
	{
		var control = $('<input>').attr ('type', 'text').appendTo (parentDiv);
		control.val (paramValue.intVal);
		control.on ('input', function () {
			onChange ({ intVal : parseInt (control.val ()) });
		});
	}

	function CreateNumberControl (parentDiv, paramValue, onChange)
	{
		var control = $('<input>').attr ('type', 'text').appendTo (parentDiv);
		control.val (paramValue.numVal);
		control.on ('input', function () {
			onChange ({ numVal : parseFloat (control.val ()) });
		});
	}

	function CreateStringControl (parentDiv, paramValue, onChange)
	{
		var control = $('<input>').attr ('type', 'text').appendTo (parentDiv);
		control.val (paramValue.strVal);
		control.on ('input', function () {
			onChange ({ strVal : control.val () });
		});
	}

	function CreateEnumerationControl (parentDiv, paramValue, onChange)
	{
		var control = $('<select>').appendTo (parentDiv);
		var i, choice;
		for (i = 0; i < paramValue.choices.length; i++) {
			choice = paramValue.choices[i];
			$('<option>').val (i).html (choice).appendTo (control);
		}
		control.val (paramValue.intVal);
		control.on ('input', function () {
			onChange ({ intVal : parseInt (control.val ()) });
		});
	}
	
	if (parameter.type == 'boolean') {
		CreateBoolControl (parentDiv, parameter.value, onChange);
		return true;
	} else if (parameter.type == 'integer') {
		CreateIntegerControl (parentDiv, parameter.value, onChange);
		return true;
	} else if (parameter.type == 'float' || parameter.type == 'double') {
		CreateNumberControl (parentDiv, parameter.value, onChange);
		return true;
	} else if (parameter.type == 'string') {
		CreateStringControl (parentDiv, parameter.value, onChange);
		return true;
	} else if (parameter.type == 'enumeration') {
		CreateEnumerationControl (parentDiv, parameter.value, onChange);
		return true;
	}
	return false;
}

var ParameterSettings = function (parentElement, parameters, customControlCreator, onClose)
{
	this.parentElement = parentElement;
	this.parameters = parameters;
	this.customControlCreator = customControlCreator;
	this.onClose = onClose;
	this.changedParams = {
		params : []		
	};
	this.result = null;
	
	var myThis = this;
	var keyHandler = function (ev) {
		if (ev.which == 13) {
			myThis.result = myThis.changedParams;
			myThis.popUpDiv.Close ();
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
			myThis.onClose (myThis.result);
		}
	});
};

ParameterSettings.prototype.Open = function ()
{
	this.popUpDiv.Open (0, 0);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('dialog');
	var titleDiv = $('<div>').addClass ('dialogtitle').appendTo (popUpDivElem);
	$('<img>').attr ('src', 'images/settings.png').appendTo (titleDiv);
	$('<span>').html ('Settings').appendTo (titleDiv);
	this.GenerateTable ();
	this.GenerateButtons ();
	this.popUpDiv.CenterToElement (this.parentElement);
};

ParameterSettings.prototype.GenerateTable = function ()
{
	function AddControl (controlColumn, paramIndex, parameter, customControlCreator, changedParams)
	{
		function ChangeParameter (value)
		{
			changedParams.params[paramIndex] = {};
			changedParams.params[paramIndex].value = value;
		}
		
		var hasControl = CreateControl (controlColumn, parameter, ChangeParameter);
		if (!hasControl && customControlCreator) {
			hasControl = customControlCreator (controlColumn, parameter, ChangeParameter);
		}
		if (!hasControl) {
			$('<span>').html ('N/A').appendTo (controlColumn);
		}
		changedParams.params.push (null);
	}
	
	var popUpDivElem = this.popUpDiv.GetDiv ();
	var table = $('<table>').addClass ('parametertable').appendTo (popUpDivElem);
	var i, parameter, row, nameColumn, controlColumn;
	for (i = 0; i < this.parameters.length; i++) {
		parameter = this.parameters[i];
		row = $('<tr>').appendTo (table);
		nameColumn = $('<td>').html (parameter.name).appendTo (row);
		controlColumn = $('<td>').appendTo (row);
		AddControl (controlColumn, i, parameter, this.customControlCreator, this.changedParams);
	}
};

ParameterSettings.prototype.GenerateButtons = function ()
{
	var popUpDivElem = this.popUpDiv.GetDiv ();
	var buttonsDiv = $('<div>').addClass ('dialogbuttons').appendTo (popUpDivElem);
	var okButton = $('<div>').addClass ('dialogbutton').addClass ('ok').html ('OK').appendTo (buttonsDiv);
	var cancelButton = $('<div>').addClass ('dialogbutton').addClass ('cancel').html ('Cancel').appendTo (buttonsDiv);
	
	var myThis = this;
	cancelButton.click (function () {
		myThis.popUpDiv.Close ();
	});	
	okButton.click (function () {
		myThis.result = myThis.changedParams;
		myThis.popUpDiv.Close ();
	});
};
