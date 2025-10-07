#pragma once
#include "Candlestick.h"
#include "PeakValleyDetector.h"
#include "Wave.h"

// Include necessary .NET namespaces
using namespace System;
using namespace System::Collections::Generic;

namespace CppCLRWinFormsProject { // This is the namespace for the project

	public ref class WaveDetector // This is the WaveDetector class definition
    {
    private:
        // Static comparison function for sorting points by index
        static int ComparePoints(PeakValleyDetector::Point^ p1, PeakValleyDetector::Point^ p2)
        {
			return p1->Index - p2->Index; // This returns the difference between the indices of the two points.
        }

    public:
        /// <summary>
        /// Detects waves based on peaks and valleys.
        /// </summary>
        List<Wave^>^ DetectWaves(List<Candlestick^>^ candlesticks, List<PeakValleyDetector::Point^>^ points)
        {
			List<Wave^>^ waves = gcnew List<Wave^>(); // This is the list of waves that will be returned.

			if (points->Count < 2) // If there are less than 2 points, return an empty list of waves.
                return waves;

            // Sort points by index using the static comparison function
			array<PeakValleyDetector::Point^>^ pointsArray = points->ToArray(); // This converts the list of points to an array.
			Array::Sort(pointsArray, gcnew Comparison<PeakValleyDetector::Point^>(ComparePoints)); // This sorts the array of points based on the index.
			List<PeakValleyDetector::Point^>^ sortedPoints = gcnew List<PeakValleyDetector::Point^>(pointsArray); // This converts the sorted array back to a list.

			for (int i = 0; i < sortedPoints->Count - 1; i++) // This is a loop that iterates over the sorted points.
            {
				PeakValleyDetector::Point^ currentPoint = sortedPoints[i]; // This gets the current point.
				PeakValleyDetector::Point^ nextPoint = sortedPoints[i + 1]; // This gets the next point.

                // Skip if both points are of the same type
                if (currentPoint->Type == nextPoint->Type)
                    continue;

				bool isUpWave = currentPoint->Type == PeakValleyDetector::PointType::Valley; // This checks if the current point is a valley.

				Wave^ wave = gcnew Wave(); // This creates a new wave object.
				wave->StartDate = currentPoint->Candlestick->Date; // This sets the start date of the wave to the date of the current point.
				wave->EndDate = nextPoint->Candlestick->Date; // This sets the end date of the wave to the date of the next point.
				wave->StartValue = Decimal::ToDouble(isUpWave ? currentPoint->Candlestick->Low : currentPoint->Candlestick->High); // This sets the start value of the wave based on the type of the current point.
				wave->EndValue = Decimal::ToDouble(isUpWave ? nextPoint->Candlestick->High : nextPoint->Candlestick->Low); // This sets the end value of the wave based on the type of the next point.
				wave->IsUpWave = isUpWave; // This sets the isUpWave property of the wave.
				wave->StartIndex = currentPoint->Index; // This sets the start index of the wave to the index of the current point.
				wave->EndIndex = nextPoint->Index; // This sets the end index of the wave to the index of the next point.

				waves->Add(wave); // This adds the wave to the list of waves.
            }

			return waves; // This returns the list of waves.
        }
    };
}
