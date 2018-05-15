#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Segment.h"
class SegmentCombiner
{
public:
	SegmentCombiner();
	~SegmentCombiner();

	void CombineSegments(int, vector<Segment>, string); //Unused
	void CombinationAlgorithm1(int, vector<Segment>, string, string); //ALgo 1/4
	void CombinationAlgorithm2(int, vector<Segment>, string, string); //Algo 2
	//Algo 3 is done in ImageToSegments
};

