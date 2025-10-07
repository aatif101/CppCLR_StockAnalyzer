#pragma once
using namespace System; // This imports the namespace which contains the core functionality.
using namespace System::Globalization; // This imports the namespace which contains the CultureInfo class. This class is used to format the date.

namespace CppCLRWinFormsProject { //This is the namespace for our overall project

    public ref class Candlestick {
        /// <summary>
        /// This represents a single candlestick in a stock chart. This contains the date, open, high, low, close, and volume of the stock.
        /// </summary>
    public:
        property DateTime Date; // This is the date of the candlestick
        property Decimal Open; // This is the opening price of the candlestick
        property Decimal High; // This is the highest price of the candlestick
        property Decimal Low; // This is the lowest price of the candlestick
        property Decimal Close; // This is the closing price of the candlestick
        property unsigned __int64 Volume; // This is the volume of the candlestick

        /// <summary>
        /// This is the constructor for the Candlestick class. It takes a string as input and parses it to create a new Candlestick object.
        /// </summary>
        /// <param name="data">A string containing comma-separated values in the format: date,open,high,low,close,volume</param>
        Candlestick(String^ data) {
            // Define the separators: comma and double-quote
            array<wchar_t>^ separators = gcnew array<wchar_t> { ',', '\"' }; // This is an array of characters that are used as separators in the data string.
            array<String^>^ values = data->Split(separators, StringSplitOptions::RemoveEmptyEntries); // This splits the data string into an array of strings using the separators defined above.
            if (values->Length != 6) { // This checks if the length of the values array is not equal to 6. If it is not, an exception is thrown.
                throw gcnew ArgumentException("Invalid data format. Expected 5 values separated by commas.");
            }
            Date = DateTime::ParseExact(values[0], "yyyy-MM-dd", CultureInfo::InvariantCulture); // This exactly parses the first value in the values array as a DateTime object using the specified format.
            Open = Decimal::Parse(values[1]); // This parses the second value in the values array as a Decimal object. This is the opening price of the candlestick.
            High = Decimal::Parse(values[2]); // This parses the third value in the values array as a Decimal object. This is the highest price of the candlestick.
            Low = Decimal::Parse(values[3]); // This parses the fourth value in the values array as a Decimal object. This is the lowest price of the candlestick.
            Close = Decimal::Parse(values[4]); // This parses the fifth value in the values array as a Decimal object. This is the closing price of the candlestick.
            Volume = UInt64::Parse(values[5]); // This parses the sixth value in the values array as an unsigned __int64 object. This is the volume of the candlestick.
        }

        /// <summary>
        /// This is the constructor for the Candlestick class. It takes individual values as input and creates a new Candlestick object.
        /// </summary>
        /// <param name="date">the trading date of the stock</param>
        /// <param name="open">the opening price of the stock on the particular date</param>
        /// <param name="high">the highest price of the stock on a particular date</param>
        /// <param name="low">the lowest price of the stock on a particular date</param>
        /// <param name="close">the closing price of the stock on a particular date</param>
        /// <param name="volume">the totla volume of the stock traded on that particular date.</param>
        Candlestick(DateTime date, Decimal open, Decimal high, Decimal low, Decimal close, unsigned __int64 volume) {
            Date = date; // This assigns the date parameter to the Date property of the Candlestick object.
            Open = open; // This assigns the open parameter to the Open property of the Candlestick object.
            High = high; // This assigns the high parameter to the High property of the Candlestick object.
            Low = low; // This assigns the low parameter to the Low property of the Candlestick object.
            Close = close;  // This assigns the close parameter to the Close property of the Candlestick object.
            Volume = volume; // This assigns the volume parameter to the Volume property of the Candlestick object.
        }
    };

}
