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
void ofxMtlWatchFolder::start(const string& path, int interval)
{
    if (isThreadRunning()) {
        stopThread();
    }
    
    _watchPath = path;
    _checkInterval = interval;
    
    ofDirectory watchDir;
    watchDir.listDir(_watchPath);
	watchDir.sort();
    
	// allocate one entry per file in the map
    _watchFiles.clear();
    for (int i = 0; i < watchDir.size(); i++) {
        _watchFiles[watchDir.getName(i)] = false;
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
    for (map<string, bool>::iterator it = _watchFiles.begin() ; it != _watchFiles.end(); it++) {
        (*it).second = true;
    }
    
    // retrieve an updated file list
    ofDirectory watchDir;
    watchDir.listDir(_watchPath);
	watchDir.sort();
    
    for (int i=0; i < watchDir.size(); i++) {
        string filename = watchDir.getName(i);
        //ofLogVerbose() << "ofxMtlWatchFolder: Checking file '" << filename << "'" << endl;
        
        if (_watchFiles.find(filename) == _watchFiles.end()) {
            // file added
            ofLogVerbose() << "ofxMtlWatchFolder: File '" << filename << "' ADDED!" << endl;
            filesAdded.push_back(filename);
        }
        
        // if it's found in the current list, this call will unflag it
        // otherwise, a new unflagged entry will be created
        // win win!
        _watchFiles[filename] = false;
    }
    
    // go through the current list and remove any flagged entries
    for (map<string, bool>::iterator it = _watchFiles.begin(); it != _watchFiles.end(); ) {
        if ((*it).second) {
            string filename = (*it).first;
            ofLogVerbose() << "ofxMtlWatchFolder: File '" << filename << "' REMOVED!" << endl;
            filesRemoved.push_back(filename);
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
