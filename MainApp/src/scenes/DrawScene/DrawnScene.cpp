#include "DrawnScene.h"

// ----------------------------------------------------
void DrawnScene::setup() {
    name = "drawn scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(name, safeName+".xml", 20, 20);
    
    gui.add(frequency.setup("frequency", 7, 0.0002, 10));
    gui.add(damping.setup("damping", 1, 0.0003, 1.0));
    
    
    
	// box2d
    box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);
    
    // images
    eyeA.loadImage("graphics/drawn/eye_002.png");
    eyeRatio = (eyeA.getHeight()/eyeA.getWidth());
    
    for(int i=0; i<6; i++) {
        dots.push_back(ofImage());
        dots.back().loadImage("graphics/drawn/dot_"+ofToString(i)+".png");
    }
    
    for(int i=0; i<3; i++) {
        tris.push_back(ofImage());
        tris.back().loadImage("graphics/drawn/triangle_"+ofToString(i)+".png");
    }
    ofDisableArbTex();
    dotRepeatImg.loadImage("graphics/drawn/kusamadot_repeat.png");
    
    /*
	// TOP
    int nVines = 20;
    int ranTriIndex = ofRandomIndex(tris);
    for(int i=0; i<nVines; i++) {
        float x = ofMap(i, 0, nVines-1, 20, CUBE_SCREEN_WIDTH-20);
        Viner v;
        v.triImg = &tris[ranTriIndex];
        v.pullForce.set(0, 2);
        v.make(x, 0, Viner::TOP, box2d);
        v.colorDes = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]; //choice of random complimentary colour
        vines.push_back(v);
    }
  
    
    // BOTTOM
    int nVines = 3;
    int ranTriIndex = ofRandomIndex(tris);
    for(int i=0; i<nVines; i++) {
        float x = ofMap(i, 0, nVines-1, 100, CUBE_SCREEN_WIDTH-100) + ofRandom(-50, 50);
        Viner v;
        v.triImg = &tris[ranTriIndex];
        v.pullForce.set(0, -2);
        v.make(x, CUBE_SCREEN_HEIGHT, Viner::BOTTOM, box2d);
        v.colorDes = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]; //choice of random complimentary colour
        vines.push_back(v);
    }
      */

    addBush(30);
    addBush(500);    
    addBush(CUBE_SCREEN_WIDTH-300);
    
    field.setupField(60, 60, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    field.randomizeField(3);
    field.fadeField(0.8);
    
    bgColor.setHex(0xE1E3D3);
    
    
    // get all the bushes
    for(int i=0; i<vines.size(); i++) {
        
        if(vines[i].type == Viner::LINER) {
            poopVines.push_back(&vines[i]);
        }
    
    }
    
#ifdef USE_SWIRPS
	repel_effect = 0.3;
	follow_effect = 0.001;
#endif
   
    
    // make 3 wigglers...
    for(int i=0; i<4; i++) {
        makeWiggler();
    }
    
    for(int i=0; i<10; i++) {
        makeTinyVine();
    }

}

// ----------------------------------------------------
void DrawnScene::makeWiggler() {
    
    float x = ofRandom(50, CUBE_SCREEN_WIDTH-50);
    float y = CUBE_SCREEN_HEIGHT+10;
    
    Wiggler w;
    w.txt = &dotRepeatImg.getTextureReference();
    w.make(x, y);
    
    
    wigglers.push_back(w);
    
}

// ----------------------------------------------------
void DrawnScene::makeTinyVine() {
    float x     = ofRandom(50, CUBE_SCREEN_WIDTH-50);
    float y     = 0;
    int   side  = BaseScene::TOP;// ofRandom(0, 4);
    for(int i=0; i<5; i++) {
        TinyVine w;
        w.side = side;
        w.make(x+ofRandom(-3,3), y, ofRandom(4, 10));
        w.headImg = &dots[ofRandomIndex(dots)];
        w.colorD  = complimentaryColours[ofRandomIndex(complimentaryColours)]; 
        tinyVines.push_back(w);
    }
}

// ----------------------------------------------------
void DrawnScene::addBush(float startX) {
    int n = ofRandom(3, 10);
    int ranDotImg = ofRandomIndex(dots);
    for(int i=0; i<n; i++) {
        Viner v;
        v.type = Viner::LINER;
        v.dotImg = &dots[ranDotImg];
        v.pullForce.set(0, -2);
        v.make(ofRandom(-10, 10)+startX, CUBE_SCREEN_HEIGHT, Viner::BOTTOM, box2d);
        v.colorDes = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]; //choice of random complimentary colour

        vines.push_back(v);
        
    }
    
}


// ----------------------------------------------------
void DrawnScene::addChasers(float x, float y) {
    if(chasers.size() < MAX_CHASERS) {
        Chaser p;
        p.radius = ofRandom(20, 50);
        p.setPosition(x, y);
        
        chasers.push_back(p);
    }
}

// ----------------------------------------------------
void DrawnScene::makePoop() {
    if(poop.size() < 50) {
        int ranVine  = ofRandomIndex(poopVines);
        
        Viner * vine = poopVines[ranVine];
        if(vine->type == Viner::LINER) {
            int nPoops = 2;
            for(int i=0; i<nPoops; i++) {
                
                
                ofVec2f pt = vine->circles.back().getPosition();
                float   r  = (vine->dotImg->getWidth() * vine->dotSize)/2;
                
                VinePoop p;
                p.setPhysics(0.2, 0.2, 0.9);
                p.setup(box2d.getWorld(), pt.x, pt.y, r);
                p.img = vine->dotImg;
                p.dotSizeD = vine->dotSizeD;
                p.dotSize  = vine->dotSizeD;
                p.colorD = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
                
                poop.push_back(p);
                
            }
            vine->bPop = true;
        }
    }
}

// ----------------------------------------------------
void DrawnScene::update() {

    bgColor.setHex(0xE1E3D3);
    ofRectangle screen(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);

    
    // ----------------------
    // Poop 
    // ----------------------
    for(vector<VinePoop>::iterator it=poop.begin(); it!=poop.end(); ++it) {
        
        ofVec2f frc;
        int nFrc = 0;
        ofVec2f pos = it->getPosition();
        ofVec2f vel = it->getVelocity();
        
        float   r   = it->getRadius();
        for(vector<Chaser>::iterator itC=chasers.begin(); itC!=chasers.end(); ++itC) {
            ofVec2f vec = itC->pos - pos;
            if(vec.length() <  r*2) {
                vec.normalize();
                frc += vec;
                ++nFrc;
            }
        }
        if(nFrc > 0) {
            frc /= nFrc;
        }
        it->updateColor();
        it->addForce(frc, 1);
        field.addVectorCircle(pos.x, pos.y, vel.x, vel.y, r*2, .1);

    }    
    
    
    // ----------------------
    // Vines
    // ----------------------
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        
        for(int i=0; i<it->joints.size(); ++i) {
            it->joints[i].setFrequency(frequency);
            it->joints[i].setDamping(damping);
        }
        
        
        ofVec2f v = getCentreCubeScreen() -  it->circles.back().getPosition();
        v.normalize();
        
       // it->circles.back().addAttractionPoint(ofGetMouse(), .08);
        ofVec2f frc = it->pullForce;
        
        
        int nFrc = 0;
        for(vector<Chaser>::iterator itC=chasers.begin(); itC!=chasers.end(); ++itC) {
            ofVec2f vec = itC->pos - it->getHead()->getPosition();
            if(vec.length() <  220) {
                vec.normalize();
                frc += vec;
                ++nFrc;
            }
        }
        if(nFrc > 0) {
            frc /= nFrc;
        }
        it->circles.back().addForce(frc, 1);
        
        if(it->bPop) {
            
            it->dotSize  = 0;
            it->dotSizeD = ofRandom(.03, 0.2);
            it->dotImg   = &dots[(int)ofRandomIndex(dots)];
            
            it->bPop = false;
            
        }
        
        it->update();

    }   
    
    // ----------------------
    // Chasers
    // ----------------------
    float t = ofGetElapsedTimef()*0.02;
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        ofVec2f pos = it->pos;
        float frwX  = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
        
        ofVec2f noiseFrc;
     	noiseFrc.x = ofSignedNoise(t, pos.y * 0.04) * 0.6;
		noiseFrc.y = ofSignedNoise(it->uniquef, pos.x * 0.006, t);
        noiseFrc *= 0.94;
  
        
        // avoid the vines
        ofVec2f vineFrc = 0;
        int nVecFrc = 0;
        for(vector<Viner>::iterator itV=vines.begin(); itV!=vines.end(); ++itV) {
            ofVec2f vec = itV->getHead()->getPosition() - it->pos;
            if(vec.length() < 60) {
                vec.normalize();
                vineFrc -= vec; // avoid
                ++nVecFrc;
            }
        }    
        if(nVecFrc > 0) {
            vineFrc /= nVecFrc;
            vineFrc *= 0.89;
        }
        
        
        ofVec2f avoidFrc = 0;
        int nAvoidFrc = 0;
        for(vector<Chaser>::iterator other=it; other!=chasers.end(); ++other) {
            ofVec2f vec = other->pos - it->pos;
            float minDis = 50;
            if(vec.length() < minDis) {
                vec.normalize();
                avoidFrc -= vec; // avoid
                ++nAvoidFrc;
            }
        }    
        if(nAvoidFrc > 0) {
            avoidFrc /= nAvoidFrc;
            avoidFrc *= 1.2;
        }
        
        
        // add forces
        it->frc += noiseFrc;        
        it->frc += vineFrc;
        it->frc += avoidFrc;
        it->frc.limit(3.0);
        
        // applu the forces
		it->vel *= it->drag; 
		it->vel += it->frc;
        it->pos += it->vel;
        
        it->bRemove = !screen.inside(it->pos);
        
        field.addVectorCircle(it->pos.x, it->pos.y, it->vel.x, it->vel.y, 30, .1);
        
        
    }
    ofRemove(chasers, Particle::shouldRemove);

    // ----------------------
    // wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=wigglers.begin(); it!=wigglers.end(); ++it) {
        it->update();
        it->soundAmp = audioPtr->getVolume(0);
    }
    
    // Tiny Vines
    // ----------------------
    for(vector<TinyVine>::iterator it=tinyVines.begin(); it!=tinyVines.end(); ++it) {
        it->update();
    }
    
    
    // ----------------------
    box2d.update();	    
    field.fadeField(0.98);
}

// ----------------------------------------------------
void DrawnScene::draw() {

    drawBackground();
    ofColor peteWhite = ofColor(255,255,255);
    ofRectangle screen(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    ofSetColor(peteWhite);
    ofFill();
    ofRect(screen);

    
    // ----------------------
    // Vector field
    // ----------------------
    float scalex = (float)field.externalWidth / (float)field.fieldWidth;
    float scaley = (float)field.externalHeight / (float)field.fieldHeight;
    ofSetColor(150);
    for (int i = 0; i < field.fieldWidth; i++){
        for (int j = 0; j < field.fieldHeight; j++){
            
            // pos in array
            int pos = j * field.fieldWidth + i;
            // pos externally
            float px = 	i * scalex;
            float py = 	j * scaley;
            float px2 = px + field.field[pos].x * 5;
            float py2 = py + field.field[pos].y * 5;
            
            ofLine(px,py, px2, py2);
          
            
        }
    }
    
    // ----------------------
    // wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=wigglers.begin(); it!=wigglers.end(); ++it) {
        it->draw();
    }
    
    // ----------------------
    // Tiny Vines
    // ----------------------
    for(vector<TinyVine>::iterator it=tinyVines.begin(); it!=tinyVines.end(); ++it) {
        it->draw();
    }
    
    
    // ----------------------
    // Chasers
    // ----------------------
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        
        it->nConnections = 0;

        if(it->nConnections < 2) {
            for(vector<Chaser>::iterator itB=it; itB!=chasers.end(); ++itB) {
                ofVec2f a = it->pos;
                ofVec2f b = itB->pos;
                if(a.distance(b) < 20) {
                    it->nConnections ++;
                    //ofSetColor(0);
                    ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
                    ofLine(it->pos, itB->pos);
                }
            }
        }
        
        //ofSetColor(0);
        //ofNoFill();
        //it->draw();
        
        float angle = ofRadToDeg( atan2(it->vel.y, it->vel.x) );
        it->rotation += (angle-it->rotation) * 0.3;
        ofEnableAlphaBlending();
        ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
        ofPushMatrix();
        ofTranslate(it->pos);
        ofRotate(it->rotation);
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        eyeA.draw(0, 0, it->radius, it->radius * eyeRatio);
        ofPopStyle();
        ofPopMatrix();
    }

    
    // draw the vines
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
        it->drawAsVine();
    }
    
    // ----------------------
    // Poop
    // ----------------------
    for(vector<VinePoop>::iterator it=poop.begin(); it!=poop.end(); ++it) {
      
        float r = it->getRadius() * 2;
        ofVec2f pos = it->getPosition();
        ofEnableAlphaBlending();
        //previously just black
        ofSetColor(it->color, it->alpha);
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(it->getRotation());
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        it->img->draw(0, 0, r, r);
        ofPopStyle();
        ofPopMatrix();
        
        float age = ofGetElapsedTimef() - it->birthdate;
        if(!screen.inside(pos.x, pos.y)) {
            it->alive  = false;
            it->dead   = true;
        }
        
        if(age > 10 && it->alpha>0){
            it->alpha -= 3.0;
            if(it->alpha <= 0) {
                it->alive  = false;
                it->dead   = true;
            }
        }
    }
    ofRemove(poop, ofxBox2dBaseShape::shouldRemove);
    
    
    // ----------------------
    // sound
    // ----------------------
    float peak = 0.5;
    for(int i=0; i<4; i++) {
        
        float amp = audioPtr->getVolume(i);
        ofVec2f pos;
        // TOP
        if(i == AudioManager::TOP) {
            pos.set(CUBE_SCREEN_WIDTH/2, 20);
        }
        
        // BOTTOM
        if(i == AudioManager::BOTTOM) {
            pos.set(CUBE_SCREEN_WIDTH/2, CUBE_SCREEN_HEIGHT-20);
        }
        
        // LEFT
        if(i == AudioManager::LEFT) {
            pos.set(20, CUBE_SCREEN_HEIGHT/2);
        }
        
        // RIGHT
        if(i == AudioManager::RIGHT) {
            pos.set(CUBE_SCREEN_WIDTH-20, CUBE_SCREEN_HEIGHT/2);
            
        }
        
        
        if(amp > 0.3) {
            int ranTinyVine = ofRandomIndex(tinyVines);
            tinyVines[ranTinyVine].grow();
        }
        
        if(amp>peak) addChasers(pos.x, pos.y);

        if(mustDrawGui){
            // draw
            ofSetColor(255);
            ofCircle(pos, 25);
            ofSetColor(255, 0, 255);
            ofCircle(pos, 5+(20*amp));
            ofSetColor(0, 250, 250);
            ofDrawBitmapString(AudioManager::getSideName(i), pos);
        }
    }

    
    // ----------------------
    // add some randomly
    // ----------------------

    if((int)ofRandom(0, 40)==10) addChasers();
    if((int)ofRandom(0, 4)==2)   makePoop();
}

// ----------------------------------------------------
void DrawnScene::drawGui() {

    if(!mustDrawGui){
		return;
	}
	
	gui.draw();
#ifdef USE_SWIRPS
	ofSetColor(BaseScene::twitterColour);
	particles.draw();
	glColor3f(0,0,0);
	ofDrawBitmapString("Click to create SWIRP\n1 - repel\n2 - atract",10,ofGetHeight()-35);
#endif        
}

// ----------------------------------------------------
void DrawnScene::keyPressed(int key) {
    
    
    if(key == 'q') {
        
        int ranTinyVine = ofRandomIndex(tinyVines);
        tinyVines[ranTinyVine].grow();
        
    }
    
    switch (key) { 
        case 'g': {
            mustDrawGui = !mustDrawGui;
            break;
        }
        case 'c': {
        addChasers();
            break;
        }    
            case ' ': {
        int ranVine = ofRandomIndex(vines);
        vines[ranVine].add(box2d);
                break;
        }
        case 's':{
    // *****************************
    // *** working on this still ***
    // *****************************
        int ranVine  = ofRandomIndex(vines);
        int ranJoint = ofRandomIndex(vines[ranVine].joints);
        
        //vines[ranVine].joints[ranJoint].destroy();
        //vines[ranVine].joints.erase( vines[ranVine].joints.begin() + ranJoint );// + ranJoint);
            break;
        }
        case 'p' : {
        makePoop();
            break;
        }    
        #ifdef USE_SWIRPS
        case '1': {
            repel_effect = 0.3;
            follow_effect = 0.001;
            break;
        }
        case '2': {
            repel_effect = 0.24;
            follow_effect = 0.002;
            break;
        }
        #endif		
        default: {
            break;
        } 
    }
    
}

// ----------------------------------------------------
void DrawnScene::mousePressed(int x, int y, int button){
#ifdef USE_SWIRPS
    rxSwirp* sw = new rxSwirp(ofVec3f(x,y,0),particles, (int)ofRandom(5,15));
    particles.addSwirp(sw);
#endif   
}

// ----------------------------------------------------
void DrawnScene::mouseMoved(int x, int y ){
#ifdef USE_SWIRPS
    ofVec3f m(x, y, 0);
    particles.follow(m,follow_effect);
    particles.repel(repel_effect);
    particles.update();
#endif	
}

// ----------------------------------------------------
void DrawnScene::respondToNewComplimentaryColours(){
    for(int i=0; i<vines.size(); i++) {
        vines[i].colorDes = complimentaryColours[ofRandomIndex(complimentaryColours)];  
    }
    for(int i=0; i<poop.size(); i++) {
        poop[i].colorD = complimentaryColours[ofRandomIndex(complimentaryColours)];  
    }
    for(int i=0; i<tinyVines.size(); i++) {
        tinyVines[i].colorD = complimentaryColours[ofRandomIndex(complimentaryColours)]; 
    }
}

// ----------------------------------------------------
// @todd you can handle commands here events to change the scene....
void DrawnScene::handleCommands(TwitterCommand& cmd, Effects& fx) {

	// handle commands.
	set<string>::const_iterator it = cmd.tokens.begin();
	while(it != cmd.tokens.end()) {
		const string& c = (*it);
		if(c == "bigger") {
		
		}
		else if(c == "smaller") {
		
		}
		else if(c == "faster") {
		
		}
		else if(c == "slower") {
		
		}
		else if(c == "less") {
		
		}
		else if(c == "more") {
		
		}
		fx.applyEffect(c);
		++it;
	}
	
	// handle colour
	map<string, ofColor>::const_iterator cit = cmd.colours.begin();
	while(cit != cmd.colours.end()) {	
		BaseScene::twitterColour = cit->second;
		bgColor = BaseScene::twitterColour;
        generateComplimentaryColours();
        respondToNewComplimentaryColours();
		break;
	}
}