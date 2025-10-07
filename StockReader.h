#pragma once
#include "Candlestick.h" // This imports the Candlestick class.

using namespace System;
using namespace System::Collections::Generic; // This imports the namespace containing List<T>.
using namespace System::IO;  // This imports the namespace containing StreamReader and other handling classes.

namespace CppCLRWinFormsProject { //This is the namespace for our overall project
    /// <summary>
    /// This class reads stock data from a file and returns a list of Candlestick objects.
    /// </summary>

    public ref class StockReader {
        /// <summary>
        /// This method reads stock data from a file and converts each row into a candlestick object.
        /// </summary>
        /// <param name="filePath"> This string contains the complete path to the stock data file which is needed to be processed.</param>
        /// <returns>This returns a list which will contain all the rows of the stock data file as candlestick objects.</returns>
    public:
        List<Candlestick^>^ ReadCandlestickFromFile(String^ filePath) {
            // Create a new managed list to hold the Candlestick objects.
            List<Candlestick^>^ candlesticks = gcnew List<Candlestick^>();

            // Create a StreamReader to read the file.
            StreamReader^ reader = gcnew StreamReader(filePath);
            try { // Try to read the file.
                // Read the first line (it is the name of the columns) and discard it.
                String^ line = reader->ReadLine();
                // Read each subsequent line until end of file.
                while ((line = reader->ReadLine()) != nullptr) {
                    // Create a new Candlestick object from the line.
                    Candlestick^ candlestick = gcnew Candlestick(line);
                    // Add it to the list.
                    candlesticks->Add(candlestick);
                }
            }
            finally {
                // Ensure the reader is properly disposed.
                if (reader != nullptr) {
                    delete reader;
                }
            }
            return candlesticks; // Return the list of Candlestick objects.
        }
    };

}
