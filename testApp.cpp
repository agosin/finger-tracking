#include "testApp.h"
#include <vector.h>


void testApp::setup(){
    
    ofBackground(255, 255, 255);
    ofSetFrameRate(60);
    rgbaFbo.allocate(W, H, GL_RGBA);
    rgbaFbo.begin();
        ofClear(255, 255, 255, 0);
    rgbaFbo.end();

  vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(1);
	vidGrabber.initGrabber(W,H);

	colorImg.allocate(W, H);	
    colorImgHSV.allocate(W, H);			
    hueImg.allocate(W, H);				
    satImg.allocate(W, H);
    valImg.allocate(W, H);					
    
    colorTrackedPixels = new unsigned char [W*H];	
    trackedTexture.allocate(W, H, GL_LUMINANCE);
    
	// HSV variables. by default track red color.
	hue = 0.64;
	hueWidth = 0.18;
//	sat = 0.6;
//	satWidth = 0.94;
//  val = 0.7;
//	valWidth = 0.62;
    sat = 0.6;
    satWidth = 0.94;
    val = 0.82;
    valWidth = 0.62;
    
    for(int i=0; i<K; i++){
        pvx.push_back(400);
        pvy.push_back(400);
    }
    
//    cameraRotation.set(0);
	zoom = -500;
	zoomTarget = 200;
	
	billboards.getVertices().resize(NUM_BILLBOARDS);
	billboards.getColors().resize(NUM_BILLBOARDS);
	billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));


	// ------------------------- billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboards.getVertices()[i].set(ofRandom(-500, 500),
                                      ofRandom(-500, 500),
                                      ofRandom(-500, 500));
		
		billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(200, 500),255, 255));
	    billboardSizeTarget[i] = ofRandom(4, 64);
		
	}
	

    billboardsFinal.getVertices().resize(NUM_PAOPAO);
    billboardsFinal.getColors().resize(NUM_PAOPAO);
    billboardsFinal.getNormals().resize(NUM_PAOPAO,ofVec3f(0));
        
    // ------------------------- billboard particles
    for (int i=0; i<NUM_PAOPAO; i++) {
            
        billboardVelsFinal[i].set(ofRandomf(), -1.0, ofRandomf());
        billboardsFinal.getVertices()[i].set(ofRandom(-500, 500),
                                            ofRandom(-500, 500),
                                            ofRandom(-500, 500));
        
        billboardsFinal.getColors()[i].set(ofColor::fromHsb(ofRandom(0, 50),255, 255));
        billboardSizeTargetFinal[i] = ofRandom(4, 64);
            
    }
	
	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
    
    billboardsFinal.setUsage(GL_DYNAMIC_DRAW);
    billboardsFinal.setMode(OF_PRIMITIVE_POINTS);
	//billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader
	// this is used to change the
	// size of the particle
	billboardShader.load("Billboard");
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("dot.png");
	ofEnableAlphaBlending();
    

}




int testApp::center(vector<int> x){
    int sum = 0;
    if (x.size()==0)
        return 0;
    for (int i=0; i<x.size(); i++)
        sum += x[i];
    return int(sum/x.size());
}

int testApp::minindex(vector<float> x){
    int index = 0;
    float min = x[0];
    for(int j=1; j<x.size(); j++){
        if(x[j]<min){
            index = j;
            min = x[j];
        }
    }
    return index;
}

int testApp::exist(vector<int> x, vector<int> y, int vx, int vy){
    for(int j=0; j<x.size(); j++)
    {
        if (x[j]==vx && y[j]==vy){
            return 1;
            break;
        }
    }
    return 0;
}

void testApp::filter(vector<int> x, vector<int> y){
    int vx,vy;
    int sum;
    for(int i=0; i<x.size(); i++){
        vx = x[i];
        vy = y[i];
        sum = 0;
        if(vx>1 && vx<W-2 && vy>1 && vy<H-2){
            sum += exist(x, y, vx-2, vy-2);
            sum += exist(x, y, vx-2, vy-1);
            sum += exist(x, y, vx-2, vy);
            sum += exist(x, y, vx-2, vy+1);
            sum += exist(x, y, vx-2, vy+2);
            sum += exist(x, y, vx-1, vy-2);
            sum += exist(x, y, vx-1, vy-1);
            sum += exist(x, y, vx-1, vy);
            sum += exist(x, y, vx-1, vy+1);
            sum += exist(x, y, vx-1, vy+2);
            sum += exist(x, y, vx, vy-2);
            sum += exist(x, y, vx, vy-1);
            sum += exist(x, y, vx, vy);
            sum += exist(x, y, vx, vy+1);
            sum += exist(x, y, vx, vy+2);
            sum += exist(x, y, vx+1, vy-2);
            sum += exist(x, y, vx+1, vy-1);
            sum += exist(x, y, vx+1, vy);
            sum += exist(x, y, vx+1, vy+1);
            sum += exist(x, y, vx+1, vy+2);
            sum += exist(x, y, vx+2, vy-2);
            sum += exist(x, y, vx+2, vy-1);
            sum += exist(x, y, vx+2, vy);
            sum += exist(x, y, vx+2, vy+1);
            sum += exist(x, y, vx+2, vy+2);
        }
        if(sum<20){
            x.erase(x.begin()+i);
            y.erase(y.begin()+i);
            i--;
        }
    }
    
}


void testApp::cluster(vector<int> x, vector<int> y){
    vector<float> dist;
    for(int i=0; i<K; i++){
        uxx.push_back(400);
        uyy.push_back(400);
    }
    
    vector<int> c;
    c.clear();
    for(int i=0; i<x.size(); i++){
        c.push_back(i%K);
    }
   
    int kk=0;
    while(1){
        kk++;
        
        ux.clear();
        uy.clear();
        for(int j=0; j<K; j++){
            int count = 0;
            int tempux = 0;
            int tempuy = 0;
            for (int i=0; i<x.size(); i++){
                if(c[i]==j){
                    count++;
                    tempux += x[i];
                    tempuy += y[i];
                }
            }
            if(count>0){
                ux.push_back(int(tempux*1.0/count));
                uy.push_back(int(tempuy*1.0/count));
            }
            else {
                ux.push_back(uxx[j]);
                uy.push_back(uyy[j]);
            }
        }
        
        uxx.clear();
        uyy.clear();
        
        for (int j=0; j<K; j++){
            uxx.push_back(ux[j]);
            uyy.push_back(uy[j]);
        }
    
        c.clear();
        for(int i=0; i<x.size(); i++){
            dist.clear();
            for(int j=0; j<K; j++)
                dist.push_back(((x[i]-ux[j])*1.0/1000*(x[i]-ux[j])*1.0/1000+(y[i]-uy[j])*1.0/1000*(y[i]-uy[j])*1.0/1000));
            c.push_back(minindex(dist));
        }
        if(kk>10)
            break;
     }
}

int testApp::highb(vector<int> x)
{
    if(x.size()==0)
        return 0;
    float max = x[0];
    for (int i=1; i<x.size(); i++) {
        if(x[i]>max)
            max = x[i];
    }
    return max;
}


int testApp::lowb(vector<int> x)
{
    if (x.size()==0)
        return 0;
    int min = x[0];
    for (int i=1; i<x.size(); i++) {
        if(x[i]<min)
            min = x[i];
    }
    return min;
}

float testApp::dist(vector<int> x, vector<int> y)
{
    float d=0;
    for(int i=1; i<x.size(); i++)
        d += abs(x[i]-x[0]);
    for(int i=1; i<y.size(); i++)
        d += abs(y[i]-y[0]);
    return sqrt(d);
}

void testApp::update(){
    
    ofBackground(100,100,100);
    
    if(!paopao){
    
	vidGrabber.update();
	if (vidGrabber.isFrameNew()){
        
		minHue = max((hue - hueWidth*0.5) * 255, 0.0);
		maxHue = min((hue + hueWidth*0.5) * 255, 255.0);
		minSat = max((sat - satWidth*0.5) * 255, 0.0);
		maxSat = min((sat + satWidth*0.5) * 255, 255.0);
		minVal = max((val - valWidth*0.5) * 255, 0.0);
		maxVal = min((val + valWidth*0.5) * 255, 255.0);
        
        colorImg.setFromPixels(vidGrabber.getPixels(), W, H);
		
		colorImgHSV = colorImg;
		colorImgHSV.convertRgbToHsv();
        colorImgHSV.mirror(FALSE, TRUE);
		colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, valImg);
        
		unsigned char * huePixels = hueImg.getPixels();
		unsigned char * satPixels = satImg.getPixels();
		unsigned char * valPixels = valImg.getPixels();
		int nPixels = W * H;
        
        px.clear();
        py.clear();
		for (int i = 0; i < nPixels; i++){
			if ((huePixels[i] >= minHue && huePixels[i] <= maxHue) &&
				(satPixels[i] >= minSat && satPixels[i] <= maxSat) &&
				(valPixels[i] >= minVal && valPixels[i] <= maxVal)){
				colorTrackedPixels[i] = 255;
                px.push_back(i%W);
                py.push_back(i/W);
			} else {
				colorTrackedPixels[i] = 0;
			}
		}
        
        trackedTexture.loadData(colorTrackedPixels, W, H, GL_LUMINANCE);
        
        if (px.size()>=K){
            filter(px,py);
            cluster(px, py);

            cx.clear();
            cy.clear();
            for(int i=0; i<K; i++){
                xx1 = ux[i];
                yy1 = uy[i];
                xx0 = pvx[i];
                yy0 = pvy[i];
                if (abs(xx1-xx0)<5 && abs(yy1-yy0)<5){
                    ppx = xx1;
                    ppy = yy1;
                }
                else{
                    ppx = xx0;
                    ppy = yy0;
                }
                cx.push_back(ppx);
                cy.push_back(ppy);
                pvx.push_back(xx1);
                pvy.push_back(yy1);
            }
            pvx.erase(pvx.begin(), pvx.begin()+K);
            pvy.erase(pvy.begin(), pvy.begin()+K);

            if(cy[0]<cy[1]){
                xx1 = cx[0];
                yy1 = cy[0];
            }
            else{
                xx1 = cx[1];
                yy1 = cy[1];
            }
            
            if(xx1<200 && yy1<200)
                flag1 = 1;
            if(xx1>824 && yy1<200)
                flag2 = 1;
            
            cxx = center(cx);
            cyy = center(cy);
            
            osc::SendPosition(0, xx1, yy1, cxx, cyy);
            

        }
        
    }
    ofEnableAlphaBlending();
    
    rgbaFbo.begin();
        drawFbo();
    rgbaFbo.end();
    
    for (int i=0; i<points.size(); i++) {
        speeds[i].y += 0.04; // some grav
        points[i]   += speeds[i];
        speeds[i]   *= 0.98;
        
        ofVec2f posVec = ofVec2f(cxx, ofGetHeight()) - points[i];
        if(posVec.length() < 200) {
            posVec.normalize();
            speeds[i] += posVec;
        }
        
    }
    
    for (int i=0; i<300; i++)
        addPoint(ofRandom(10, ofGetWidth()-10), ofGetHeight());
        
    
        
      float t = (ofGetElapsedTimef());
      float div = 250.0;
      
      for (int i=0; i<NUM_BILLBOARDS; i++) {
          
          // noise
          ofVec3f vec(ofSignedNoise(t, billboards.getVertex(i).y/div, billboards.getVertex(i).z/div),
                      ofSignedNoise(billboards.getVertex(i).x/div, t, billboards.getVertex(i).z/div),
                      ofSignedNoise(billboards.getVertex(i).x/div, billboards.getVertex(i).y/div, t));
          
          vec *= 20 * ofGetLastFrameTime();
          billboardVels[i] += vec;
          billboards.getVertices()[i] += billboardVels[i];
          billboardVels[i] *= 0.9f;
          billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i] * ofNoise(t+i),0,0));
      }
      zoom += (zoomTarget - zoom) * 0.1;
      
  }
    
    
    
    
    if (paopao){
    
        float t = (ofGetElapsedTimef()) * 0.9f;
        float div = 250.0;
        
        for (int i=0; i<NUM_PAOPAO; i++) {
            
            // noise
            ofVec3f vec(ofSignedNoise(t, billboardsFinal.getVertex(i).y/div, billboardsFinal.getVertex(i).z/div),
                        ofSignedNoise(billboardsFinal.getVertex(i).x/div, t, billboardsFinal.getVertex(i).z/div),
                        ofSignedNoise(billboardsFinal.getVertex(i).x/div, billboardsFinal.getVertex(i).y/div, t));
            
            vec *= 20 * ofGetLastFrameTime();
            billboardVelsFinal[i] += 2*vec;
            billboardsFinal.getVertices()[i] += billboardVelsFinal[i];
            billboardVelsFinal[i] *= 0.94f;
            billboardsFinal.setNormal(i,ofVec3f(12 + billboardSizeTargetFinal[i] * ofNoise(t+i),0,0));
        }
        
        zoom += (zoomTarget - zoom) * 0.03;

        
    }

}

void testApp::drawFbo(){
    
	ofFill();
	ofSetColor(255,255,255, alpha);
	ofRect(0,0,W,H);
	
//	ofNoFill();
//	ofSetColor(255,255,255);

	ofFill();
    ofSetColor(255, 255, 200);
    if(cx.size()==K){
        int size = 20;
        int ppx = (cx[1]-cx[0])/size;
        int ppy = (cy[1]-cy[0])/size;
        for(int i=0; i<size; i++)
            ofCircle(cx[0]+ppx*i, cy[0]+ppy*i, 15);
    }
//    if(cx.size()==K)
//        ofCircle(cx[0], cy[0], 15);
}


void testApp::draw(){
    
    if (!paopao){
    
	ofSetColor(0xffffff);
//	colorImg.draw(20, 25);
//	colorImgHSV.draw(20, H+50);
//	trackedTexture.draw(0, 0);
 
    
    ofBackgroundGradient(ofColor(backr, backg, backb), ofColor(0, 0, 0));
       
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
    //    ofRotate(cameraRotation.x, 1, 0, 0);
    //    ofRotate(cameraRotation.y, 0, 1, 0);
    //    ofRotate(cameraRotation.y, 0, 0, 1);
        
        // bind the shader so that wee can change the
        // size of the points via the vert shader
    billboardShader.begin();
        
    ofEnablePointSprites();
    texture.getTextureReference().bind();
    billboards.draw();
    texture.getTextureReference().unbind();
    ofDisablePointSprites();
        
    billboardShader.end();
        
    ofPopMatrix();
    
    if (flag1){
        ofSetColor(100,200,200);
        ofCircle(50, 50, 30);
    }
    else {
        ofSetColor(255,110,170);
        ofCircle(50, 50, 30);
    }
    
    if (flag2){
        ofSetColor(100,200,200);
        ofCircle(900, 50, 30);
        flag2 = 0;
        flag1 = 0;
    }
    else {
        ofSetColor(255,110,170);
        ofCircle(900, 50, 30);
    }
    
    rgbaFbo.draw(0, 0);
    
    ofSetColor(100, 110, 170);
    glBegin(GL_POINTS);
    for (int i=0; i<points.size(); i++) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
        
    
    }
    
    timer++;
    if(timer == 2){
        if(step1){
            backr--;
            if(backr==0){
                step1 = 0;
                step2 = 1;
            }
        }
        
        if(step2){
            backg++;
            if(backg == 220){
                step2 = 0;
                step3 = 1;
            }
        }
        
        if(step3){
            backb--;
            if(backb == 0){
                step3 = 0;
                step4 = 1;
            }
        }
        
        if(step4){
            backr++;
            if(backr == 220){
                step4 = 0;
                step5 = 1;
            }
        }
        
        if(step5){
            backg--;
            if (backg == 0)
                step5 = 0;
        }
        timer = 0;
    }

        
  if (paopao){
  
      ofBackgroundGradient(ofColor(220, 0, 0), ofColor(0, 0, 0));

      ofSetColor(255);
      
      ofPushMatrix();
      ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
//    ofRotate(cameraRotation.x, 1, 0, 0);
//    ofRotate(cameraRotation.y, 0, 1, 0);
//    ofRotate(cameraRotation.y, 0, 0, 1);
      
      // bind the shader so that wee can change the
      // size of the points via the vert shader
      billboardShader.begin();
      
      ofEnablePointSprites();
      texture.getTextureReference().bind();
      billboardsFinal.draw();
      texture.getTextureReference().unbind();
      ofDisablePointSprites();
      
      billboardShader.end();
      
      ofPopMatrix();
      
  }

}



void testApp::keyPressed(int key){

	switch(key){
		case 'a':
			hue = min(hue + 0.02, 1.0);
			break;
		case 'z':
			hue = max(hue - 0.02, 0.0);
			break;
		case 's':
			hueWidth = min(hueWidth + 0.02, 1.0);
			break;
		case 'x':
			hueWidth = max(hueWidth - 0.02, 0.0);
			break;
		case 'd':
			sat = min(sat + 0.02, 1.0);
			break;
		case 'c':
			sat = max(sat - 0.02, 0.0);
			break;
		case 'f':
			satWidth = min(satWidth + 0.02, 1.0);
			break;
		case 'v':
			satWidth = max(satWidth - 0.02, 0.0);
			break;
		case 'g':
			val = min(val + 0.02, 1.0);
			break;
		case 'b':
			val = max(val - 0.02, 0.0);
			break;
		case 'h':
			valWidth = min(valWidth + 0.02, 1.0);
			break;
		case 'n':
			valWidth = max(valWidth - 0.02, 0.0);
			break;
        case 'p':
            paopao = 1;
            break;
        case 'q':
            ofToggleFullscreen();
            break;
        
	}
 //   cout<<hue<<" , "<<hueWidth<<" , "<<sat<<" , "<<satWidth<<" , "<<val<<" , "<<valWidth<<endl;
}


