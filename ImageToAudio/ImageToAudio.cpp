#include "stdafx.h"


#include <iostream>
#include <vector>
#include <string>
#include "ImageToSegment.cpp"
#include "Segment.h"
#include "SegmentCombiner.h"

using namespace std;
ImageToSegment i;
SegmentCombiner combiner;

/*
Main class, takes in two arguements:
	1. picture
	2. which algorithm (1-4)
*/

int main(int argc, char** argv)
{
	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	//1: segment image
	vector<Segment> segs = i.imageToSeg(argv[1]);
	//2: segment to audio
	int count = 0;
	for each (Segment seg in segs)
	{
		count++;
		seg.maxHeight = i.h;
		seg.maxWidth = i.m;
		seg.ProcessRawSeg();
		seg.SegmentToAudio();
	}
	
	//3: audio combination
		combiner.CombinationAlgorithm2(count, segs, "panned", "Algorithm1.wav"); //1 
		combiner.CombinationAlgorithm1(count, segs, "volpannedenvel", "Algorithm2.wav"); //2
		combiner.CombinationAlgorithm2(count, segs, "pannedbpbaseWave", "Algorithm4.wav"); //4 
	waitKey(0);
}