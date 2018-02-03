/*
 *  Particle.cpp
 *  Graphics Assignment Four
 *
 *  Created by Karl Grogan on 26/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
Particle::Particle(){

}

Particle::Particle(float x, float y, float z){
	xpos=x;
	ypos=y;
	zpos=z;
	r=1.0;
	g=1.0;
	b=1.0;
}

// Constructor for a particle.
void Particle::CreateParticle(float x, float y, float z)
{
	lifetime= 10;
	decay=1;
	r = 177.0/256.0;
	g = 0.7;
	b = 1.0;
	xpos=x;
	ypos=y;
	zpos=z;
	xspeed = 2-(int)rand() % 5 ;
	yspeed = 2-(int)rand() % 5  ;
	zspeed = 2- (int)rand() % 5  ;
	active = true;
}
	
/* 
 Evolves the particle parameters over time.
 This method changes the vertical and horizontal poition of the particle, its, speed and decay time.
*/
void Particle::EvolveParticle()
{
	lifetime-=decay;
	xpos += 2- (int)rand()  % 5 ;
	ypos -= 2-(int)rand()  % 5 ;
	zpos -= 2-(int)rand()  % 5 ;
	yspeed+= 2- (int)rand()  % 5 ;
}

/*
 This method draws the particle on screen.
 Sets the clorour mode.
 Sets the drawing primitive.
 Sets the coordinates of each point on the triangle strip
*/
void Particle::DrawObjects(bool needTurn)
{
    glPushMatrix();
    if(needTurn)
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	if ((lifetime>0.0)) {
		glBegin(GL_TRIANGLE_STRIP);

		glColor3f(r,g,b);
		glTexCoord2f(0.0,1.0); glVertex3f(xpos+0.4, ypos+0.4,zpos);     // top    right
		glTexCoord2f(0.0,1.0); glVertex3f(xpos-0.4, ypos+0.4,zpos);     // top    left
		glTexCoord2f(0.0,1.0); glVertex3f(xpos+0.4, ypos-0.4,zpos);     // bottom right
		glTexCoord2f(0.0,1.0); glVertex3f(xpos-0.4, ypos-0.4,zpos);     // bottom left
		
		glEnd();
	}
    glPopMatrix();
}

// Getter and setter methods


float Particle::GetXPos()
{
	return xpos;
}

void Particle::SetXPos(float xPos)
{
	xpos = xPos;
}


void Particle::SetYPos(float yPos)
{
	xpos = yPos;
}

float Particle::GetYPos()
{
	return ypos;
}

void Particle::SetG(float myG)
{
	g = myG;
}

float Particle::GetG()
{
	return g;
}
