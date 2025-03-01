#include "ofApp.h"

int total = 6;
string m_words[] = {"X", "C", "U", "F", "S", "L", "Z"};

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    gui.setup("Setup");
    gui.add(guiDots.setup("Dots", 90, 1, 200));
    gui.add(guiSpeed.setup("Speed", 2, 0.1, 10));
    gui.add(guiSize.setup("Size", 0.5, 0.00, 3));
    gui.add(guiMorph.setup("Morph", 0.0, 0.0, 1.0));
    
    font.load("fonts/HelveticaNeueLTStd-Blk.otf", 500, true, true, true);
    m_wordA = m_words[0];
    m_wordB = m_words[1];
    m_next = 1;
    m_wait = 5;
    m_counter = 0;
    timer = 0;
}


//--------------------------------------------------------------
void ofApp::update(){
    line.resize(guiDots);
}

//--------------------------------------------------------------
void ofApp::draw(){
    float time = ofGetElapsedTimef();
    ofSetLineWidth(1.5);
    

    if(!m_stop) {
        timer += 0.1;
        timerMap = ofMap(sin(timer * guiSpeed), -1, 1, 0, 1);
    }
   
    
    /// Animation
    if(timerMap > 0.98 && !m_morphed){
        
        if(m_counter < m_wait && !m_morphed){
            m_counter += 0.1;
            m_stop = true;
        } else {
            m_stop = false;
            m_counter = 0;
            m_morphed = true;
            
            if(m_next == total){
                m_next = 1;
            } else {
                m_next++;
            }
            
            m_wordA = m_words[m_next];
        }
    }
    
    if(timerMap < 0.01 && m_morphed){
        if(m_counter < m_wait && m_morphed){
            m_counter += 0.1;
            m_stop = true;
        } else {
            m_stop = false;
            m_counter = 0;
            m_morphed = false;
            
            if(m_next == total){
                m_next = 1;
            } else {
                m_next++;
            }
            
            m_wordB = m_words[m_next];
        }
    }
    

  
    /// Letters
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
    ofRectangle centerA = font.getStringBoundingBox(m_wordA, 0, 0);
    ofRectangle centerB = font.getStringBoundingBox(m_wordB, 0, 0);
    
    ofVec2f targetCenter(centerA.width, centerA.height);
    ofVec2f endCenter(centerB.width, centerB.height);
    
    ofVec2f interpolatedCenter = targetCenter.getInterpolated(endCenter, timerMap);
    ofTranslate(-interpolatedCenter.x/2, interpolatedCenter.y/2);
    ofTranslate(-40, 0, 0);
    
    vector <vector <vector <ofPoint> > > font_points_start = getStringAsPoints3DMatrix(font, m_wordA, guiDots);
    vector <vector <vector <ofPoint> > > font_points_end = getStringAsPoints3DMatrix(font, m_wordB, guiDots);
    
    for (int c = 0; c < font_points_start.size(); c++){
        vector <vector <ofPoint> > start_lines = font_points_start[c];
        ofVec2f start_centroid;
        ofVec2f target_centroid;
        
        for (int l = 0; l < start_lines.size(); l++){
            vector <ofPoint> line_points = start_lines[l];
            
            for (int p = 0; p < line_points.size(); p++){
                ofPoint current_point = line_points[p];
                ofVec2f target(font_points_end[c][l][p].x, font_points_end[c][l][p].y);
                ofVec2f interpolated = current_point.getInterpolated(target, timerMap);
                ofDrawCircle(interpolated, guiSize);

                if (line[p].size() > ((p + 1)*0.5)) {
                    line[p].getVertices().erase(line[p].getVertices().begin());
                }
                
                line[p].addVertex(interpolated);
                line[p].draw();
            }
        }
    }
    ofPopMatrix();
    
    
    if(!m_hide) gui.draw();
}


//--------------------------------------------------------------
vector <vector <vector <ofPoint> > > ofApp::getStringAsPoints3DMatrix(ofTrueTypeFont & font, string s, int numOfSamples){
    
    vector <vector <vector <ofPoint> > > returned_points;
    vector <ofTTFCharacter> paths = font.getStringAsPoints(s);
    
    int max_perimeter = 0;
    for (int i = 0; i < paths.size(); i++){
        vector <ofPolyline> polylines = paths[i].getOutline();
        for (int j = 0; j < polylines.size(); j++){
            if (polylines[j].getPerimeter() > max_perimeter) max_perimeter = polylines[j].getPerimeter();
        }
    }
    for (int i = 0; i < paths.size(); i++){
        vector <ofPolyline> polylines = paths[i].getOutline();
        vector <vector <ofPoint> > character_points;
        
        for (int j = 0; j < polylines.size(); j++){
            vector <ofPoint> line_points;
            int num_of_points = numOfSamples;
            
            for (int p = 0; p < num_of_points; p++){
                if (character_points.size() < numOfSamples){
                    ofPoint current_point = ofPoint(polylines[j].getPointAtPercent(ofMap(p, 0, num_of_points, 0, 1)));
                    line_points.push_back(current_point);
                }
            }
            character_points.push_back(line_points);
        }
        returned_points.push_back(character_points);
    }
    return returned_points;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g') m_hide = !m_hide;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
