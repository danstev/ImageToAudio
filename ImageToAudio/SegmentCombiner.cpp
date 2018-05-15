#include "stdafx.h"
#include "SegmentCombiner.h"
#include <iostream>
#include <vector>
#include <string>
#include "Segment.h"

SegmentCombiner::SegmentCombiner()
{
}


SegmentCombiner::~SegmentCombiner()
{
}

//Was used in testing, moved away to the resulting combination algorithms.
void SegmentCombiner::CombineSegments(int count, vector<Segment> segs, string path)
{
	
}

/*
CombinationAlgorithm1 takes a list of segments and creates a file which has the segments playing concurrently.
*/
void SegmentCombiner::CombinationAlgorithm1(int count, vector<Segment> segs, string path, string output)
{
	//Create empty file to fill up.
	string empty = "synth silence image.wav 44100 2 1";
	system(empty.c_str());
	int last = 0;

	//For every segment
	for each(Segment s in segs)
	{
		//Check if its not last segment
		if (s.segNumber > last)
		{
			last = s.segNumber;
		}

		//If first segment
		if (s.segNumber == 0)
		{
			//Add current segment to the blank file.
			string first = "nmix image.wav " + (path + "0.wav") + " image" + to_string(s.segNumber) + ".wav";
			system(first.c_str());

			//cleanup
			string del = "del image.wav";
			system(del.c_str());

			//cleanup
			del = "del " + (path + "0.wav");
			system(del.c_str());
			
		}
		else
		{
			//Construct arguements correctly.
			//Adds current segment to the file.
			string c = "image"+ to_string(s.segNumber)+ ".wav"; //current seg
			string b = "image" + to_string(s.segNumber - 1) + ".wav"; //previous seg
			string a = (path + to_string(s.segNumber) + ".wav"); //new file
			cout << a << endl;
			string combine = ("nmix " + b + " ") + a + (" " + c);
			system(combine.c_str());

			//cleanuup
			string del = "del " + b;
			system(del.c_str());

			//cleanup
			del = "del " + (path + to_string(s.segNumber) + ".wav");
			system(del.c_str());
		}
	}

	//Gain normalisation
	string loud = "modify loudness 2 image" + to_string(last) + ".wav " + output + " 6";
	system(loud.c_str());

	//cleaup
	string del = "del image" + to_string(last) +".wav";
	system(del.c_str());

	cout << "End file is: " + to_string(last) + "image.wav" << endl;
}

/*
CombinationAlgorithm2 takes a list of segments and has them played one after another.
*/

void SegmentCombiner::CombinationAlgorithm2(int count, vector<Segment> segs, string path, string output)
{
	int amount = 0;
	for each(Segment s in segs)
	{
		//iterates through all the segments and adds their files to a text file
		string a = "echo " + (path + to_string(s.segNumber) + ".wav") +">> conc.txt";
		system(a.c_str());
		amount++;
	}

	//Concantenation works via a list of the files to be added together and an output file.
	string concantenate = "njoin conc.txt " + output;
	system(concantenate.c_str());

	//cleaup
	for (int i = 0; i < amount; i++)
	{
		string del = "del "+ (path + to_string(i)) + ".wav";
		system(del.c_str());
	}

	//cleaup
	string del = "del conc.txt";
	system(del.c_str());

	cout << "Algorithm file is: " + output << endl;
}

