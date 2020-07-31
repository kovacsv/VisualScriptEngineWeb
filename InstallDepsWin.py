import os
import sys
import re
import urllib2
import subprocess

def DownloadFile (url, targetPath):
	print ('Downloading ' + url)
	response = urllib2.urlopen (url)
	content = response.read ()
	file = open (targetPath, 'wb')
	file.write (content)
	file.close ()
	
def UnzipFile (zipFile, targetFolder):
	print ('Unzipping ' + zipFile)
	subprocess.call (['7z', 'x', zipFile, '-o' + targetFolder])

def CmakeProject (projectPath, cMakePath, buildFolderName):
	print ('CmakeProject ' + projectPath)
	buildFolder = os.path.join (projectPath, buildFolderName)
	os.makedirs (buildFolder)
	prevWorkDir = os.getcwd ()
	os.chdir (buildFolder)
	subprocess.call ([cMakePath, '..', '-G', 'Visual Studio 15 2017 Win64'])
	os.chdir (prevWorkDir)

def BuildSolution (msBuildPath, solutionPath, configuration):
	print ('Building ' + solutionPath)
	subprocess.call ([msBuildPath, solutionPath, '/property:Configuration=' + configuration, '/property:Platform=x64'])

def InstallVisualScriptEngine (targetFolder, cMakePath, msBuildPath, msBuildConfiguration):
	vseName = 'VisualScriptEngine-master'
	vseZipUrl = 'https://github.com/kovacsv/VisualScriptEngine/archive/master.zip'
	vseZipPath = os.path.join (targetFolder, vseName + '.zip')
	vseFolderPath = os.path.join (targetFolder, vseName)
	if not os.path.exists (vseFolderPath):
		DownloadFile (vseZipUrl, vseZipPath)
		UnzipFile (vseZipPath, targetFolder)
		CmakeProject (vseFolderPath, cMakePath, 'Build')
		solutionPath = os.path.join (vseFolderPath, 'Build', 'VisualScriptEngine.sln')
		installProjectPath = os.path.join (vseFolderPath, 'Build', 'INSTALL.vcxproj')
		BuildSolution (msBuildPath, solutionPath, msBuildConfiguration)
		BuildSolution (msBuildPath, installProjectPath, msBuildConfiguration)

def Main (argv):
	if len (argv) != 5:
		print 'usage: installdepswin.py <targetFolder> <cMakePath> <msBuildPath> <msBuildConfiguration>'
		return 1
	
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	targetFolder = sys.argv[1]
	cMakePath = sys.argv[2] # "C:\Program Files\CMake\bin\cmake.exe"
	msBuildPath = sys.argv[3] # "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"
	msBuildConfiguration = sys.argv[4]

	if not os.path.exists (targetFolder):
		os.makedirs (targetFolder)
	
	InstallVisualScriptEngine (targetFolder, cMakePath, msBuildPath, msBuildConfiguration)
	return 0
	
sys.exit (Main (sys.argv))
