function DownloadFile (url, name)
{
	var link = document.createElement ('a');
	document.body.appendChild (link);
	link.href = url;
	link.download = name;
	link.click ();
	document.body.removeChild (link);	
}
