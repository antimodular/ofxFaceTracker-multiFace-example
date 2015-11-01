#pragma once

/*
 test code for multi frace tracking via ofxfacetracker
 
 if a face was found in the 1st tracker instance use it and draw black rect in it's location.
 now the 2nd tracker can't see a face in that location and has to look for a face anywhere else.

 */
#include "ofxCv.h"

#define USE_THREADED

#ifdef USE_THREADED
#include "ofxFaceTrackerThreaded.h"
#else
#include "ofxFaceTracker.h"
#endif
//https://github.com/kylemcdonald/ofxFaceTracker/issues/24#issuecomment-145899247

using namespace cv;
using namespace ofxCv;

#ifdef USE_THREADED
class ofxFaceTrackerMulti {
#else
    class ofxFaceTrackerMulti{
#endif
    public:
        
        int maxNumOfFaces;
        int amtFound;
        int camWidth, camHeight;
   
        vector<cv::Rect> bRect;
        vector<bool> bFound;
        vector<string> name_active;
        vector<string> name_inactive;
        vector<int> index_active;
        vector<int> wasActive;
        
        vector<ofColor> co;
        cv::Mat buffer,buffer2;
        //ofImage buffer;
        
        bool checkActiveTrackers;
        
#ifdef USE_THREADED
        vector<ofxFaceTrackerThreaded*> trackers_active;
        vector<ofxFaceTrackerThreaded*> trackers_inactive;
#else
        vector<ofxFaceTracker*> trackers_active;
        vector<ofxFaceTracker*> trackers_inactive;
#endif
        

        
        void setup(int _maxFaces, int _camW, int _camH){
            maxNumOfFaces = _maxFaces;
            camWidth = _camW;
            camHeight = _camH;
            
            bRect.resize(maxNumOfFaces);
            bFound.resize(maxNumOfFaces);
            co.resize(maxNumOfFaces);
            
            for(int i=0; i<maxNumOfFaces;i++){
#ifdef USE_THREADED
                trackers_active.push_back(new ofxFaceTrackerThreaded());
#else
                trackers.push_back(new ofxFaceTracker());
#endif
                trackers_active.back()->setup();
                name_active.push_back("ID_"+ofToString(i));
                index_active.push_back(i);
                wasActive.push_back(0);
            }
            
            checkActiveTrackers = true;
            
            co[0] = ofColor(0,0,0);
            co[1] = ofColor(255,0,0);
            co[2] = ofColor(0,255,0);
            co[3] = ofColor(0,0,255);
            
            buffer = Mat(camHeight,camWidth,CV_8UC3);
        }
        
        //void update(cv::Mat image){
          void update(cv::Mat image){
              
            copy(image,buffer);
            
            
            amtFound = 0;
            
              ofLog()<<"trackers_active.size() "<<trackers_active.size();
              
              for(int i=0; i<trackers_active.size();i++){

                  if(wasActive[i] == 1){
                      trackers_active[i]->update(buffer);
                      
                      if(trackers_active[i]->getFound()){
                          wasActive[i] = 1;
                        //    cv::ellipse(buffer, cv::fitEllipse(toCv(trackers_active[i]->getImagePoints())) ,  cvScalar(0,0,0), -1, 8);
                          cv::Rect bRect = cv::boundingRect(toCv(trackers_active[i]->getImagePoints() ));
                          cv::rectangle(buffer, bRect, cvScalar(0, 0, 0), -1, 8, 0);
                          
                          index_active[i] = i;
                          amtFound++;
                      }else{
                          wasActive[i] = -1;
                      }
                  }
                  
              }
              
               copy(buffer,buffer2);
              
              for(int i=0; i<trackers_active.size();i++){
                  
                  if(wasActive[i] == 0){
                      trackers_active[i]->update(buffer2);
                      
                      if(trackers_active[i]->getFound()){
                          wasActive[i] = 1;
                          
                          //    cv::ellipse(buffer, cv::fitEllipse(toCv(trackers_active[i]->getImagePoints())) ,  cvScalar(0,0,0), -1, 8);
                          cv::Rect bRect = cv::boundingRect(toCv(trackers_active[i]->getImagePoints() ));
                          
                          cv::rectangle(buffer2, bRect, cvScalar(255, 0, 0), -1, 8, 0);
                          
                          index_active[i] = i;
                          amtFound++;
                      }
                  } else if(wasActive[i] == -1){
                      wasActive[i] = 0;
                  }
                  
              }
              
              
            
            
              /*
            ofLog()<<"trackers_inactive() "<<trackers_inactive.size();
            
              if(trackers_inactive.size() > 0){
                  int i = 0;
                  //for(int i=0; i<trackers_inactive.size();i++){
                  trackers_inactive[i]->update(buffer);
                  
                  if(trackers_inactive[i]->getFound()){
                      //bRect[i] = cv::boundingRect(toCv( trackers_inactive[i]->getImagePoints() ));
                      //cv::rectangle(buffer, bRect[i] , cvScalar(0, 0, 0), -1, 8, 0);
                      
                      // cv::ellipse(buffer, cv::fitEllipse(toCv(trackers_inactive[i]->getImagePoints())) , cvScalar(0, 0, 0), -1, 8);
                      cv::Rect bRect = cv::boundingRect(toCv(trackers_inactive[i]->getImagePoints() ));
                      
                      cv::rectangle(buffer, bRect, cvScalar(255, 0, 0), -1, 8, 0);
                      
                      
                      
                      //amtFound++;
                      
                      trackers_active.push_back(trackers_inactive[i]);
                      trackers_inactive.erase(trackers_inactive.begin()+i);
                      
                      name_active.push_back(name_inactive[i]);
                      name_inactive.erase(name_inactive.begin()+i);
                  }else{
                     // break;
                  }
                  //}
              }
              
              */
              
            
           
            
        }
        
        void exit() {
            

            for(int i=0; i<trackers_inactive.size();i++){
                trackers_inactive[i]->waitForThread();
            }
            
            for(int i=0; i<trackers_active.size();i++){
                trackers_active[i]->waitForThread();
            }
        }
        
        void draw(){
            
            
        }

        
    protected:
        
    };
