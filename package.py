# -*- coding: utf-8 -*-#

# YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
# Copyright © 2004-2020 YaPB Project <yapb@jeefo.net>.
#
# SPDX-License-Identifier: MIT
# 

import os, sys
import locale, subprocess
import pathlib, shutil
import zipfile, tarfile
import datetime, calendar

from github import Github

class ModuleRelease (object):
   def __init__(self):
      print ("Initializing Packaging")
      
      self.workDir = os.path.join (pathlib.Path ().absolute (), 'dist')
      self.distDir = os.path.join (pathlib.Path ().absolute (), 'out')
      self.modDir = os.path.join (self.workDir, 'addons', 'amxmodx', 'modules')
      
      if len (sys.argv) < 2:
         raise Exception('Missing required parameters.')

      self.version = sys.argv[1]
		
      os.makedirs (self.distDir, exist_ok=True)
      os.makedirs (self.modDir, exist_ok=True)
		
      self.outFileWin32 = os.path.join (self.distDir, "yapb-amxx-module-{}-windows.zip".format (self.version))
      self.outFileLinux = os.path.join (self.distDir, "yapb-amxx-module-{}-linux.tar.xz".format (self.version))
      self.outFileMacOS = os.path.join (self.distDir, "yapb-amxx-module-{}-macos.zip".format (self.version))
		
   def unlinkBinaries (self):
      libs = ['yapb_amxx_i386.so', 'yapb_amxx.dll', 'yapb_amxx.dylib']
      
      for lib in libs:
         path = os.path.join (self.modDir, lib)
         
         if os.path.exists (path):
            os.remove (path)
   
   def copyBinary (self, path):
      shutil.copy (path, self.modDir)
      
   def zipDir (self, path, handle):
      length = len (path) + 1
      emptyDirs = []  
      
      for root, dirs, files in os.walk (path):
         emptyDirs.extend ([dir for dir in dirs if os.listdir (os.path.join (root, dir)) == []])  
         
         for file in files:
            filePath = os.path.join (root, file)
            handle.write (filePath, filePath[length:])
            
         for dir in emptyDirs:  
            dirPath = os.path.join (root, dir)
            
            zif = zipfile.ZipInfo (dirPath[length:] + "/")  
            handle.writestr (zif, "")  
            
         emptyDirs = []

   def generateZip (self, dir): 
      zipFile = zipfile.ZipFile (dir, 'w', zipfile.ZIP_DEFLATED)
      zipFile.comment = bytes (self.version, encoding = "ascii")
      
      self.zipDir (self.workDir, zipFile)
      zipFile.close ()
   
   def convertZipToTXZ (self, zip, txz):
      timeshift = int ((datetime.datetime.now () - datetime.datetime.utcnow ()).total_seconds ())
      
      with zipfile.ZipFile (zip) as zipf:
         with tarfile.open (txz, 'w:xz') as tarf:
            for zif in zipf.infolist ():
               tif = tarfile.TarInfo (name = zif.filename)
               tif.size = zif.file_size
               tif.mtime =  calendar.timegm (zif.date_time) - timeshift
               
               tarf.addfile (tarinfo = tif, fileobj = zipf.open (zif.filename))
               
      os.remove (zip)
      
   def generateWin32 (self):
      print ("Generating Win32 ZIP")
      
      binary = os.path.join ('build_x86_win32', 'yapb_amxx.dll')
      
      if not os.path.exists (binary):
         return
         
      self.unlinkBinaries ()
      
      self.copyBinary (binary)
      self.generateZip (self.outFileWin32)
      
   def generateLinux (self):
      print ("Generating Linux TXZ")
      
      binary = os.path.join ('build_x86_linux', 'yapb_amxx_i386.so');
      
      if not os.path.exists (binary):
         return
         
      self.unlinkBinaries ()
      self.copyBinary (binary)
      
      tmpFile = "tmp.zip"
      
      self.generateZip (tmpFile)
      self.convertZipToTXZ (tmpFile, self.outFileLinux)
      
   def generateMac (self):
      print ("Generating macOS ZIP")
      
      binary = os.path.join ('build_x86_macos', 'yapb_amxx.dylib')
      
      if not os.path.exists (binary):
         return
         
      self.unlinkBinaries ()
      self.copyBinary (binary)
      
      self.generateZip (self.outFileMacOS)
      
   def generate (self):
      self.generateWin32 ()
      self.generateLinux ()
      self.generateMac ()
      
   def createRelease (self, repository, version):
      print ("Creating Github Tag")
      
      releases = [self.outFileLinux, self.outFileWin32, self.outFileMacOS]
      
      for release in releases:
         if not os.path.exists (release):
            return
      
      releaseName = "YaPB AMXX Module " + version
      releaseMessage = repository.get_commits()[0].commit.message
      releaseSha = repository.get_commits()[0].sha;

      ghr = repository.create_git_tag_and_release (tag = version, tag_message = version, release_name = releaseName, release_message = releaseMessage, type='commit', object = releaseSha, draft = False)
      
      print ("Uploading packages to Github")
      
      for release in releases:
         ghr.upload_asset( path = release, label = os.path.basename (release)) 
            
   def uploadGithub (self): 
      gh = Github (os.environ["GITHUB_TOKEN"])
      repo = gh.get_repo ("yapb/amxx")
      
      self.createRelease (repo, self.version)
         
release = ModuleRelease ()

release.generate ()
release.uploadGithub ()
