#pragma once
#include<iostream>
#include<string>
using namespace std;

class Segment
{
public:
	Segment();
	~Segment();

	int xPos = 0;
	int yPos = 0;
	int x = 0;
	int y = 0;
	int count = 0;
	int r, g, b = 0;
	int segNumber = 0;
	float area = 0;
	float lrSpec = 0;
	float colR = 0;
	float colG = 0;
	float colB = 0;
	float luminance = 0;
	float maxHeight = 0;
	float maxWidth = 0;
	string path;

	void SegmentToAudio();
	void ProcessRawSeg();
	void PrintSeg();
	void Pan();
	void Envelope();
	void Gain();
	void NormaliseGain();
	void Haas();
	void BaseWave();
	void BaseWave(float freq);
	void BaseWave(float freq, int chans);
	void BaseWavePitched();
	void BaseWavePitched(float freq);
	void BaseWavePitched(float freq, int chans);
	void BaseWaveShort();
	void BaseWaveShort(float freq);
	void BaseWaveShort(float freq, int chans);
	void Louder();
	void Reverb();
	void BandPass();
	void BandPass(float freq, float acuity);
	void ReversedDecay();
	void Lowpass();
	void Lowpass(float freq, float steepness);
	void Highpass();
	void Highpass(float freq, float steepness);
	void Luminance();

	void Algorithm1();
	void Algorithm2();
	void Algorithm3();
	void Algorithm4();

	void UnusedAlgorithm1();
	void UnusedAlgorithm2();
	void UnusedAlgorithm3();
	void UnusedAlgorithm4();
	void UnusedAlgorithm5();
	void UnusedAlgorithm6();
	void UnusedAlgorithm7();
	void UnusedAlgorithm8();
	void UnusedAlgorithm9();
	void UnusedAlgorithm10();
	void UnusedAlgorithm11();
	void UnusedAlgorithm12();
	void UnusedAlgorithm13();
	void UnusedAlgorithm14();
};

