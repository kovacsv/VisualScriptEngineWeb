const { app, BrowserWindow } = require ('electron');

function CreateWindow ()
{
	const browser = new BrowserWindow ({
		width : 1000,
		height : 800,
		webPreferences : {
			nodeIntegration: true
		}
	});
	browser.loadFile ('index.html');
	browser.setMenuBarVisibility (false);
	//browser.webContents.openDevTools ();
}

if (require ('electron-squirrel-startup')) {
	return app.quit ();
}

app.whenReady ().then (CreateWindow);
