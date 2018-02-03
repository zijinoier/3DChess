/*
 *  Particle.h
 *  Graphics Assignment Four
 *
 *  Created by Karl Grogan on 26/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

class Particle
{
private:
	float lifetime=10;                       // Lifetime of the particle
	float decay=1;                          // decay speed of the particle
	float r=1.0;
	float g=0.0;
	float b=0.0;                       // color values of the particle
	float xpos,ypos,zpos;                 // position of the particle
	float xspeed,yspeed,zspeed;           // speed of the particle
	bool active;						  // is particle active or not?
	
public:
	Particle();
	Particle(float x, float y, float z);
	void CreateParticle(float x, float y, float z);
	void EvolveParticle();
	void DrawObjects(bool needTurn);
	float GetXPos();
	void SetXPos(float xPos);
	void SetYPos(float yPos);
	float GetYPos();
	float GetG();
	void SetG(float myG);

};
