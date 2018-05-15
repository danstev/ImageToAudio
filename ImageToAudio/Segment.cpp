#include "stdafx.h"
#include "Segment.h"
#include<iostream>
#include<string>
using namespace std;

Segment::Segment()
{
}


Segment::~Segment()
{
}

/*
Easily constructs the required audio files for algorithms 1 2 and 4, then resets path for next algorithm.
*/
void Segment::SegmentToAudio()
{
		Algorithm1();
		path = "";
		Algorithm2();
		path = "";
		Algorithm4();
}

/*
ProcessRawSeg processes the raw information in to more complex data.
*/
void Segment::ProcessRawSeg()
{
	xPos = x / count;
	yPos = y / count;
	colR = r / count;
	colG = g / count;
	colB = b / count;
	luminance = (0.2126*colR + 0.7152*colG + 0.0722*colB);
	area = count / (maxHeight * maxWidth);
}

/*
PrintSeg for testing purposes.
*/
void Segment::PrintSeg()
{
	cout << "X count: " << x
		 << " Y count: " << y
		 << " Count: " << count
		 << " xPos: " << yPos
		 << " yPos: " << xPos
		 << " RED: " << colR
		 << " R count: " << r
		 << " Green: " << colG
		 << " G count: " << g
		 << " Blue: " << colB
		 << " B count: " << b
		 << " Luminance: " << luminance
		<< endl;
}

/*
Pan, pans the file according the the X position.
*/
void Segment::Pan()
{
	//Echos what is happening
	string echo = "echo Panning segment.";
	system(echo.c_str());

	//Pan takes in a float (from -1 to 1) which dictates where in the spectrum the sound will be placed.
	float amount = (2 * (xPos / maxWidth)) - 1;
	string pan = "modify space 1 " + path + (" panned" + path) + " " + to_string(amount);
	system(pan.c_str());

	//cleaup
	string del = "del " + path;
	system(del.c_str());

	//New path
	path = "panned" + path;
}

/*
Envelope applys a filtered envelope going down, improved version of filter functions.
*/

void Segment::Envelope()
{
	//Echos what is happening
	string echo = "echo Enveloping segment.";
	system(echo.c_str());

	//Envelope starting at a pitch and going down, improved version of the lowpass and highpass functions.
	int pitch = (600 * (yPos / maxHeight)) + 100;
	string envelope = "distort envel 2 " + path + (" envel" + path) + (" " + to_string(pitch));
	system(envelope.c_str());

	//cleanup
	string del = "del " + path;
	system(del.c_str());
	//new path
	path = "envel" + path;
}

/*
Gain takes the area of a segment and uses it as a gain reduction.
*/
void Segment::Gain()
{
	//Echos what is happening
	string echo = "echo adding gain to segment.";
	system(echo.c_str());

	//Gain change applied here.
	string vol = "modify loudness 4 " + path + (" vol" + path) + " -l" + to_string(area);
	system(vol.c_str());

	//cleanup
	string del = "del " + path;
	system(del.c_str());

	//new path
	path = "vol" + path;
}

//Haas effect applied via 0.01 second delay to one channel.
void Segment::Haas()
{
	//delay to add to add to one channel
	string empty = "synth silence silence.wav 44100 2 0.01";
	system(empty.c_str());

	//add the delay to the channel.
	string first = "nmix silence.wav " + (path + "0.wav") + " image" +  + ".wav";
	system(first.c_str());
}

//Normalise gain
void Segment::NormaliseGain()
{
	string norm = "loudness 3 " + path;
	system(norm.c_str());
	path = "norm" + std::to_string(segNumber) + ".wav";
}

//Creates a basewave intended for bandpassing. 80 hz is a low estimate where sub-bass ends.
void Segment::BaseWave()
{
	//Echos what is happening
	string echo = "echo Creating basewave for bandpassing.";
	system(echo.c_str());

	//Create a sawtooth wave at 80hz
	string audio = "synth wave 4 baseWave" + std::to_string(segNumber) + ".wav 44100 1 0.1 80";
	system(audio.c_str());
	path = "baseWave" + std::to_string(segNumber) + ".wav";

}

//Creates a basewave intended for bandpassing. Freq is passed down to this function and this changes the freq of the sawtooth generated.
void Segment::BaseWave(float freq)
{
	//Echos what is happening
	string echo = "echo Creating basewave for bandpassing.";
	system(echo.c_str());
	string audio = "synth wave 4 baseWave" + std::to_string(segNumber) + ".wav 44100 1 0.25 " + std::to_string(freq);
	system(audio.c_str());
	path = "baseWave" + std::to_string(segNumber) + ".wav";
}

//Takes channel here as well, for playing with binaural effects.
void Segment::BaseWave(float freq, int chans)
{
	//Echos what is happening
	string echo = "echo Creating basewave for bandpassing.";
	system(echo.c_str());
	string audio = "synth wave 4 baseWave" + std::to_string(segNumber) + ".wav 44100 "+ std::to_string(chans) +" 0.25 " + std::to_string(freq);
	system(audio.c_str());
	path = "baseWave" + std::to_string(segNumber) + ".wav";
}

//Creates a sawtooth wave at a pitch dictated by segment height.
void Segment::BaseWavePitched()
{
	//Echos what is happening
	string echo = "echo Creating a pitched base wave.";
	system(echo.c_str());

	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 1 1 ";
	audio = audio + wav;

	//pitch = atleast 100 + how far up the pixel is.
	int pitch = (600 * (yPos / maxHeight)) + 100;
	audio = audio + to_string(pitch);
	system(audio.c_str());

}

//Creates a sawtooth wave, takes in freq manually this time.
void Segment::BaseWavePitched(float freq)
{
	//Echos what is happening
	string echo = "echo Creating a pitched base wave.";
	system(echo.c_str());

	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 1 1 ";
	audio = audio + wav;

	//pitch
	audio = audio + to_string(freq);
	system(audio.c_str());
}

//Creates a sawtooth wave, takes in freq and channels manually this time.
void Segment::BaseWavePitched(float freq, int chans)
{
	//Echos what is happening
	string echo = "echo Creating a pitched base wave.";
	system(echo.c_str());

	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 1 "+ std::to_string(chans)+" ";
	audio = audio + wav;

	//pitch
	audio = audio + to_string(freq);
	system(audio.c_str());
}

//Creates a short base wave for algorithm 1.
void Segment::BaseWaveShort()
{
	//Echos what is happening
	string echo = "echo Creating a short pitched base wave.";
	system(echo.c_str());
	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 1 0.125 ";
	audio = audio + wav;

	//pitch
	int pitch = (600 * (yPos / maxHeight)) + 100;
	audio = audio + to_string(pitch);
	system(audio.c_str());

}

//Creates a short base wave for algorithm 1. Takes in a freq.
void Segment::BaseWaveShort(float freq)
{
	//Echos what is happening
	string echo = "echo Creating a short pitched base wave.";
	system(echo.c_str());
	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 1 0.125 ";
	audio = audio + wav;

	//pitch
	audio = audio + to_string(freq);
	system(audio.c_str());
}

//Creates a short base wave for algorithm 1. Takes in a freq and channel amount.
void Segment::BaseWaveShort(float freq, int chans)
{
	//Echos what is happening
	string echo = "echo Creating a short pitched base wave.";
	system(echo.c_str());
	//Soundwave if 1/2
	string audio = "synth wave 4 ";
	string n = std::to_string(segNumber);
	audio = audio + n;
	path = n + ".wav";
	string wav = ".wav 44100 "+ std::to_string(chans) +" 0.125 ";
	audio = audio + wav;

	//pitch
	audio = audio + to_string(freq);
	system(audio.c_str());
}

//Adds a uniform 6 decibel gain.
void Segment::Louder()
{
	//Echos what is happening
	string echo = "echo Adding generic gain to segment.";
	system(echo.c_str());

	string loud = "modify loudness 2 " + path + (" vol" + path) + " 6";
	system(loud.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "louder" + path;
}

//applys a chamber reverb to the segment.
void Segment::Reverb()
{
	//Echos what is happening
	string echo = "echo Reverbing segment.";
	system(echo.c_str());

	//Reverb added here
	string reverb = "modify revecho 3 " + path + (" vol" + path);
	system(reverb.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "reverbed" + path;
}

//Bandpass, freq determined via height. Helps with harmonic content creation.
void Segment::BandPass()
{
	string echo = "echo Bandpassing segment.";
	system(echo.c_str());

	float freq = (600 * (yPos / maxHeight)) + 100;
	string bandpass = "filter variable 2 "+ path + (" bp"  + path) + " 0.01 " + " 10 " + to_string(freq);
	system(bandpass.c_str());
	
	string del = "del " + path;
	system(del.c_str());
	path = "bp" + path;
}

//More controlled bandpass.
void Segment::BandPass(float freq, float acuity)
{
	//Echos what is happening
	string echo = "echo Bandpassing segment.";
	system(echo.c_str());

	string bandpass = "filter variable 2 " + path + (" bp" + path) + to_string(acuity) + " 10 " + to_string(freq);
	system(bandpass.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "bp" + path;
}

//Reverse decay done via an envelope.
void Segment::ReversedDecay()
{
	int pitch = (600 * (yPos / maxHeight)) + 100;
	string envelope = "distort envel 2 " + path + (" envel" + path) + (" " + to_string(pitch));
	system(envelope.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "envel" + path;
}

//Lowpass, reduces frequencies above a certain point.
void Segment::Lowpass()
{
	//Echos what is happening
	string echo = "echo Lowpass segment.";
	system(echo.c_str());

	string lp = "filter variable 3 " + path + (" lp" + path) + " 0.01 " + " 10 20";
	system(lp.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "lp + path";
}

//Lowpass, reduces frequencies above a certain point you decide, steepness is not used.
void Segment::Lowpass(float freq, float steepness)
{
	//Echos what is happening
	string echo = "echo Lowpass segment.";
	system(echo.c_str());

	string lp = "filter variable 3 " + path + (" lp" + path) + " 0.01 " + " 10 "+ to_string(freq);
	system(lp.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "lp" + path;
}

//Highpass, reduces frequencies below a certain point.
void Segment::Highpass()
{
	//Echos what is happening
	string echo = "echo Highpass segment.";
	system(echo.c_str());

	string lp = "filter variable 4 " + path + (" hp" + path) + " 0.01 " + " 10  20000";
	system(lp.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "hp" + path;
}

//Highpass, reduces frequencies below a certain point, steepness is not used.
void Segment::Highpass(float freq, float steepness)
{
	//Echos what is happening
	string echo = "echo Lowpass segment.";
	system(echo.c_str());

	string lp = "filter variable 4 " + path + (" hp" + path) + " 0.01 " + " 10 " + to_string(freq);
	system(lp.c_str());

	string del = "del " + path;
	system(del.c_str());
	path = "hp" + path;
}

//Creates wave, bandpasses, adds gain, filters to remove harsh frequencies, luiminance is used in basewave and bandpass.
void Segment::Luminance()
{
	BaseWavePitched();
	BandPass();
	Gain();
	Highpass(8000,2);
}

/*
Algorithm 1:
	Short wave meant for concantenation.
	Harmonics from luminosity
	Panned
	volume increased
*/
void Segment::Algorithm1()
{
	//gen small wave
	BaseWaveShort();
	Pan();
}

/*
Algorithm 2:
	Concurrent waves, low passed enveloped
Harmonics from luminosity
Panned
volume increased
*/
void Segment::Algorithm2()
{
	//wave gen
	BaseWavePitched();
	Envelope();
	Pan();
	Gain();

}

/*
Algorithm 3:
	
Harmonics from pixels, this algorithm is performed all in the
ImageToSegment.cpp, due to it requiring a look at all the pixels.
Panned
volume increased

As its not in this class, this is a placeholder function.
*/
void Segment::Algorithm3()
{

}

void Segment::Algorithm4()
{
	//generate wave
	BaseWave();
	BandPass();
	Pan();
}


//Past this point, the algorithms are not in use and are relics from past experimentation.

/*
Unused algorithm - 1

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Spectral
Dynamics - Reversed Decay
*/

void Segment::UnusedAlgorithm1()
{
	//Pitch
	BaseWaveShort(); //Short saw tooth wave, with at a specific pitch
	//Left-Right
	Pan(); //Generic panning algorithm
	//Harmonics are generated during the base wave creation
	//Dynamics 
	ReversedDecay();
}

/*
Unused algorithm - 2

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Bandpass
Dynamics - Reversed Decay
*/

void Segment::UnusedAlgorithm2()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	BandPass();

	//Dynamics 
	ReversedDecay();
}

/*
Unused algorithm - 3

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Luminance
Dynamics - Reversed Decay
*/

void Segment::UnusedAlgorithm3()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Gain();

	//Dynamics 
	ReversedDecay();
}

/*
Unused algorithm - 4

Pitch - Y Position (Bandpass)
Left-Right - X Position
Harmonic Content - Luminance
Dynamics - Reversed Decay
*/

void Segment::UnusedAlgorithm4()
{
	//Pitch
	BaseWave();
	BandPass();

	//Left-Right
	Pan();

	//Harmonics
	Gain();

	//Dynamics 
	ReversedDecay();
}

/*
Unused algorithm - 5

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Bandpass
Dynamics - Decay
*/

void Segment::UnusedAlgorithm5()
{
	//Pitch
	BaseWave();

	//Left-Right
	Pan();

	//Harmonics
	BandPass();

	//Dynamics Done already
}

/*
Unused algorithm - 6

Pitch - Y Position (Bandpass)
Left-Right - X Position
Harmonic Content - Bandpass 
Dynamics - Reversed DEcay
*/

void Segment::UnusedAlgorithm6()
{
	//Pitch
	BaseWave();
	BandPass();
	//Left-Right
	Pan();

	//Harmonics
	BandPass();

	//Dynamics
	ReversedDecay();
}

/*
Unused algorithm - 7

Pitch - Y Position (Bandpass)
Left-Right - X Position
Harmonic Content - Bandpass
Dynamics - Reversed Decay
*/

void Segment::UnusedAlgorithm7()
{
	//Pitch
	BaseWave();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();

	//Dynamics Done already
}

/*
Unused algorithm - 8

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Luminance
Dynamics - Eneveloped Lowpass
*/

void Segment::UnusedAlgorithm8()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();

	//Dynamics
	Lowpass();
}

/*
Unused algorithm - 9

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Bandpassed
Dynamics - Enveloped Highpass
*/

void Segment::UnusedAlgorithm9()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();
	BandPass();

	//Dynamics
	Highpass();
}

/*
Unused algorithm - 10

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Luminace
Dynamics - Decay
*/

void Segment::UnusedAlgorithm10()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();

	//Dynamics
	ReversedDecay();
}

/*
Unused algorithm - 11

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Bandpass (pitchshifted?)
Dynamics - Lowpass
*/

void Segment::UnusedAlgorithm11()
{
	//Pitch
	BaseWaveShort();
	BandPass();

	//Left-Right
	Pan();

	//Harmonics
	BandPass();

	//Dynamics
	Lowpass();
}

/*
Unused algorithm - 12

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Bandpass (pitchshifted?)
Dynamics - Highpass
*/

void Segment::UnusedAlgorithm12()
{
	//Pitch
	BaseWaveShort();
	BandPass();

	//Left-Right
	Pan();

	//Harmonics
	BandPass();

	//Dynamics
	Highpass();
}

/*
Unused algorithm - 13

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Luminance
Dynamics - Lowpass
*/

void Segment::UnusedAlgorithm13()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();

	//Dynamics
	Lowpass();
}

/*
Unused algorithm - 14

Pitch - Y Position
Left-Right - X Position
Harmonic Content - Luminance
Dynamics - Highpass
*/

void Segment::UnusedAlgorithm14()
{
	//Pitch
	BaseWaveShort();

	//Left-Right
	Pan();

	//Harmonics
	Luminance();

	//Dynamics
	Highpass();
}