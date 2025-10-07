#pragma once
#include "Candlestick.h" // Include the Candlestick header file
using namespace System; // Use the System namespace
using namespace System::Collections::Generic; // Use the System.Collections.Generic namespace

namespace CppCLRWinFormsProject { // Use the CppCLRWinFormsProject namespace

	public ref class PeakValleyDetector { // Create a PeakValleyDetector class
    public:
        /// <summary>
        /// Enumeration for the type of point.
        /// </summary>
        enum class PointType {
            None,
            Peak,
            Valley
        };

        /// <summary>
        /// Represents a point (peak or valley) in a list of candlesticks.
        /// </summary>
        ref class Point {
        public:
            /// <summary>
            /// The index of the candlestick in the list.
            /// </summary>
            property int Index;

            /// <summary>
            /// The type of the point (Peak or Valley).
            /// </summary>
            property PointType Type;

            /// <summary>
            /// The candlestick at this point.
            /// </summary>
            property Candlestick^ Candlestick;
        };

        /// <summary>
        /// Detects peaks and valleys in a list of candlesticks based on a specified margin.
        /// </summary>
        /// <param name="candlesticks">List of candlesticks to analyze.</param>
        /// <param name="margin">Number of candlesticks to check on each side.</param>
        /// <returns>List of points identified as peaks or valleys.</returns>
        List<Point^>^ DetectPeaksAndValleys(List<Candlestick^>^ candlesticks, int margin) {
			List<Point^>^ points = gcnew List<Point^>(); // Create a list of points

            // Skip if not enough data
            if (candlesticks->Count < (2 * margin + 1))
				return points; // Return an empty list

			for (int i = margin; i < candlesticks->Count - margin; i++) { // Iterate over the candlesticks
				bool isPeak = true; // Initialize the isPeak variable to true
				bool isValley = true; // Initialize the isValley variable to true

                // Check if the current candlestick is a peak.
				for (int j = i - margin; j <= i + margin; j++) { // Iterate over the margin
					if (j == i) // Skip if the index is the same as the current candlestick
                        continue;
					if (candlesticks[j]->High >= candlesticks[i]->High) { // Check if the high of the current candlestick is less than the high of the candlestick at index j
						isPeak = false; // Set isPeak to false
                        break;
                    }
                }

                // Check if the current candlestick is a valley.
				for (int j = i - margin; j <= i + margin; j++) { // Iterate over the margin
					if (j == i) // Skip if the index is the same as the current candlestick
                        continue;
					if (candlesticks[j]->Low <= candlesticks[i]->Low) { // Check if the low of the current candlestick is greater than the low of the candlestick at index j
                        isValley = false;
                        break;
                    }
                }
				// Add the point to the list if it is a peak or valley.
				if (isPeak) { // Check if the current candlestick is a peak
                    Point^ pt = gcnew Point();
                    pt->Index = i;
                    pt->Type = PointType::Peak;
                    pt->Candlestick = candlesticks[i];
                    points->Add(pt);
                }
				if (isValley) { // Check if the current candlestick is a valley
                    Point^ pt = gcnew Point();
                    pt->Index = i;
                    pt->Type = PointType::Valley;
                    pt->Candlestick = candlesticks[i];
                    points->Add(pt);
                }
            }
			return points; // Return the list of points
        }
    };
}
