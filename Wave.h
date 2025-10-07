// Wave.h
#pragma once
using namespace System; // This is the namespace for the .NET framework

namespace CppCLRWinFormsProject { // This is the namespace for the project
	public ref class Wave // This is the Wave class definition
    {
    public:
		// Properties of the Wave class
        property DateTime StartDate;
        property DateTime EndDate;
        property double StartValue;
        property double EndValue;
        property bool IsUpWave;
        property int StartIndex;
        property int EndIndex;

		Wave() // Constructor for the Wave class
		{
			StartValue = 0.0; // This initializes the start value to 0.
			EndValue = 0.0; // This initializes the end value to 0.
			IsUpWave = false; // This initializes the isUpWave property to false.
			StartIndex = 0; // This initializes the start index to 0.
			EndIndex = 0; // This initializes the end index to 0.
		}
        // ToString method using .NET String
        virtual String^ ToString() override
        {
			return StartDate.ToShortDateString() + " to " + EndDate.ToShortDateString(); // This returns a string representation of the wave.
        }
    };
}
