/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Tutorial:		Audio / Filter / Comb
	Description:	
*/

#include "tutorial.h"

int feedType=0;				// Feed type
double sweepFreq = 1./8;
Accum<> tmr(sweepFreq, 2);	// Switch between flanging types
//LFO<> src(110);			// A rich source
NoiseWhite<> src;
LFO<> mod(sweepFreq);
Comb<float, ipl::Any> comb(1./100, 1,0);

void audioCB(AudioIOData& io){

	for(uint32_t i=0; i<io.framesPerBuffer(); i++){
		
		if(tmr()){
			switch(feedType){
			case 0:	printf("Low-pass feedforward\n");
				comb.feeds( 1,0); break;
			case 1:	printf("High-pass feedforward\n");
				comb.feeds(-1,0); break;
			case 2:	printf("Low-pass feedback\n");
				comb.feeds(0,0.7); break;
			case 3:	printf("High-pass feedback\n");
				comb.feeds(0,-0.7); break;
			case 4:	printf("Low-pass dual-feed\n");
				comb.feeds(0.7,0.7); break;
			case 5:	printf("High-pass dual-feed\n");
				comb.feeds(-0.7,-0.7); break;
			case 6:	printf("All-pass 1\n");
				comb.feeds(0.9,-0.9); break;
			case 7:	printf("All-pass 2\n");
				comb.feeds(-0.9,0.9); break;
			}
			(++feedType) %= 8;
		}
		
		float s = src();
		
		comb.ipolType(ipl::ROUND);
		comb.delay(mod.triU() * 1./400);
		s = comb(s)*0.1;

		io.out(0)[i] = io.out(1)[i] = s;
	}
}

RUN(audioCB);
