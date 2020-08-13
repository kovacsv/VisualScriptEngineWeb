var ParameterSettings = function (parentElement, parameters, onClose)
{
	this.parentElement = parentElement;
	this.parameters = parameters;
	this.onClose = onClose;
	this.changedParams = {
		params : []		
	};
	this.result = null;
	
	var myThis = this;
	this.popUpDiv = new PopUpDiv ({
		onOpen : function () {
		},
		onClose : function () {
			myThis.onClose (myThis.result);
		}
	});
};

ParameterSettings.prototype.Open = function (positionX, positionY)
{
	this.popUpDiv.Open (positionX, positionY);
	var popUpDivElem = this.popUpDiv.GetDiv ();
	popUpDivElem.addClass ('parametersettings');
	$('<div>').html ('Settings').addClass ('parametersettingstitle').appendTo (popUpDivElem);
	this.GenerateTable ();
	this.GenerateButtons ();
	this.popUpDiv.CenterToElement (this.parentElement);

};

ParameterSettings.prototype.GenerateTable = function ()
{
	function AddControl (controlColumn, paramIndex, parameter, changedParams)
	{
		var control = null;
		if (parameter.type == 'boolean') {
			control = $('<select>').appendTo (controlColumn);
			$('<option>').val (0).html ('true').appendTo (control);
			$('<option>').val (1).html ('false').appendTo (control);
			control.val (parameter.value.boolVal ? 0 : 1);
		} else if (parameter.type == 'integer') {
			control = $('<input>').attr ('type', 'text').appendTo (controlColumn);
			control.val (parameter.value.intVal);
		} else if (parameter.type == 'float' || parameter.type == 'double') {
			control = $('<input>').attr ('type', 'text').appendTo (controlColumn);
			control.val (parameter.value.numVal);
		} else if (parameter.type == 'string') {
			control = $('<input>').attr ('type', 'text').appendTo (controlColumn);
			control.val (parameter.value.strVal);
		} else if (parameter.type == 'enumeration') {
			control = $('<select>').appendTo (controlColumn);
			var i, choice;
			for (i = 0; i < parameter.value.choices.length; i++) {
				choice = parameter.value.choices[i];
				$('<option>').val (i).html (choice).appendTo (control);
			}
			control.val (parameter.value.intVal);
		}
		changedParams.params.push (null);
		control.on ('input', function () {
			var value = {};
			if (parameter.type == 'boolean') {
				value.boolVal = (control.val () == 0 ? true : false);
			} else if (parameter.type == 'integer') {
				value.intVal = control.val ();
			} else if (parameter.type == 'float' || parameter.type == 'double') {
				value.numVal = control.val ();
			} else if (parameter.type == 'string') {
				value.strVal = control.val ();
			} else if (parameter.type == 'enumeration') {
				value.intVal = control.val ();
			}
			changedParams.params[paramIndex] = {};
			changedParams.params[paramIndex].value = value;
		});
	}
	
	var popUpDivElem = this.popUpDiv.GetDiv ();
	var table = $('<table>').addClass ('parametertable').appendTo (popUpDivElem);
	var i, parameter, row, nameColumn, controlColumn;
	for (i = 0; i < this.parameters.length; i++) {
		parameter = this.parameters[i];
		row = $('<tr>').appendTo (table);
		nameColumn = $('<td>').html (parameter.name).appendTo (row);
		controlColumn = $('<td>').appendTo (row);
		AddControl (controlColumn, i, parameter, this.changedParams);
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
		myThis.result = myThis.changedParams;
		myThis.popUpDiv.Close ();
	});
};
