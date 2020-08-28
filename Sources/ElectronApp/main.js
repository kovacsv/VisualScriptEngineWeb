const { app, BrowserWindow } = require ('electron');

function CreateWindow ()
{
	// Create the browser window.
	const browser = new BrowserWindow ({
		width : 1000,
		height : 800,
		webPreferences : {
			nodeIntegration: true
		}
	});
	browser.loadFile ('index.html');
	//browser.webContents.openDevTools ();
}

app.whenReady ().then (CreateWindow);
