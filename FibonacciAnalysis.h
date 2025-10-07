#pragma once

#include "Candlestick.h"
#include "PeakValleyDetector.h"
using namespace System;
using namespace System::Collections::Generic;

namespace CppCLRWinFormsProject {

    public ref class FibonacciAnalysis
    {
    private:
        array<double>^ fibLevels;

    public:
        property PeakValleyDetector::Point^ StartPoint;
        property PeakValleyDetector::Point^ EndPoint;
        property DateTime StartDate;
        property DateTime EndDate;
        property double StartValue;
        property double EndValue;
        property bool IsUpWave;
        property array<double>^ FibonacciValues;
        property array<List<Candlestick^>^>^ Confirmations;

        // Default constructor
        FibonacciAnalysis()
        {
            // Initialize Fibonacci levels (cannot use initializer lists in C++/CLR)
            fibLevels = gcnew array<double>(7);
            fibLevels[0] = 0.0;
            fibLevels[1] = 0.236;
            fibLevels[2] = 0.382;
            fibLevels[3] = 0.5;
            fibLevels[4] = 0.618;
            fibLevels[5] = 0.786;
            fibLevels[6] = 1.0;

            FibonacciValues = gcnew array<double>(fibLevels->Length);
            Confirmations = gcnew array<List<Candlestick^>^>(fibLevels->Length);
            for (int i = 0; i < Confirmations->Length; i++)
                Confirmations[i] = gcnew List<Candlestick^>();
        }

        // Constructor with start and end points
        FibonacciAnalysis(PeakValleyDetector::Point^ start, PeakValleyDetector::Point^ end)
            : FibonacciAnalysis()
        {
            StartPoint = start;
            EndPoint = end;
            StartDate = start->Candlestick->Date;
            EndDate = end->Candlestick->Date;

            if (start->Type == PeakValleyDetector::PointType::Valley)
            {
                StartValue = System::Convert::ToDouble(start->Candlestick->Low);
                EndValue = System::Convert::ToDouble(end->Candlestick->High);
                IsUpWave = true;
            }
            else
            {
                StartValue = System::Convert::ToDouble(start->Candlestick->High);
                EndValue = System::Convert::ToDouble(end->Candlestick->Low);
                IsUpWave = false;
            }
        }

        void CalculateFibonacciValues()
        {
            double range = EndValue - StartValue;
            for (int i = 0; i < fibLevels->Length; i++)
            {
                FibonacciValues[i] = StartValue + (range * fibLevels[i]);
            }
            // If it's a down wave, reverse the order
            if (!IsUpWave)
                Array::Reverse(FibonacciValues);
        }

        // Overload WITHOUT default param (C++/CLI cannot have default values in ref class member functions)
        void DetectConfirmations(List<Candlestick^>^ candlesticks)
        {
            DetectConfirmations(candlesticks, 0.0001);
        }

        void DetectConfirmations(List<Candlestick^>^ candlesticks, double tolerance)
        {
            // Clear previous confirmations
            for (int i = 0; i < Confirmations->Length; i++)
                Confirmations[i]->Clear();

            // Find candlesticks ONLY within the date range
            List<Candlestick^>^ relevantCandlesticks = gcnew List<Candlestick^>();
            for each (Candlestick ^ c in candlesticks)
            {
                if (c->Date >= StartDate && c->Date <= EndDate)
                    relevantCandlesticks->Add(c);
            }

            // Check for confirmations within tolerance
            for each (Candlestick ^ candle in relevantCandlesticks)
            {
                for (int i = 0; i < FibonacciValues->Length; i++)
                {
                    double fibValue = FibonacciValues[i];
                    if (IsWithinTolerance(System::Convert::ToDouble(candle->Open), fibValue, tolerance) ||
                        IsWithinTolerance(System::Convert::ToDouble(candle->High), fibValue, tolerance) ||
                        IsWithinTolerance(System::Convert::ToDouble(candle->Low), fibValue, tolerance) ||
                        IsWithinTolerance(System::Convert::ToDouble(candle->Close), fibValue, tolerance))
                    {
                        Confirmations[i]->Add(candle);
                    }
                }
            }
        }

        bool IsWithinTolerance(double price, double target, double tolerance)
        {
            double relativeDistance = Math::Abs(price - target) / target;
            return relativeDistance <= tolerance;
        }

        int GetTotalConfirmationCount()
        {
            int count = 0;
            for (int i = 0; i < Confirmations->Length; i++)
                count += Confirmations[i]->Count;
            return count;
        }
    };

}