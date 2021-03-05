// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DESCTIPTION COMMENT FOR PORTFOLIO:
// 
// This class was created as part of an assignment I did while in college.
// The assignment was to create an application that could read word search puzzles from a text file, and then solve them as fast as possible.
// 
// This class specifically contains the 2 different puzzle solving algorithms,
// The first is a brute force method, and the second is an improved method.
// The brute force method is much slower and uses a basic recursive search to find every word in the puzzle.
// 
// The improved method benefits from the use of a prefix tree class, as well an improved recursive search algorithm.
// This prefix tree class is a much more efficient way of accesing the word list and comparing it to the puzzle.
// 
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "PuzzleHandler.h"

// Handles creating a new object
PuzzleHandler::PuzzleHandler()
{

}

// Makes a new puzzle
void PuzzleHandler::MakePuzzleFromFile(std::string fileName)
{
	puzzle = file.LoadPuzzle(fileName);											//load puzzle
	MoveList[0] = puzzle->XDim * -1;											//set movelist
	MoveList[1] = (puzzle->XDim * -1) + 1;
	MoveList[2] = 1;
	MoveList[3] = puzzle->XDim + 1;
	MoveList[4] = puzzle->XDim;
	MoveList[5] = puzzle->XDim - 1;
	MoveList[6] = -1;
	MoveList[7] = (puzzle->XDim * -1) - 1;

	std::vector<std::string> Words = puzzle->GetWords();						//get words

	std::vector<char> PuzzleList = puzzle->GetPuzzle();							//get puzzle
	CorrectList.clear();														//clear CorrectList
	std::bitset<1> t_bit;
	short counter = 0;
	t_bit.set(0, 0);
	while (counter < PuzzleList.size())											//loop over PuzzleList
	{
		CorrectList.push_back(t_bit);											//push back normal tile
		counter++;
	}
}

// Returns our CorrectList
std::vector<std::bitset<1> > PuzzleHandler::GetCorrectList()
{
	return CorrectList;
}

// Returns our ErrorList
std::vector<std::string> PuzzleHandler::GetErrorList()
{
	return ErrorList;
}

// Solves the puzzle using a brute force method
void PuzzleHandler::Solve_BruteForce()
{
	success = 'Y';																
	std::vector<std::string> WordList = puzzle->GetWords();						//get words
	std::vector<char> PuzzleList = puzzle->GetPuzzle();							//get puzzle

	CorrectList.clear();														//clear vector
	CorrectBuffer.clear();
	std::bitset<1> t_bit;
	int counter = 0;
	t_bit.set(0, 0);
	while (counter < PuzzleList.size())											//loop over PuzzleList
	{
		CorrectList.push_back(t_bit);											//set to normall tile
		CorrectBuffer.push_back(t_bit);
		counter++;
	}

	short wordCounter = 0;
	short puzzleCounter = 0;
	bool result = false;

	while (wordCounter < WordList.size())										//loop over WordList
	{
		puzzleCounter = 0;
		while (puzzleCounter < PuzzleList.size())								//loop over Puzzle
		{
			if (PuzzleList[puzzleCounter] == WordList[wordCounter].at(0))		//check if the first letter of the word is found
			{
				result = Search(0, puzzleCounter, WordList[wordCounter], PuzzleList);//call recursive files
				if (result)
				{
					puzzleCounter = PuzzleList.size() + 1;						//break out of loop
					Buffer();													//buffer error list
				}
			}
			puzzleCounter++;
		}
		if (!result)
		{
			ErrorList.push_back(WordList[wordCounter]);							//push back not found word
			success = 'N';
		}
		wordCounter++;
	}
}

// Searches the puzzle in the input direction
bool PuzzleHandler::Search(short Dir, short Start, std::string Word, std::vector<char> PuzzleList)
{
	short increment = 0;
	switch (Dir)									//switch on driection
	{
	case 0:
		increment = puzzle->XDim * -1;				//set direction increment
		break;
	case 1:
		increment = (puzzle->XDim * -1) + 1;
		break;
	case 2:
		increment = 1;
		break;
	case 3:
		increment = puzzle->XDim  + 1;
		break;
	case 4:
		increment = puzzle->XDim;
		break;
	case 5:
		increment = puzzle->XDim - 1;
		break;
	case 6:
		increment = -1;
		break;
	case 7:
		increment = (puzzle->XDim * -1) - 1;
		break;
	}

	short counter = 0;
	bool match = true;
	short t_index = Start;
	while (counter < Word.size() && match == true)					//loop over the word size, and stop if match is not true
	{
		if (t_index < PuzzleList.size() && t_index > 0)				//check if index is inside the puzzle
		{
			if (Word.at(counter) != PuzzleList[t_index])			//check if the current letter matches the puzzle index
			{
				match = false;
				BackUp(increment, Start, t_index);					//back up
			}
			else
			{
				CorrectBuffer[t_index].set(0, 1);					//add to the correct buffer
				t_index += increment;								//move 1 tile in given direction
			}
		}
		else
		{
			match = false;											
			BackUp(increment, Start, t_index);						//back up
		}
		counter++;
	}

	if (match == false)												
	{
		if (Dir == 7)												//check if we have searched all directions
		{
			return match;
		}
		else
		{
			return Search(Dir + 1, Start, Word, PuzzleList);		//search in the next direction
		}
	}
	else
	{
		return match;
	}
}

// Handles buffer for the CorrectList
void PuzzleHandler::Buffer()
{
	for (short counter = 0; counter < CorrectBuffer.size(); counter++)	//loop over CorrectBuffer
	{
		if (CorrectBuffer[counter] == 1)								//check if current tile is correct
		{
			CorrectList[counter].set(0, 1);								//add correct tile to CorrectList
			CorrectBuffer[counter].set(0, 0);							//set CorrctBuffer back to normal
		}
	}
}

// Handles backing up the puzzle index
void PuzzleHandler::BackUp(short increment, short Start, short t_Index)
{
	increment = -increment;												//set to increment backwards

	if (t_Index > Start)												//check if we should move backwards
	{
		while (t_Index >= Start)										
		{
			if (t_Index > -1 && t_Index < CorrectBuffer.size())			//check if index is inside CorrectBuffer
			{
				CorrectBuffer[t_Index].set(0, 0);						//set Correct Buffer to normal
			}
			t_Index += increment;	
		}
	}
	else
	{
		while (t_Index <= Start)
		{
			if (t_Index > -1 && t_Index < CorrectBuffer.size())
			{
				CorrectBuffer[t_Index].set(0, 0);
			}
			t_Index += increment;
		}
	}
}

// Solves the puzzle using an improved method
void PuzzleHandler::Solve_Improved()
{
	success = 'Y';
	std::vector<char> PuzzleList = puzzle->GetPuzzle();						//get puzzle
	Trie* WordTrie = puzzle->GetWordTrie();									//get word list
	CheckList.clear();
	CorrectIndex.clear();

	bool result = false;													
	std::string t_char;
	short direction;
	for (short counter = 0; counter < PuzzleList.size(); counter++)			//loop over puzzleList
	{
		if (WordTrie->root->CheckForChild(PuzzleList[counter]))				//check if the trie root has any child matching the current puzzle index
		{
			direction = 0;													
			while (direction < 8)											//loop over all directions
			{
				t_char = PuzzleList[counter];								//get current puzzle letter
				result = CheckDirection(direction, counter, PuzzleList, string(t_char), WordTrie->root->FindChild(PuzzleList[counter]));//call recursive method
																																			 
				if (result)													//check if it found a word
				{	
					CorrectIndex.push_back(firstIndex);						//push back index
				}
				direction++;
			}
		}
	}
	if (CheckList.size() != 0)												
	{
		std::sort(CheckList.begin(), CheckList.end());						//sort CheckList
	}
	if (CheckList != puzzle->GetWords())									//compare the words found to list of words
	{
		success = 'N';														//set success to no
	}
	else
	{
		for (short counter = 0; counter < CorrectIndex.size(); counter++)	//loop over correctIndex
		{
			CorrectList[CorrectIndex[counter]].set(0, 1);					//set current tile to correct
		}
	}
}

// Recursively checks for any word in a given direction
bool PuzzleHandler::CheckDirection(short Dir, short Start, std::vector<char> PuzzleList, std::string string, Node* node)
{
	Trie* WordTrie = puzzle->GetWordTrie();									//get WordTrie
	short t_index = 0;
	std::string t_string;

	if (WordTrie->SearchWord(string))										//check if string is valid word
	{
		CheckList.push_back(string);										//push word onto checkList
		WordTrie->RemoveWord(string);										//remove word from WordTrie
		return true; 
	}

	t_index = Start + MoveList[Dir];										//set t_index to 1 tile in current direction
	if (t_index >= 0 && t_index < PuzzleList.size())						//check if t_index is inside puzzle
	{
		if (node->CheckForChild(PuzzleList[t_index]))						//check if current node has a child matching current puzzle letter
		{
			if (string.size() == 1) { firstIndex = Start; }					//if we are at the first letter of the word, set firstIndex to Start
			t_string = PuzzleList[t_index];									//set t_string to current Puzzle letter
			string.append(t_string);										//add current puzzle letter to string
			if (CheckDirection(Dir, t_index, PuzzleList, string, node->FindChild(PuzzleList[t_index])))	//recursively check next tile in direction
			{
				CorrectIndex.push_back(t_index);							//push back the current index onto CorrectIndex vetor
				return true;
			}
		}
	}
	return false;
}