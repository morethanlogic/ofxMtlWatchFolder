//
//  ofxMtlWatchFolder.cpp
//  ofxMtlWatchFolder
//
//  Created by Elie Zananiri on 12-06-17.
//  Copyright (c) 2012 Departement. All rights reserved.
//

#include "ofxMtlWatchFolder.h"

//--------------------------------------------------------------
ofxMtlWatchFolder::ofxMtlWatchFolder()
{
    ofAddListener(ofEvents().update, this, &ofxMtlWatchFolder::update);
}

//--------------------------------------------------------------
ofxMtlWatchFolder::~ofxMtlWatchFolder()
{
    ofRemoveListener(ofEvents().update, this, &ofxMtlWatchFolder::update);
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::allowExt(const string& ext)
{
    _watchDir.allowExt(ext);
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::start(const string& path, unsigned checkInterval, unsigned sizeInterval)
{
    if (isThreadRunning()) {
        stopThread();
    }
    
    _watchPath = path;
    _checkInterval = checkInterval;
    _sizeInterval = sizeInterval;
    
    _watchDir.listDir(_watchPath);
	_watchDir.sort();
    
	// allocate one entry per file in the map
    _watchFiles.clear();
    for (int i = 0; i < _watchDir.size(); i++) {
        _watchFiles[_watchDir.getName(i)].done = false;
        _watchFiles[_watchDir.getName(i)].flag = false;
        _watchFiles[_watchDir.getName(i)].size = _watchDir.getFile(i).getSize();
        _watchFiles[_watchDir.getName(i)].time = ofGetElapsedTimeMillis();
    }
    
    startThread(false, false);
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::stop()
{
    stopThread();
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::threadedFunction()
{
    while (isThreadRunning()) {
        if (lock()) {
            checkFolder();
            unlock();
            ofSleepMillis(_checkInterval);
        }
    }
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::checkFolder()
{
    // flag all the current watch files for deletion
    for (map<string, ofxMtlWatchFile>::iterator it = _watchFiles.begin() ; it != _watchFiles.end(); ++it) {
        (*it).second.flag = true;
    }
    
    // retrieve an updated file list
    _watchDir.listDir(_watchPath);
	_watchDir.sort();
    
    for (int i=0; i < _watchDir.size(); i++) {
        string name = _watchDir.getName(i);
        ofFile file = _watchDir.getFile(i);
        unsigned now = ofGetElapsedTimeMillis();

        //ofLogVerbose() << "ofxMtlWatchFolder: Checking file '" << name << "'" << endl;

        // if it's found in the current list, this call will unflag it
        // otherwise, a new unflagged entry will be created
        _watchFiles[name].flag = false;
        
        if (_watchFiles.find(name) == _watchFiles.end()) {
            // file added to folder
            _watchFiles[name].done = false;
            _watchFiles[name].size = file.getSize();
            _watchFiles[name].time = now;
        }
        else if (!_watchFiles[name].done) {
            // file previously added, check for change in size
            if (_watchFiles[name].size != file.getSize()) {
                _watchFiles[name].size = file.getSize();
                _watchFiles[name].time = now;
            }
            else if ((now - _watchFiles[name].time) >= _sizeInterval) {
                ofLogVerbose() << "ofxMtlWatchFolder: File '" << name << "' ADDED!" << endl;
                _watchFiles[name].done = true;
                filesAdded.push_back(name);
            }
        }
    }
    
    // go through the current list and remove any flagged entries
    for (map<string, ofxMtlWatchFile>::iterator it = _watchFiles.begin(); it != _watchFiles.end(); ) {
        if ((*it).second.flag) {
            string name = (*it).first;
            ofLogVerbose() << "ofxMtlWatchFolder: File '" << name << "' REMOVED!" << endl;
            filesRemoved.push_back(name);
            _watchFiles.erase(it++); 
        }
        else {
            ++it;
        }
    }
}

//--------------------------------------------------------------
void ofxMtlWatchFolder::update(ofEventArgs& args)
{
    if (lock()) {
        // send out all the notifications on the main thread
        for (int i = 0; i < filesAdded.size(); i++) {
            ofNotifyEvent(fileAdded, filesAdded[i], this);
        }
        filesAdded.clear();
        
        for (int i = 0; i < filesRemoved.size(); i++) {
            ofNotifyEvent(fileRemoved, filesRemoved[i], this);
        }
        filesRemoved.clear();
        
        unlock();
    }
}
