import os
import sys
import re
import subprocess

def GetFileContent (fileName):
	file = open (fileName, 'rb')
	content = file.read ()
	file.close ()
	return content

def SetFileContent (fileName, content):
	file = open (fileName, 'wb')
	file.write (content)
	file.close ()
	
def GenerateWhiteIcon (sourceIconPath, whiteSvgIconPath):
	sourceContent = GetFileContent (sourceIconPath)
	whiteSvgContent = sourceContent
	whiteSvgContent = re.sub (r'#[0-9a-fA-F]{6}', '#FAFAFA', whiteSvgContent)
	SetFileContent (whiteSvgIconPath, whiteSvgContent)

def GeneratePng (inkscapePath, sourceSvgPath, resultPngPath, size):
	command = [
		inkscapePath,
		'--export-png=' + resultPngPath,
		'--export-width=' + str (size),
		'--export-height=' + str (size),
		sourceSvgPath
	]
	subprocess.call (command)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	if len (argv) != 2:
		print ('usage: GenerateIcons.py <inkscapePath>')
		return 1

	inkscapePath = sys.argv[1] # "C:\Program Files\Inkscape\inkscape.com"
	
	svgIconsPath = os.path.abspath (os.path.join ('..', 'Documentation', 'CommandIcons'))
	pngIconsPath = os.path.abspath (os.path.join ('..', 'Sources', 'WebSite', 'images', 'command_icons'))
	
	whitePostFix = '_White'
	for svgName in os.listdir (svgIconsPath):
		svgBaseName = os.path.splitext (svgName)[0]
		if whitePostFix in svgBaseName:
			continue
		
		sourceIconPath = os.path.join (svgIconsPath, svgName)
		whiteSvgIconPath = os.path.join (svgIconsPath, svgBaseName + whitePostFix + '.svg')
		GenerateWhiteIcon (sourceIconPath, whiteSvgIconPath)

		png18Path = os.path.join (pngIconsPath, svgBaseName + '.png')
		GeneratePng (inkscapePath, sourceIconPath, png18Path, 18)

		pngWhite18Path = os.path.join (pngIconsPath, svgBaseName + whitePostFix + '.png')
		GeneratePng (inkscapePath, whiteSvgIconPath, pngWhite18Path, 18)

	return 0
	
sys.exit (Main (sys.argv))
