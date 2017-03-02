#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

struct memoryObject
{
	int value;
	int count;
	bool used;
	memoryObject(int val)
	{
		value = val;
		count = 1;
		used = false;
	}
};

//Header Stuff
void FIFO(vector<int>, int);
void LRU(vector<int>, int);
void MFU(vector<int>, int);
void OPT(vector<int>, int);
vector<int> ReadInputFile();
void ScreenWrite(string, float, float, float, float, float);
void WriteHeader(int);
//End Header Stuff

char* fileName;
ofstream outFile;

int main(int argc, char* argv[])
{
	cout << "Args: " << argc << endl;
	int frameSize;
	string inFile;
	if(argc >= 4)
	{
		frameSize = atoi(argv[1]);
		fileName = argv[2];
		outFile.open(argv[3]);
		vector<int> requests = ReadInputFile();
		WriteHeader(frameSize);
		FIFO(requests, frameSize);
		LRU(requests, frameSize);
		MFU(requests, frameSize);
		OPT(requests, frameSize);
		outFile.close();
	}
	else
	{
		cout << "Invalid command please try again" << endl;
	}
}

vector<int> ReadInputFile()
{
	vector<int> memoryRequests;
	ifstream file;
	cout << "Reading File: " << fileName << endl;
	file.open(fileName);
	if(file.is_open())
	{
		int read;
		while(file >> read)
		{
			memoryRequests.push_back(read);
		}
		cout << "Found " << memoryRequests.size() << " Entries" << endl;
		return memoryRequests;
	}
	else
	{
		cout << "File not found" << endl;
		return memoryRequests;
	}
}

void FIFO(vector<int> memoryRequests, int frameSize)
{
	int existCount = 0;
	float twoKRate = 0;
	float fourKRate = 0;
	float sixKRate = 0;
	float eightKRate = 0;
	float tenKRate = 0;
	bool alreadyExists = false;
	vector<int> frame;
	for(int i = 0; i < memoryRequests.size(); i++)
	{
		alreadyExists = false;
		int currentRequest = memoryRequests[i];
		if(frame.size() > 0)
		{
			for(int k = 0; k < frame.size(); k++)
			{
				if(frame[k] == currentRequest)
				{
					alreadyExists = true;
					break;
				}
			}
		}
		if(alreadyExists)
		{
			//done no change
		}
		else
		{
			frame.push_back(currentRequest);
			if(frame.size() > frameSize)
			{
				frame.erase(frame.begin());//Remove end of list if overflowing
			}
			if(i < 2001)
			{
				twoKRate++;
			}
			else if (i < 4001)
			{
				fourKRate++;
			}
			else if (i < 6001)
			{
				sixKRate++;
			}
			else if (i < 8001)
			{
				eightKRate++;
			}
			else
			{
				tenKRate++;
			}
		}
	}
	ScreenWrite("FIFO", twoKRate, fourKRate, sixKRate, eightKRate, tenKRate);
}

void LRU(vector<int> memoryRequests, int frameSize)
{
	float twoKRate = 0;
	float fourKRate = 0;
	float sixKRate = 0;
	float eightKRate = 0;
	float tenKRate = 0;
	vector<int> frame;
	bool alreadyExists = false;
	int index = 0;
	for(int i = 0; i < memoryRequests.size(); i++)
	{
		alreadyExists = false;
		int currentRequest = memoryRequests[i];
		for(int k = 0; k < frame.size(); k++)
		{
			if(frame[k] == currentRequest)
			{
				alreadyExists = true;
				index = k;
				break;
			}
		}
		if(alreadyExists)
		{//Move to the back as it is the most recently used.
			int temp = frame[index];
			frame.erase(frame.begin()+index);
			frame.push_back(temp);
		}
		else
		{
			frame.push_back(currentRequest);
			if(frame.size() > frameSize)
			{
				frame.erase(frame.begin());//Remove beginning of list as that is the least recently used
			}
			if(i < 2001)
			{
				twoKRate++;
			}
			else if (i < 4001)
			{
				fourKRate++;
			}
			else if (i < 6001)
			{
				sixKRate++;
			}
			else if (i < 8001)
			{
				eightKRate++;
			}
			else
			{
				tenKRate++;
			}
		}
	}
	ScreenWrite("LRU", twoKRate, fourKRate, sixKRate, eightKRate, tenKRate);
}

void MFU(vector<int> memoryRequests, int frameSize)
{
	float twoKRate = 0;
	float fourKRate = 0;
	float sixKRate = 0;
	float eightKRate = 0;
	float tenKRate = 0;
	bool alreadyExists = false;
	vector<memoryObject> frame;
	int index = 0;
	for(int i = 0; i < memoryRequests.size(); i++)
	{
		alreadyExists = false;
		int current = memoryRequests[i];
		for(int k = 0; k < frame.size(); k++)
		{
			if(frame[k].value == current)
			{
				alreadyExists = true;
				frame[k].count++;
				index = k;
				break;
			}
		}
		if(alreadyExists)
		{//Add to use count
			frame[index].count++;
		}
		else
		{
			if(frame.size() > frameSize)
			{
				int tempObj = frame[0].count;
				int indexJ = 0;
				for(int j = 1; j < frame.size();j++)
				{
					if(frame[j].count > tempObj)
					{
						tempObj = frame[j].count;
						indexJ = j;
					}
				}
				frame.erase(frame.begin()+indexJ);//remove MFU
			}
			memoryObject currentRequest(current);
			frame.push_back(currentRequest);
			if(i < 2001)
			{
				twoKRate++;
			}
			else if (i < 4001)
			{
				fourKRate++;
			}
			else if (i < 6001)
			{
				sixKRate++;
			}
			else if (i < 8001)
			{
				eightKRate++;
			}
			else
			{
				tenKRate++;
			}
		}
	}
	ScreenWrite("MFU", twoKRate, fourKRate, sixKRate, eightKRate, tenKRate);
}

//Due to the forward scanning this will be the least efficient in terms of processing time.
void OPT(vector<int> memoryRequests, int frameSize)
{
	float twoKRate = 0;
	float fourKRate = 0;
	float sixKRate = 0;
	float eightKRate = 0;
	float tenKRate = 0;
	bool alreadyExists = false;
	vector<memoryObject> frame;
	int index = 0;
	for(int i = 0; i < memoryRequests.size(); i++)
	{
		alreadyExists = false;
		int current = memoryRequests[i];
		for(int k = 0; k < frame.size(); k++)
		{
			if(frame[k].value == current)
			{
				alreadyExists = true;
				frame[k].count++;
				index = k;
				break;
			}
		}
		if(alreadyExists)
		{//Add to use count
			frame[index].count++;
		}
		else
		{			
			if(frame.size() >= frameSize)
			{
				vector<memoryObject> tempList = frame;
				bool contained = false;
				for(int x = 0; x < tempList.size(); x++)
				{
					if(!tempList[x].used)
					{
						for(int j = i; j < memoryRequests.size(); j++)
						{
							if(memoryRequests[j] == tempList[x].value)
							{
								tempList[x].count = j;//count is used as index in this.
								tempList[x].used = true;
								break;
							}
						}
					}
				}
				int tempObj = tempList[0].count;
				int indexJ = 0;
				for(int j = 1; j < tempList.size(); j++)
				{
					if(!tempList[j].used)
					{
						indexJ = j;
						break;
					}
					if(tempList[j].used && tempList[j].count > tempObj)
					{
						tempObj = tempList[j].count;
						indexJ = j;
					}
				}
				frame.erase(frame.begin()+indexJ);
			}
			memoryObject currentRequest(current);
			frame.push_back(currentRequest);
			if(i < 2001)
			{
				twoKRate++;
			}
			else if (i < 4001)
			{
				fourKRate++;
			}
			else if (i < 6001)
			{
				sixKRate++;
			}
			else if (i < 8001)
			{
				eightKRate++;
			}
			else
			{
				tenKRate++;
			}
		}
	}
	ScreenWrite("Optimal", twoKRate, fourKRate, sixKRate, eightKRate, tenKRate);
}

void WriteHeader(int size)
{
	outFile << "========================================================================"<< endl;
	outFile << "	Page Replacement Algorithm Simulation (frame size = " << size << ")" << endl;
	outFile << "========================================================================"<< endl; 
	outFile << " NOTE: OPT may take a few minutes to process" << endl;
	outFile << " _______________________________________________________________________" << endl;
	outFile << "| Algorithm | Total Page Faults | 2000  | 4000  | 6000  | 8000  | 10000 |" << endl;
	outFile << "|-----------------------------------------------------------------------|" << endl;
}

void ScreenWrite(string algo, float two, float four, float six, float eight, float ten)
{
	double scale = 0.001;
	outFile << "| " << left << setw(10) << setfill(' ') << algo;
	outFile << "| " << left << setw(18) << setfill(' ') << (two + four + six + eight + ten);
	double twoRound = (two / 2000);
	twoRound = (int)(twoRound / scale) * scale;
	outFile << "| " << left << setw(6) << setfill(' ') << twoRound;
	double fourRound = ((two + four) / 4000);
	fourRound = (int)(fourRound / scale) * scale;
	outFile << "| " << left << setw(6) << setfill(' ') << fourRound;
	double sixRound = ((two + four + six) / 6000);
	sixRound = (int)(sixRound / scale) * scale;
	outFile << "| " << left << setw(6) << setfill(' ') << sixRound;
	double eightRound = ((two + four + six + eight) / 8000);
	eightRound = (int)(eightRound / scale) * scale;
	outFile << "| " << left << setw(6) << setfill(' ') << eightRound;
	double tenRound = ((two + four + six + eight + ten) / 10000);
	tenRound = (int)(tenRound / scale) * scale;
	outFile << "| " << left << setw(6) << setfill(' ') << tenRound << "|" << endl;
}