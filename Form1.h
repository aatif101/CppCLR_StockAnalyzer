

// Include the necessary header files
#pragma once
#include "Candlestick.h"
#include "StockReader.h"
#include "PeakValleyDetector.h"
#include "WaveDetector.h"
#include "Wave.h"
#include "FibonacciAnalysis.h"

// Define the namespace for the project
namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Globalization;
	using namespace System::Windows::Forms::DataVisualization::Charting;

	/// <summary>
	/// 
	/// 
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	private: // Private members
		List<Candlestick^>^ ListOfCandlesticks; // This is the list of candlesticks which will be used to store the candlestick data.
		StockReader^ stockReader; // This is the stock reader object which will be used to read the stock data from the file.
		PeakValleyDetector^ peakValleyDetector; // This is the peak valley detector object which will be used to detect peaks and valleys in the stock data.
		WaveDetector^ waveDetector; // This is the wave detector object which will be used to detect waves in the stock data.
		List<PeakValleyDetector::Point^>^ points; // This is the list of points which will be used to store the peaks and valleys detected in the stock data.
		List<Wave^>^ upWaves; // This is the list of up waves which will be used to store the up waves detected in the stock data.
		List<Wave^>^ downWaves; // This is the list of down waves which will be used to store the down waves detected in the stock data.
		int margin; // This is the margin value which will be used to detect peaks and valleys in the stock data.

		bool isRubberBanding = false;
		PeakValleyDetector::Point^ rubberBandStartPoint;
		int rubberBandEndIndex;
		FibonacciAnalysis^ currentFibAnalysis;
		LineAnnotation^ rubberBandLine;
		RectangleAnnotation^ rubberBandRect;
	private: System::Windows::Forms::HScrollBar^ hScrollBar_Tolerance_Scroll;
	private: System::Windows::Forms::Label^ label2;
		   double tolerance = 0.0001; // Default tolerance value
	private: System::Windows::Forms::Timer^ timer_Simulation;
	private: System::Windows::Forms::Button^ button_StartStop;
	private: System::Windows::Forms::Button^ button_Increase;
	private: System::Windows::Forms::Button^ button_Decrease;
	private: System::Windows::Forms::Label^ label_Price;
	private: System::Windows::Forms::HScrollBar^ hScrollBar_PercentRange;
	private: System::Windows::Forms::HScrollBar^ hScrollBar_StepSize;
	private: System::Windows::Forms::Label^ label_PercentRange;
	private: System::Windows::Forms::Label^ label_StepSize;



		   Wave^ simulationWave;
		   double currentPrice;
		   double stepSize;
		   double minPrice;
		   double maxPrice;
		   bool increasing;






		   List<LineAnnotation^>^ fibLines;

		void DisplayConfirmationsFor(FibonacciAnalysis^ fibAnalysis) {
			// Clear previous confirmations
			for (int i = chart_LoadTicker2->Annotations->Count - 1; i >= 0; --i) {
				auto el = chart_LoadTicker2->Annotations[i];
				if (dynamic_cast<EllipseAnnotation^>(el) != nullptr &&
					el->Tag != nullptr && el->Tag->ToString() == "Confirm") {
					chart_LoadTicker2->Annotations->RemoveAt(i);
				}
			}

			if (!fibAnalysis) return;

			// Draw new confirmations
			for (int level = 0; level < fibAnalysis->Confirmations->Length; level++) {
				double fibValue = fibAnalysis->FibonacciValues[level];
				for each (Candlestick ^ candle in fibAnalysis->Confirmations[level]) {
					int idx = FindPointIndexByDate(candle->Date);
					if (idx < 0) continue;

					DataPoint^ dp = chart_LoadTicker2->Series["Series_OHLC2"]->Points[idx];

					EllipseAnnotation^ circle = gcnew EllipseAnnotation();
					circle->Width = 0.5;
					circle->Height = 0.5;
					circle->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
					circle->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
					circle->AnchorDataPoint = dp;
					circle->AnchorY = fibValue;
					circle->BackColor = Color::Orange;
					circle->LineColor = Color::DarkOrange;
					circle->Tag = "Confirm";
					chart_LoadTicker2->Annotations->Add(circle);
				}
			}
		}

		int FindPointIndexByDate(DateTime date) {
			for (int i = 0; i < ListOfCandlesticks->Count; i++) {
				if (ListOfCandlesticks[i]->Date.Date == date.Date) {
					return i;
				}
			}
			return -1;
		}

	private: System::Windows::Forms::Label^ label_ConfirmCount;
	public:
		Form1(void)
		{
			InitializeComponent(); // This is the function which initializes the form's components.
			stockReader = gcnew StockReader(); // This is the stock reader object which will be used to read the stock data from the file.
			peakValleyDetector = gcnew PeakValleyDetector(); // This is the peak valley detector object which will be used to detect peaks and valleys in the stock data.
			waveDetector = gcnew WaveDetector(); // This is the wave detector object which will be used to detect waves in the stock data.
			margin = 2; // Default margin value
			System::String^ initialDirectory = Path::Combine(AppDomain::CurrentDomain->BaseDirectory, "..\\..\\..\\", "Stock Data"); // This is the path to the stock data folder.
			openFileDialog_LoadTicker->InitialDirectory = Path::GetFullPath(initialDirectory); // This sets the initial directory for the open file dialog to the stock data folder.
			// Configure margin controls
			textBox_Margin->Text = "2";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button_Refresh;
	protected:
	private: System::Windows::Forms::Label^ label_UpWaves;
	private: System::Windows::Forms::CheckedListBox^ checkedListBox_DownWaves;
	private: System::Windows::Forms::CheckedListBox^ checkedListBox_UpWaves;
	private: System::Windows::Forms::TextBox^ textBox_Margin;
	private: System::Windows::Forms::Label^ label_DownWaves;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label_Margin;
	private: System::Windows::Forms::HScrollBar^ hScrollBar_Margin;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker_EndDate;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker_StartDate;
	private: System::Windows::Forms::Button^ button_LoadData;
	private: System::Windows::Forms::Label^ label_EndDate;
	private: System::Windows::Forms::Label^ label_StartDate;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart_LoadTicker2;

	private: System::Windows::Forms::OpenFileDialog^ openFileDialog_LoadTicker;
private: System::ComponentModel::IContainer^ components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea5 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea6 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^ series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->button_Refresh = (gcnew System::Windows::Forms::Button());
			this->label_UpWaves = (gcnew System::Windows::Forms::Label());
			this->checkedListBox_DownWaves = (gcnew System::Windows::Forms::CheckedListBox());
			this->checkedListBox_UpWaves = (gcnew System::Windows::Forms::CheckedListBox());
			this->textBox_Margin = (gcnew System::Windows::Forms::TextBox());
			this->label_DownWaves = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label_Margin = (gcnew System::Windows::Forms::Label());
			this->hScrollBar_Margin = (gcnew System::Windows::Forms::HScrollBar());
			this->dateTimePicker_EndDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->dateTimePicker_StartDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->button_LoadData = (gcnew System::Windows::Forms::Button());
			this->label_EndDate = (gcnew System::Windows::Forms::Label());
			this->label_StartDate = (gcnew System::Windows::Forms::Label());
			this->chart_LoadTicker2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->openFileDialog_LoadTicker = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label_ConfirmCount = (gcnew System::Windows::Forms::Label());
			this->hScrollBar_Tolerance_Scroll = (gcnew System::Windows::Forms::HScrollBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->timer_Simulation = (gcnew System::Windows::Forms::Timer(this->components));
			this->button_StartStop = (gcnew System::Windows::Forms::Button());
			this->button_Increase = (gcnew System::Windows::Forms::Button());
			this->button_Decrease = (gcnew System::Windows::Forms::Button());
			this->label_Price = (gcnew System::Windows::Forms::Label());
			this->hScrollBar_PercentRange = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBar_StepSize = (gcnew System::Windows::Forms::HScrollBar());
			this->label_PercentRange = (gcnew System::Windows::Forms::Label());
			this->label_StepSize = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart_LoadTicker2))->BeginInit();
			this->SuspendLayout();
			// 
			// button_Refresh
			// 
			this->button_Refresh->Location = System::Drawing::Point(1113, 647);
			this->button_Refresh->Margin = System::Windows::Forms::Padding(4);
			this->button_Refresh->Name = L"button_Refresh";
			this->button_Refresh->Size = System::Drawing::Size(128, 41);
			this->button_Refresh->TabIndex = 35;
			this->button_Refresh->Text = L"Refresh";
			this->button_Refresh->UseVisualStyleBackColor = true;
			this->button_Refresh->Click += gcnew System::EventHandler(this, &Form1::button_Refresh_Click);
			// 
			// label_UpWaves
			// 
			this->label_UpWaves->AutoSize = true;
			this->label_UpWaves->Location = System::Drawing::Point(447, 617);
			this->label_UpWaves->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label_UpWaves->Name = L"label_UpWaves";
			this->label_UpWaves->Size = System::Drawing::Size(78, 16);
			this->label_UpWaves->TabIndex = 34;
			this->label_UpWaves->Text = L"Up Waves -";
			// 
			// checkedListBox_DownWaves
			// 
			this->checkedListBox_DownWaves->FormattingEnabled = true;
			this->checkedListBox_DownWaves->Location = System::Drawing::Point(133, 575);
			this->checkedListBox_DownWaves->Margin = System::Windows::Forms::Padding(4);
			this->checkedListBox_DownWaves->Name = L"checkedListBox_DownWaves";
			this->checkedListBox_DownWaves->Size = System::Drawing::Size(260, 106);
			this->checkedListBox_DownWaves->TabIndex = 33;
			this->checkedListBox_DownWaves->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &Form1::checkedListBox_DownWaves_ItemCheck);
			// 
			// checkedListBox_UpWaves
			// 
			this->checkedListBox_UpWaves->FormattingEnabled = true;
			this->checkedListBox_UpWaves->Location = System::Drawing::Point(552, 572);
			this->checkedListBox_UpWaves->Margin = System::Windows::Forms::Padding(4);
			this->checkedListBox_UpWaves->Name = L"checkedListBox_UpWaves";
			this->checkedListBox_UpWaves->Size = System::Drawing::Size(252, 106);
			this->checkedListBox_UpWaves->TabIndex = 32;
			this->checkedListBox_UpWaves->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &Form1::checkedListBox_UpWaves_ItemCheck);
			// 
			// textBox_Margin
			// 
			this->textBox_Margin->Location = System::Drawing::Point(836, 657);
			this->textBox_Margin->Margin = System::Windows::Forms::Padding(4);
			this->textBox_Margin->Name = L"textBox_Margin";
			this->textBox_Margin->Size = System::Drawing::Size(132, 22);
			this->textBox_Margin->TabIndex = 31;
			this->textBox_Margin->TextChanged += gcnew System::EventHandler(this, &Form1::textBox_Margin_TextChanged);
			// 
			// label_DownWaves
			// 
			this->label_DownWaves->AutoSize = true;
			this->label_DownWaves->Location = System::Drawing::Point(21, 617);
			this->label_DownWaves->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label_DownWaves->Name = L"label_DownWaves";
			this->label_DownWaves->Size = System::Drawing::Size(94, 16);
			this->label_DownWaves->TabIndex = 30;
			this->label_DownWaves->Text = L"Down Waves -";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(377, 617);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 16);
			this->label1->TabIndex = 29;
			// 
			// label_Margin
			// 
			this->label_Margin->AutoSize = true;
			this->label_Margin->Location = System::Drawing::Point(881, 578);
			this->label_Margin->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label_Margin->Name = L"label_Margin";
			this->label_Margin->Size = System::Drawing::Size(48, 16);
			this->label_Margin->TabIndex = 28;
			this->label_Margin->Text = L"Margin";
			// 
			// hScrollBar_Margin
			// 
			this->hScrollBar_Margin->LargeChange = 1;
			this->hScrollBar_Margin->Location = System::Drawing::Point(836, 608);
			this->hScrollBar_Margin->Maximum = 4;
			this->hScrollBar_Margin->Minimum = 1;
			this->hScrollBar_Margin->Name = L"hScrollBar_Margin";
			this->hScrollBar_Margin->Size = System::Drawing::Size(136, 18);
			this->hScrollBar_Margin->TabIndex = 27;
			this->hScrollBar_Margin->Value = 2;
			this->hScrollBar_Margin->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar_Margin_Scroll);
			// 
			// dateTimePicker_EndDate
			// 
			this->dateTimePicker_EndDate->Location = System::Drawing::Point(1113, 608);
			this->dateTimePicker_EndDate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dateTimePicker_EndDate->Name = L"dateTimePicker_EndDate";
			this->dateTimePicker_EndDate->Size = System::Drawing::Size(263, 22);
			this->dateTimePicker_EndDate->TabIndex = 26;
			this->dateTimePicker_EndDate->Value = System::DateTime(2021, 2, 28, 0, 0, 0, 0);
			// 
			// dateTimePicker_StartDate
			// 
			this->dateTimePicker_StartDate->Location = System::Drawing::Point(1113, 571);
			this->dateTimePicker_StartDate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->dateTimePicker_StartDate->Name = L"dateTimePicker_StartDate";
			this->dateTimePicker_StartDate->Size = System::Drawing::Size(263, 22);
			this->dateTimePicker_StartDate->TabIndex = 25;
			this->dateTimePicker_StartDate->Value = System::DateTime(2021, 2, 1, 0, 0, 0, 0);
			// 
			// button_LoadData
			// 
			this->button_LoadData->Location = System::Drawing::Point(1271, 649);
			this->button_LoadData->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_LoadData->Name = L"button_LoadData";
			this->button_LoadData->Size = System::Drawing::Size(96, 39);
			this->button_LoadData->TabIndex = 24;
			this->button_LoadData->Text = L"Load Ticker";
			this->button_LoadData->UseVisualStyleBackColor = true;
			this->button_LoadData->Click += gcnew System::EventHandler(this, &Form1::button_LoadData_Click);
			// 
			// label_EndDate
			// 
			this->label_EndDate->AutoSize = true;
			this->label_EndDate->Location = System::Drawing::Point(1028, 613);
			this->label_EndDate->Name = L"label_EndDate";
			this->label_EndDate->Size = System::Drawing::Size(73, 16);
			this->label_EndDate->TabIndex = 23;
			this->label_EndDate->Text = L"End Date - ";
			// 
			// label_StartDate
			// 
			this->label_StartDate->AutoSize = true;
			this->label_StartDate->Location = System::Drawing::Point(1024, 575);
			this->label_StartDate->Name = L"label_StartDate";
			this->label_StartDate->Size = System::Drawing::Size(76, 16);
			this->label_StartDate->TabIndex = 22;
			this->label_StartDate->Text = L"Start Date - ";
			// 
			// chart_LoadTicker2
			// 
			chartArea5->Name = L"ChartArea_OHLC2";
			chartArea6->AlignWithChartArea = L"ChartArea_OHLC2";
			chartArea6->Name = L"ChartArea_Volume2";
			chartArea6->Visible = false;
			this->chart_LoadTicker2->ChartAreas->Add(chartArea5);
			this->chart_LoadTicker2->ChartAreas->Add(chartArea6);
			this->chart_LoadTicker2->Dock = System::Windows::Forms::DockStyle::Top;
			legend3->Enabled = false;
			legend3->Name = L"Legend1";
			this->chart_LoadTicker2->Legends->Add(legend3);
			this->chart_LoadTicker2->Location = System::Drawing::Point(0, 0);
			this->chart_LoadTicker2->Margin = System::Windows::Forms::Padding(4);
			this->chart_LoadTicker2->Name = L"chart_LoadTicker2";
			series5->ChartArea = L"ChartArea_OHLC2";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Candlestick;
			series5->CustomProperties = L"PriceDownColor=Red, PriceUpColor=Lime";
			series5->IsVisibleInLegend = false;
			series5->IsXValueIndexed = true;
			series5->Legend = L"Legend1";
			series5->Name = L"Series_OHLC2";
			series5->XValueMember = L"Date";
			series5->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;
			series5->YValueMembers = L"High, Low, Open, Close";
			series5->YValuesPerPoint = 4;
			series6->ChartArea = L"ChartArea_Volume2";
			series6->IsVisibleInLegend = false;
			series6->IsXValueIndexed = true;
			series6->Legend = L"Legend1";
			series6->Name = L"Series_Volume2";
			series6->XValueMember = L"Date";
			series6->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Date;
			series6->YValueMembers = L"Volume";
			this->chart_LoadTicker2->Series->Add(series5);
			this->chart_LoadTicker2->Series->Add(series6);
			this->chart_LoadTicker2->Size = System::Drawing::Size(1415, 544);
			this->chart_LoadTicker2->TabIndex = 36;
			this->chart_LoadTicker2->Text = L"Stock";
			this->chart_LoadTicker2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::chart_LoadTicker2_MouseDown);
			this->chart_LoadTicker2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::chart_LoadTicker2_MouseMove);
			this->chart_LoadTicker2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::chart_LoadTicker2_MouseUp);
			// 
			// openFileDialog_LoadTicker
			// 
			this->openFileDialog_LoadTicker->Multiselect = true;
			this->openFileDialog_LoadTicker->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog_LoadTicker_FileOk);
			// 
			// label_ConfirmCount
			// 
			this->label_ConfirmCount->AutoSize = true;
			this->label_ConfirmCount->Location = System::Drawing::Point(833, 704);
			this->label_ConfirmCount->Name = L"label_ConfirmCount";
			this->label_ConfirmCount->Size = System::Drawing::Size(134, 16);
			this->label_ConfirmCount->TabIndex = 37;
			this->label_ConfirmCount->Text = L"Confirmation Count : 0";
			// 
			// hScrollBar_Tolerance_Scroll
			// 
			this->hScrollBar_Tolerance_Scroll->LargeChange = 1;
			this->hScrollBar_Tolerance_Scroll->Location = System::Drawing::Point(708, 704);
			this->hScrollBar_Tolerance_Scroll->Maximum = 5;
			this->hScrollBar_Tolerance_Scroll->Name = L"hScrollBar_Tolerance_Scroll";
			this->hScrollBar_Tolerance_Scroll->Size = System::Drawing::Size(80, 21);
			this->hScrollBar_Tolerance_Scroll->TabIndex = 28;
			this->hScrollBar_Tolerance_Scroll->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar_Tolerance_Scroll_Scroll);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(650, 682);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(154, 16);
			this->label2->TabIndex = 39;
			this->label2->Text = L"Confirmation Percentage";
			// 
			// timer_Simulation
			// 
			this->timer_Simulation->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// button_StartStop
			// 
			this->button_StartStop->Location = System::Drawing::Point(431, 675);
			this->button_StartStop->Name = L"button_StartStop";
			this->button_StartStop->Size = System::Drawing::Size(75, 23);
			this->button_StartStop->TabIndex = 40;
			this->button_StartStop->Text = L"Start";
			this->button_StartStop->UseVisualStyleBackColor = true;
			this->button_StartStop->Click += gcnew System::EventHandler(this, &Form1::button_StartStop_Click);
			// 
			// button_Increase
			// 
			this->button_Increase->Location = System::Drawing::Point(380, 704);
			this->button_Increase->Name = L"button_Increase";
			this->button_Increase->Size = System::Drawing::Size(75, 23);
			this->button_Increase->TabIndex = 41;
			this->button_Increase->Text = L"+";
			this->button_Increase->UseVisualStyleBackColor = true;
			this->button_Increase->Click += gcnew System::EventHandler(this, &Form1::button_Increase_Click);
			// 
			// button_Decrease
			// 
			this->button_Decrease->Location = System::Drawing::Point(497, 704);
			this->button_Decrease->Name = L"button_Decrease";
			this->button_Decrease->Size = System::Drawing::Size(75, 23);
			this->button_Decrease->TabIndex = 42;
			this->button_Decrease->Text = L"-";
			this->button_Decrease->UseVisualStyleBackColor = true;
			this->button_Decrease->Click += gcnew System::EventHandler(this, &Form1::button_Decrease_Click);
			// 
			// label_Price
			// 
			this->label_Price->AutoSize = true;
			this->label_Price->Location = System::Drawing::Point(833, 736);
			this->label_Price->Name = L"label_Price";
			this->label_Price->Size = System::Drawing::Size(45, 16);
			this->label_Price->TabIndex = 43;
			this->label_Price->Text = L"Price -";
			this->label_Price->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// hScrollBar_PercentRange
			// 
			this->hScrollBar_PercentRange->LargeChange = 5;
			this->hScrollBar_PercentRange->Location = System::Drawing::Point(133, 706);
			this->hScrollBar_PercentRange->Minimum = 1;
			this->hScrollBar_PercentRange->Name = L"hScrollBar_PercentRange";
			this->hScrollBar_PercentRange->Size = System::Drawing::Size(80, 21);
			this->hScrollBar_PercentRange->TabIndex = 100;
			this->hScrollBar_PercentRange->Value = 20;
			this->hScrollBar_PercentRange->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar_PercentRange_Scroll);
			// 
			// hScrollBar_StepSize
			// 
			this->hScrollBar_StepSize->LargeChange = 5;
			this->hScrollBar_StepSize->Location = System::Drawing::Point(133, 736);
			this->hScrollBar_StepSize->Minimum = 1;
			this->hScrollBar_StepSize->Name = L"hScrollBar_StepSize";
			this->hScrollBar_StepSize->Size = System::Drawing::Size(80, 21);
			this->hScrollBar_StepSize->TabIndex = 100;
			this->hScrollBar_StepSize->Value = 20;
			this->hScrollBar_StepSize->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar_StepSize_Scroll);
			// 
			// label_PercentRange
			// 
			this->label_PercentRange->AutoSize = true;
			this->label_PercentRange->Location = System::Drawing::Point(33, 706);
			this->label_PercentRange->Name = L"label_PercentRange";
			this->label_PercentRange->Size = System::Drawing::Size(95, 16);
			this->label_PercentRange->TabIndex = 46;
			this->label_PercentRange->Text = L"Range (20%) - ";
			// 
			// label_StepSize
			// 
			this->label_StepSize->AutoSize = true;
			this->label_StepSize->Location = System::Drawing::Point(33, 741);
			this->label_StepSize->Name = L"label_StepSize";
			this->label_StepSize->Size = System::Drawing::Size(99, 16);
			this->label_StepSize->TabIndex = 47;
			this->label_StepSize->Text = L"Step Size (20) - ";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1415, 795);
			this->Controls->Add(this->label_StepSize);
			this->Controls->Add(this->label_PercentRange);
			this->Controls->Add(this->hScrollBar_StepSize);
			this->Controls->Add(this->hScrollBar_PercentRange);
			this->Controls->Add(this->label_Price);
			this->Controls->Add(this->button_Decrease);
			this->Controls->Add(this->button_Increase);
			this->Controls->Add(this->button_StartStop);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->hScrollBar_Tolerance_Scroll);
			this->Controls->Add(this->label_ConfirmCount);
			this->Controls->Add(this->chart_LoadTicker2);
			this->Controls->Add(this->button_Refresh);
			this->Controls->Add(this->label_UpWaves);
			this->Controls->Add(this->checkedListBox_DownWaves);
			this->Controls->Add(this->checkedListBox_UpWaves);
			this->Controls->Add(this->textBox_Margin);
			this->Controls->Add(this->label_DownWaves);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label_Margin);
			this->Controls->Add(this->hScrollBar_Margin);
			this->Controls->Add(this->dateTimePicker_EndDate);
			this->Controls->Add(this->dateTimePicker_StartDate);
			this->Controls->Add(this->button_LoadData);
			this->Controls->Add(this->label_EndDate);
			this->Controls->Add(this->label_StartDate);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart_LoadTicker2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	/// <summary>
	/// Loads ticker data when the Load Ticker button is clicked
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void button_LoadData_Click(System::Object^ sender, System::EventArgs^ e) {
		// Set the initial directory if needed
		System::String^ initialDirectory = Path::Combine(AppDomain::CurrentDomain->BaseDirectory, "..\\..\\..\\", "Stock Data");
		openFileDialog_LoadTicker->InitialDirectory = Path::GetFullPath(initialDirectory);

		// Pre-select the file
		openFileDialog_LoadTicker->FileName = "ABBV-Day.csv";

		// Show the dialog
		openFileDialog_LoadTicker->ShowDialog();
	}
	public:
		List<Candlestick^>^ FilterCandlesticksByDate(
			List<Candlestick^>^ candlesticks,
			DateTime startDate,
			DateTime endDate) // This function filters the candlesticks based on the date range selected by the user.
		{
			List<Candlestick^>^ result = gcnew List<Candlestick^>(); // This is the list of candlesticks which will be returned after filtering.
			for each (Candlestick ^ c in candlesticks) // This is a loop which iterates over all the candlesticks in the list.
			{
				if (c->Date >= startDate && c->Date <= endDate) // This checks if the date of the candlestick is within the date range selected by the user.
				{
					result->Add(c); // If the date is within the range, the candlestick is added to the result list.
				}
			}
			return result; // This returns the list of candlesticks which have been filtered based on the date range selected by the user.
		}
		/// <summary>
		/// Normalizes the chart's Y-axis based on the candlestick data with padding
		/// </summary>
		/// <param name="candlesticks">List of candlesticks to analyze for min/max values</param>
		/// <returns></returns>
		System::Void normalizeChart(List<Candlestick^>^ candlesticks)
		{
			// Find max and min values
			System::Decimal maxValue = System::Decimal::MinValue; // This gets the maximum value of the high property from the list of candlesticks.
			System::Decimal minValue = System::Decimal::MaxValue; // This gets the minimum value of the low property from the list of candlesticks.

			for each (Candlestick ^ c in candlesticks) // This is a loop which iterates over all the candlesticks in the list.
			{
				if (c->High > maxValue) maxValue = c->High; // This checks if the high value of the candlestick is greater than the maxValue. If yes, it sets the maxValue to the high value of the candlestick.
				if (c->Low < minValue) minValue = c->Low; // This checks if the low value of the candlestick is less than the minValue. If yes, it sets the minValue to the low value of the candlestick.
			}

			System::Decimal padding = System::Decimal::Subtract(maxValue, minValue) * System::Decimal(0.02); // adds 2% padding according to the project's additional specifications.
			System::Decimal minY = System::Decimal::Subtract(minValue, padding); // This is the minimum value for the Y-axis of the chart after padding.
			System::Decimal maxY = System::Decimal::Add(maxValue, padding); // This is the maximum value for the Y-axis of the chart after padding.
			// This sets the chart's limits based on the calculated values.
			chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY->Maximum = System::Convert::ToDouble(maxY); // This sets the maximum value for the Y-axis of the chart.
			chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY->Minimum = System::Convert::ToDouble(minY); // This sets the minimum value for the Y-axis of the chart.
			chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY->LabelStyle->Format = "F4"; // This sets the format for the Y-axis labels.
		}
		/// <summary>
		/// Processes the selected stock files and displays the data
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		/// <returns></returns>
		System::Void openFileDialog_LoadTicker_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
			array<String^>^ files = openFileDialog_LoadTicker->FileNames; // This gets the names of the files selected by the user.
			if (files->Length == 0) // This checks if no files have been selected by the user.
				return;

			// --- Load the first stock into the main (current) form ---
			String^ firstFile = files[0];
			this->Text = firstFile; // Set the form's title to the first file name.

			// Read candlesticks from the first file.
			ListOfCandlesticks = stockReader->ReadCandlestickFromFile(firstFile);

			// Ensure the candlesticks are in chronological order.
			if (ListOfCandlesticks->Count >= 2) {
				if (ListOfCandlesticks[0]->Date > ListOfCandlesticks[1]->Date)
					ListOfCandlesticks->Reverse();
			}

			// Filter the candlesticks based on the selected date range.
			ListOfCandlesticks = FilterCandlesticksByDate(ListOfCandlesticks,
				dateTimePicker_StartDate->Value, dateTimePicker_EndDate->Value);

			// Display the stock data
			DisplayStock(ListOfCandlesticks);

			// --- For each additional stock, create a new form instance ---
			for (int i = 1; i < files->Length; i++) {
				String^ file = files[i];
				Form1^ newForm = gcnew Form1();
				newForm->Text = file; // Set the new form's title to the file name.

				// Read and process the candlestick data for the new form.
				newForm->ListOfCandlesticks = stockReader->ReadCandlestickFromFile(file);
				if (newForm->ListOfCandlesticks->Count >= 2) {
					if (newForm->ListOfCandlesticks[0]->Date > newForm->ListOfCandlesticks[1]->Date)
						newForm->ListOfCandlesticks->Reverse();
				}
				newForm->ListOfCandlesticks = newForm->FilterCandlesticksByDate(newForm->ListOfCandlesticks,
					dateTimePicker_StartDate->Value, dateTimePicker_EndDate->Value); // Filter based on selected date range

				newForm->DisplayStock(newForm->ListOfCandlesticks); 

				// Display the new form (modeless).
				newForm->Show();
			}
		}
		   /// <summary>
		   /// Displays stock data on the chart and detects patterns
		   /// </summary>
		   /// <param name="candlesticks">List of candlesticks to display</param>
		   /// <returns></returns>
		   System::Void DisplayStock(List<Candlestick^>^ candlesticks)
		   {
			   // Normalize chart
			   normalizeChart(candlesticks);

			   // Bind data to chart
			   BindingList<Candlestick^>^ bindingList = gcnew BindingList<Candlestick^>(candlesticks);
			   chart_LoadTicker2->DataSource = bindingList;
			   chart_LoadTicker2->DataBind();
			   chart_LoadTicker2->Legends[0]->Enabled = false;

			   // Clear existing annotations
			   chart_LoadTicker2->Annotations->Clear();

			   // Clear wave list boxes
			   checkedListBox_UpWaves->Items->Clear();
			   checkedListBox_DownWaves->Items->Clear();

			   // Detect peaks, valleys, and waves
			   DetectPeaksAndValleys();
		   }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void button_Refresh_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ filePath = this->Text; // Get the file path from the form's title.

		if (!String::IsNullOrEmpty(filePath)) { // Check if the file path is not empty.
			// Re-read data from file
			ListOfCandlesticks = stockReader->ReadCandlestickFromFile(filePath);

			// Ensure chronological order
			if (ListOfCandlesticks->Count >= 2) {
				if (ListOfCandlesticks[0]->Date > ListOfCandlesticks[1]->Date) {
					ListOfCandlesticks->Reverse();
				}
			}

			// Filter based on selected date range
			ListOfCandlesticks = FilterCandlesticksByDate(ListOfCandlesticks,
				dateTimePicker_StartDate->Value, dateTimePicker_EndDate->Value);

			// Update chart display
			DisplayStock(ListOfCandlesticks);
		}
		else { // If the file path is empty, show a message box to the user.
			MessageBox::Show("Please load a stock file first.", "No Data",
				MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
	}
		   /// <summary>
		   /// Detects peaks and valleys in the loaded candlestick data
		   /// </summary>
		   /// <returns></returns>
		   System::Void DetectPeaksAndValleys()
		   {
			   if (ListOfCandlesticks == nullptr || ListOfCandlesticks->Count == 0) // Check if the list of candlesticks is null or empty.
				   return;

			   // Clear existing annotations
			   ClearAllAnnotations();

			   // Detect peaks and valleys
			   points = peakValleyDetector->DetectPeaksAndValleys(ListOfCandlesticks, margin);

			   // Draw annotations
			   DrawPeaksAndValleys();

			   // Detect and display waves
			   DetectWaves();
		   }
		   /// <summary>
		   /// Clears all annotations from the chart
		   /// </summary>
		   /// <returns></returns>
		   System::Void ClearAllAnnotations()
		   {
			   chart_LoadTicker2->Annotations->Clear(); // This clears all the annotations from the chart.
		   }
		   /// <summary>
		   /// Selectively clears peak and valley annotations from the chart
		   /// </summary>
		   /// <returns></returns>
		   System::Void ClearPeakValleyAnnotations()
		   {
			   List<Annotation^>^ annotationsToRemove = gcnew List<Annotation^>(); // This is the list of annotations which will be removed.

			   for each (Annotation ^ annotation in chart_LoadTicker2->Annotations) // This is a loop which iterates over all the annotations in the chart.
			   {
				   if ((dynamic_cast<LineAnnotation^>(annotation) != nullptr &&
					   (dynamic_cast<LineAnnotation^>(annotation))->Width == 0) ||
					   (dynamic_cast<TextAnnotation^>(annotation) != nullptr &&
						   (dynamic_cast<TextAnnotation^>(annotation)->Text == "P" ||
							   dynamic_cast<TextAnnotation^>(annotation)->Text == "V"))) // This checks if the annotation is a peak or valley annotation.
				   {
					   annotationsToRemove->Add(annotation); // If the annotation is a peak or valley annotation, it is added to the list of annotations to be removed.
				   }
			   }

			   for each (Annotation ^ annotation in annotationsToRemove) // This is a loop which iterates over all the annotations to be removed.
			   {
				   chart_LoadTicker2->Annotations->Remove(annotation); // This removes the annotation from the chart.
			   }
		   }
		   /// <summary>
		   /// Clears wave visualizations (rectangles and diagonal lines) from the chart
		   /// </summary>
		   /// <returns></returns>
		   System::Void ClearWaveVisualizations() {
			   List<Annotation^>^ annotationsToRemove = gcnew List<Annotation^>();// This is the list of annotations which will be removed.

			   for each (Annotation ^ annotation in chart_LoadTicker2->Annotations) { // This is a loop which iterates over all the annotations in the chart.
				   if (dynamic_cast<RectangleAnnotation^>(annotation) != nullptr) { // This checks if the annotation is a rectangle annotation.
					   annotationsToRemove->Add(annotation);
				   }
				   else if (dynamic_cast<LineAnnotation^>(annotation) != nullptr) { // This checks if the annotation is a line annotation.
					   LineAnnotation^ lineAnnotation = dynamic_cast<LineAnnotation^>(annotation); // This casts the annotation to a line annotation.
					   bool isDiagonalLine = (lineAnnotation->Width != 0) || (lineAnnotation->AnchorDataPoint == nullptr); // This checks if the line is a diagonal line.
					   bool hasWaveColor = (lineAnnotation->LineColor == Color::Green || lineAnnotation->LineColor == Color::Red); // This checks if the line has a wave color.

					   if (isDiagonalLine && hasWaveColor) { // This checks if the line is a diagonal line and has a wave color.
						   annotationsToRemove->Add(annotation);
					   }
				   }
			   }
			   // Remove the annotations
			   for each (Annotation ^ annotation in annotationsToRemove) {
				   chart_LoadTicker2->Annotations->Remove(annotation);
			   }
		   }
		   /// <summary>
		   /// Finds the index of a candlestick with the specified date
		   /// </summary>
		   /// <param name="date">The date to search for</param>
		   /// <returns>Index of the matching candlestick or -1 if not found</returns>
		   /*/int FindPointIndexByDate(DateTime date)
		   {
			   for (int i = 0; i < ListOfCandlesticks->Count; i++) // This is a loop which iterates over all the candlesticks in the list.
			   {
				   if (ListOfCandlesticks[i]->Date.Date == date.Date) // This checks if the date of the candlestick is equal to the date passed as a parameter.
					   return i; // If the date is found, the index of the candlestick is returned.
			   }
			   return -1; // If the date is not found, -1 is returned.
		   }
		   */
		   /// <summary>
		   /// Draws annotations for peaks and valleys on the chart
		   /// </summary>
		   /// <returns></returns>
		   System::Void DrawPeaksAndValleys()
		   {
			   // Clear existing peak/valley annotations
			   ClearPeakValleyAnnotations();
			   ClearWaveVisualizations();

			   for each (PeakValleyDetector::Point ^ point in points) // This is a loop which iterates over all the points in the list.
			   {
				   // Find the index of the point in the chart
				   int xValue = FindPointIndexByDate(point->Candlestick->Date);
				   if (xValue == -1) continue; // Skip if not found

				   // Create line annotation
				   LineAnnotation^ annotation = gcnew LineAnnotation();
				   annotation->LineColor = point->Type == PeakValleyDetector::PointType::Peak ? Color::Red : Color::Green;
				   annotation->LineWidth = 2;
				   annotation->Width = 0; // Make it vertical
				   annotation->SmartLabelStyle->Enabled = false;

				   // Create text annotation
				   TextAnnotation^ textAnnotation = gcnew TextAnnotation();
				   textAnnotation->Text = point->Type == PeakValleyDetector::PointType::Peak ? "P" : "V";
				   textAnnotation->ForeColor = point->Type == PeakValleyDetector::PointType::Peak ? Color::Red : Color::Green;
				   textAnnotation->Font = gcnew System::Drawing::Font("Arial", 8);
				   textAnnotation->SmartLabelStyle->Enabled = false;
				   // Set common properties
				   annotation->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
				   annotation->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
				   textAnnotation->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
				   textAnnotation->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
				   annotation->ClipToChartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->Name;
				   textAnnotation->ClipToChartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->Name;

				   // Get data point
				   DataPoint^ dataPoint = chart_LoadTicker2->Series["Series_OHLC2"]->Points[xValue];

				   // Anchor to data point
				   annotation->AnchorDataPoint = dataPoint;
				   textAnnotation->AnchorDataPoint = dataPoint;

				   // Set appropriate height and y value with fixed offsets
				   if (point->Type == PeakValleyDetector::PointType::Valley)
				   {
					   annotation->Height = -2; // Fixed height for valley annotation
					   annotation->AnchorY = Convert::ToDouble(point->Candlestick->Low);
					   textAnnotation->AnchorY = Convert::ToDouble(point->Candlestick->Low);
					   textAnnotation->AnchorOffsetY = -3; // Fixed offset for valley text
				   }
				   else if (point->Type == PeakValleyDetector::PointType::Peak)
				   {
					   annotation->Height = 2; // Fixed height for peak annotation
					   annotation->AnchorY = Convert::ToDouble(point->Candlestick->High);
					   textAnnotation->AnchorY = Convert::ToDouble(point->Candlestick->High);
					   textAnnotation->AnchorOffsetY = 0; // Fixed offset for peak text
				   }

				   // Add the annotations to the chart
				   chart_LoadTicker2->Annotations->Add(annotation);
				   chart_LoadTicker2->Annotations->Add(textAnnotation);
			   }
		   }
		   /// <summary>
		   /// Detects waves based on peaks and valleys and updates the wave list boxes
		   /// </summary>
		   /// <returns></returns>
		   System::Void DetectWaves() 
		   {
			   if (points == nullptr || points->Count < 2) // Check if the list of points is null or has less than 2 points.
				   return;

			   // Detect waves
			   List<Wave^>^ waves = waveDetector->DetectWaves(ListOfCandlesticks, points);

			   // Split into up and down waves
			   upWaves = gcnew List<Wave^>();
			   downWaves = gcnew List<Wave^>();

			   for each (Wave ^ wave in waves) 
			   {
				   if (wave->IsUpWave) 
					   upWaves->Add(wave);
				   else
					   downWaves->Add(wave);
			   }

			   // Update checked list boxes
			   UpdateWaveListBoxes();
		   }
		   /// <summary>
		   /// Updates the up waves and down waves checklist boxes with detected waves
		   /// </summary>
		   /// <returns></returns>
		   System::Void UpdateWaveListBoxes()
		   {
			   // Clear existing items
			   checkedListBox_UpWaves->Items->Clear();
			   checkedListBox_DownWaves->Items->Clear();

			   // Add up waves
			   for each (Wave ^ wave in upWaves)
			   {
				   checkedListBox_UpWaves->Items->Add(wave, false);
			   }

			   // Add down waves
			   for each (Wave ^ wave in downWaves)
			   {
				   checkedListBox_DownWaves->Items->Add(wave, false);
			   }
		   }
		   /// <summary>
		   /// Draws a visual representation of a wave on the chart
		   /// </summary>
		   /// <param name="wave">The wave to visualize</param>
		   /// <returns></returns>
		   System::Void DrawWave(Wave^ wave)
		   {

			   if (wave == nullptr)
				   return;

			   // CRITICAL CHANGE: Use stored indices directly instead of looking them up by date
			   int startIndex = wave->StartIndex + 1;  // +1 for chart positioning
			   int endIndex = wave->EndIndex + 1;      // +1 for chart positioning

			   // Create rectangle annotation
			   RectangleAnnotation^ rectangle = gcnew RectangleAnnotation();
			   rectangle->BackColor = wave->IsUpWave ?
				   Color::FromArgb(50, Color::Green) : Color::FromArgb(50, Color::Red);
			   rectangle->LineColor = wave->IsUpWave ? Color::Green : Color::Red;
			   rectangle->LineWidth = 1;
			   rectangle->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
			   rectangle->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
			   rectangle->ClipToChartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->Name;
			   rectangle->IsSizeAlwaysRelative = false;

			   // Set X position and width based on indices
			   rectangle->X = startIndex;
			   rectangle->Width = endIndex - startIndex;

			   // Set Y position and height based on wave values
			   double startVal = Convert::ToDouble(wave->StartValue);
			   double endVal = Convert::ToDouble(wave->EndValue);
			   rectangle->Y = Math::Min(startVal, endVal);
			   rectangle->Height = Math::Abs(endVal - startVal);

			   // Add rectangle to the chart
			   chart_LoadTicker2->Annotations->Add(rectangle);

			   // Create diagonal line annotation - rest of code remains the same
			   LineAnnotation^ diagonalLine = gcnew LineAnnotation();
			   diagonalLine->LineColor = wave->IsUpWave ? Color::Green : Color::Red;
			   diagonalLine->LineWidth = 2;
			   diagonalLine->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
			   diagonalLine->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
			   diagonalLine->ClipToChartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->Name;
			   diagonalLine->IsSizeAlwaysRelative = false;

			   // Set the diagonal line points
			   diagonalLine->X = startIndex;
			   diagonalLine->Y = startVal;
			   diagonalLine->Width = endIndex - startIndex;
			   diagonalLine->Height = endVal - startVal;

			   chart_LoadTicker2->Annotations->Add(diagonalLine);

			   // Create temporary points for Fibonacci analysis
			   PeakValleyDetector::Point^ startPoint = gcnew PeakValleyDetector::Point();
			   startPoint->Index = wave->StartIndex;
			   startPoint->Type = wave->IsUpWave ?
				   PeakValleyDetector::PointType::Valley : PeakValleyDetector::PointType::Peak;
			   startPoint->Candlestick = ListOfCandlesticks[wave->StartIndex];

			   PeakValleyDetector::Point^ endPoint = gcnew PeakValleyDetector::Point();
			   endPoint->Index = wave->EndIndex;
			   endPoint->Type = wave->IsUpWave ?
				   PeakValleyDetector::PointType::Peak : PeakValleyDetector::PointType::Valley;
			   endPoint->Candlestick = ListOfCandlesticks[wave->EndIndex];

			   // Create Fibonacci analysis for this wave
			   FibonacciAnalysis^ fibAnalysis = gcnew FibonacciAnalysis(startPoint, endPoint);
			   fibAnalysis->CalculateFibonacciValues();

			   // Draw Fibonacci levels
			   for each (double fibValue in fibAnalysis->FibonacciValues) {
				   LineAnnotation^ fibLine = gcnew LineAnnotation();
				   fibLine->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
				   fibLine->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
				   fibLine->ClipToChartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->Name;
				   fibLine->IsSizeAlwaysRelative = false;
				   fibLine->X = Math::Min(startIndex, endIndex);
				   fibLine->Y = fibValue;
				   fibLine->Width = Math::Abs(endIndex - startIndex) + 1;
				   fibLine->Height = 0;
				   fibLine->LineColor = Color::Purple;
				   fibLine->LineDashStyle = ChartDashStyle::Dash;
				   fibLine->LineWidth = 1;
				   chart_LoadTicker2->Annotations->Add(fibLine);
			   }

			   // Detect and display confirmations
			   fibAnalysis->DetectConfirmations(ListOfCandlesticks, tolerance);
			   DisplayConfirmationsFor(fibAnalysis);

			   // Update confirmation count
			   label_ConfirmCount->Text = "Confirmations: " + fibAnalysis->GetTotalConfirmationCount().ToString();
		   }

		   /// <summary>
		   /// Draws all selected waves from both checked list boxes
		   /// </summary>
		   System::Void DrawSelectedWaves() {
			   // Clear existing wave visualizations
			   ClearWaveVisualizations();

			   // Draw checked up waves
			   for (int i = 0; i < checkedListBox_UpWaves->CheckedItems->Count; i++) {
				   DrawWave(safe_cast<Wave^>(checkedListBox_UpWaves->CheckedItems[i]));
			   }

			   // Draw checked down waves
			   for (int i = 0; i < checkedListBox_DownWaves->CheckedItems->Count; i++) {
				   DrawWave(safe_cast<Wave^>(checkedListBox_DownWaves->CheckedItems[i]));
			   }
		   }
	/// <summary>
	/// Handles item check events for the down waves checklist box
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void checkedListBox_DownWaves_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e) {
		this->BeginInvoke(gcnew Action(this, &Form1::DrawSelectedWaves));
	}
	/// <summary>
	/// Handles item check events for the up waves checklist box
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	private: System::Void checkedListBox_UpWaves_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e) {
		this->BeginInvoke(gcnew Action(this, &Form1::DrawSelectedWaves));
	}
/// <summary>
/// Updates the margin value when the scroll bar is moved
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <returns></returns>
private: System::Void hScrollBar_Margin_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
	// Update the margin value
	margin = hScrollBar_Margin->Value;

	// Update the textbox display
	textBox_Margin->Text = margin.ToString();

	// Force UI update
	Application::DoEvents();

	// Re-detect peaks and valleys with new margin
	DetectPeaksAndValleys();
}
/// <summary>
/// Updates the margin value when the text box value changes
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
/// <returns></returns>
private: System::Void textBox_Margin_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	int newMargin; // This is the new margin value entered by the user.
	if (Int32::TryParse(textBox_Margin->Text, newMargin) && newMargin >= 1 && newMargin <= 4) { // This checks if the new margin value is a valid integer between 1 and 4.
		margin = newMargin; // This sets the margin value to the new margin value.
		hScrollBar_Margin->Value = newMargin; // This sets the value of the scroll bar to the new margin value.
		DetectPeaksAndValleys(); // This re-detects the peaks and valleys with the new margin value.
	}
}
private: System::Void chart_LoadTicker2_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

	ChartArea^ chartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"];

	// Convert mouse position to X-axis value
	double xVal = chartArea->AxisX->PixelPositionToValue(e->X);
	int idx = (int)Math::Round(xVal);

	// Find nearest peak/valley within 2 indexes
	PeakValleyDetector::Point^ nearest = nullptr;
	int minDistance = Int32::MaxValue;
	for each (PeakValleyDetector::Point ^ p in points) {
		int distance = Math::Abs(p->Index - idx);
		if (distance <= 2 && distance < minDistance) {
			nearest = p;
			minDistance = distance;
		}
	}

	if (nearest == nullptr) {
		MessageBox::Show("Start drag on a peak (P) or valley (V)",
			"Invalid Start",
			MessageBoxButtons::OK,
			MessageBoxIcon::Warning);
		return;
	}

	isRubberBanding = true;
	rubberBandStartPoint = nearest;
	rubberBandEndIndex = nearest->Index;

	// Initialize Fibonacci analysis
	currentFibAnalysis = gcnew FibonacciAnalysis(nearest, nearest);
}
private: System::Void chart_LoadTicker2_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (!isRubberBanding || currentFibAnalysis == nullptr) return;

	ChartArea^ chartArea = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"];

	// Convert mouse position to chart values
	double xVal = chartArea->AxisX->PixelPositionToValue(e->X);
	double yVal = chartArea->AxisY->PixelPositionToValue(e->Y);

	// Get current candlestick
	int idx = Math::Max(0, Math::Min((int)Math::Round(xVal), ListOfCandlesticks->Count - 1));

	rubberBandEndIndex = idx;


	Candlestick^ currentCandle = ListOfCandlesticks[idx];

	// Determine closest value (high or low) - ADDED THIS
	double high = Convert::ToDouble(currentCandle->High);
	double low = Convert::ToDouble(currentCandle->Low);
	double closestValue = Math::Abs(yVal - high) < Math::Abs(yVal - low) ? high : low;

	// Update end point
	PeakValleyDetector::Point^ endPoint = gcnew PeakValleyDetector::Point();
	endPoint->Index = idx;
	endPoint->Candlestick = currentCandle;
	endPoint->Type = (yVal > currentFibAnalysis->StartValue) ?
		PeakValleyDetector::PointType::Peak :
		PeakValleyDetector::PointType::Valley;

	// Update Fibonacci analysis
	currentFibAnalysis->EndPoint = endPoint;
	currentFibAnalysis->EndDate = endPoint->Candlestick->Date;
	currentFibAnalysis->EndValue = closestValue; // Use closest value instead of Close
	currentFibAnalysis->IsUpWave = currentFibAnalysis->StartValue < currentFibAnalysis->EndValue;
	currentFibAnalysis->CalculateFibonacciValues();

	// Clear previous annotations
	ClearRubberBandAnnotations(); // Use the new cleanup method

	// Draw rubber band rectangle
	RectangleAnnotation^ rect = gcnew RectangleAnnotation();
	rect->AxisX = chartArea->AxisX;
	rect->AxisY = chartArea->AxisY;
	rect->BackColor = Color::FromArgb(50, Color::Blue); // Transparent blue
	rect->LineColor = Color::Blue;
	rect->LineWidth = 1;
	rect->IsSizeAlwaysRelative = false;
	rect->X = Math::Min(rubberBandStartPoint->Index, idx);
	rect->Width = Math::Abs(idx - rubberBandStartPoint->Index);
	rect->Y = Math::Min(currentFibAnalysis->StartValue, currentFibAnalysis->EndValue);
	rect->Height = Math::Abs(currentFibAnalysis->EndValue - currentFibAnalysis->StartValue);
	chart_LoadTicker2->Annotations->Add(rect);

	// Draw diagonal line - FIXED COLOR HERE
	LineAnnotation^ diagonal = gcnew LineAnnotation();
	diagonal->AxisX = chartArea->AxisX;
	diagonal->AxisY = chartArea->AxisY;
	diagonal->LineColor = Color::Blue; // Use Blue for rubber band diagonal line
	diagonal->LineWidth = 2;
	diagonal->IsSizeAlwaysRelative = false;
	diagonal->X = rubberBandStartPoint->Index;
	diagonal->Y = currentFibAnalysis->StartValue;
	diagonal->Width = idx - rubberBandStartPoint->Index;
	diagonal->Height = currentFibAnalysis->EndValue - currentFibAnalysis->StartValue;
	chart_LoadTicker2->Annotations->Add(diagonal);

	// Draw Fibonacci lines
	for each (double fibValue in currentFibAnalysis->FibonacciValues) {
		LineAnnotation^ line = gcnew LineAnnotation();
		line->AxisX = chartArea->AxisX;
		line->AxisY = chartArea->AxisY;
		line->LineColor = Color::Purple; // Fib lines remain Purple
		line->LineDashStyle = ChartDashStyle::Dash;
		line->LineWidth = 1;
		line->IsSizeAlwaysRelative = false;
		line->X = Math::Min(rubberBandStartPoint->Index, idx);
		line->Y = fibValue;
		line->Width = Math::Abs(idx - rubberBandStartPoint->Index);
		line->Height = 0;
		chart_LoadTicker2->Annotations->Add(line);
	}

	// Also update to detect confirmations
	currentFibAnalysis->DetectConfirmations(ListOfCandlesticks, tolerance);
	DisplayConfirmationsFor(currentFibAnalysis);

}
private: System::Void chart_LoadTicker2_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (!isRubberBanding) return;

	// Validate wave direction
	bool isValid = false;
	if (rubberBandStartPoint->Type == PeakValleyDetector::PointType::Valley &&
		currentFibAnalysis->EndValue > currentFibAnalysis->StartValue) {
		isValid = true;
	}
	else if (rubberBandStartPoint->Type == PeakValleyDetector::PointType::Peak &&
		currentFibAnalysis->EndValue < currentFibAnalysis->StartValue) {
		isValid = true;
	}

	if (!isValid) {
		MessageBox::Show("Invalid wave direction!\n"
			"Valley must go up\n"
			"Peak must go down",
			"Invalid Wave",
			MessageBoxButtons::OK,
			MessageBoxIcon::Warning);

		// Clean up rubber band annotations like in C# version
		ClearRubberBandAnnotations();
		// Reset confirmation label
		label_ConfirmCount->Text = "Confirmations: 0";
	}
	else {
		// Add to appropriate list
		Wave^ newWave = gcnew Wave();

		// CRITICAL: Calculate corrected end index like in C# version
		int correctedEndIndex = Math::Max(0, Math::Min(rubberBandEndIndex - 1, ListOfCandlesticks->Count - 1));

		// Store both the index and date information
		newWave->StartIndex = rubberBandStartPoint->Index;
		newWave->EndIndex = correctedEndIndex; // Use corrected index
		newWave->StartDate = rubberBandStartPoint->Candlestick->Date;
		newWave->EndDate = ListOfCandlesticks[correctedEndIndex]->Date;
		newWave->StartValue = currentFibAnalysis->StartValue;
		newWave->EndValue = currentFibAnalysis->EndValue;
		newWave->IsUpWave = (rubberBandStartPoint->Type == PeakValleyDetector::PointType::Valley);

		// Check for duplicates (like in C# version)
		bool isDuplicate = false;
		List<Wave^>^ list = newWave->IsUpWave ? upWaves : downWaves;

		for each (Wave ^ existingWave in list) {
			if (existingWave->StartIndex == newWave->StartIndex &&
				existingWave->EndIndex == newWave->EndIndex) {
				isDuplicate = true;
				break;
			}
		}

		if (isDuplicate) {
			MessageBox::Show("That exact wave already exists. Won't add it again.",
				"Duplicate Wave",
				MessageBoxButtons::OK,
				MessageBoxIcon::Information);
		}
		else {
			// Add to the appropriate list
			if (newWave->IsUpWave) {
				upWaves->Add(newWave);
				checkedListBox_UpWaves->Items->Add(newWave, true); // Check it by default
			}
			else {
				downWaves->Add(newWave);
				checkedListBox_DownWaves->Items->Add(newWave, true); // Check it by default
			}


			// CRITICAL: Immediately redraw the waves to show it properly
			ClearRubberBandAnnotations();
			DrawSelectedWaves();
		}
	}

	// Cleanup
	isRubberBanding = false;
	currentFibAnalysis = nullptr;

}
	   void ClearRubberBandAnnotations() {
		   List<Annotation^>^ annotationsToRemove = gcnew List<Annotation^>();

		   for each (Annotation ^ annotation in chart_LoadTicker2->Annotations) {
			   if ((dynamic_cast<RectangleAnnotation^>(annotation) != nullptr &&
				   annotation->LineColor == Color::Blue) ||
				   (dynamic_cast<LineAnnotation^>(annotation) != nullptr &&
					   (annotation->LineColor == Color::Purple ||
						   annotation->LineColor == Color::Blue))) {
				   annotationsToRemove->Add(annotation);
			   }
		   }

		   for each (Annotation ^ annotation in annotationsToRemove) {
			   chart_LoadTicker2->Annotations->Remove(annotation);
		   }
	   }
private: System::Void hScrollBar_Tolerance_Scroll_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {

	// Convert scroll value to tolerance percentage (1-5 → 0.1% to 0.5%)
	tolerance = hScrollBar_Tolerance_Scroll->Value / 1000.0;

	// Update the label to show percentage
	label2->Text = String::Format("Tolerance ({0:F2}%)", tolerance * 100);

	// Redraw with new tolerance
	RedrawWithNewTolerance();
}
	   private: System::Void RedrawWithNewTolerance() {
		   // If rubber-banding is active, update its confirmations
		   if (currentFibAnalysis != nullptr) {
			   currentFibAnalysis->DetectConfirmations(ListOfCandlesticks, tolerance);
			   DisplayConfirmationsFor(currentFibAnalysis);
		   }

		   // Redraw selected waves
		   DrawSelectedWaves();
	   }
			  /// <summary>
/// Gets the currently selected wave from either checklist
/// </summary>
			  Wave^ Form1::GetSelectedWave() {
				  // Check if any up wave is selected
				  if (checkedListBox_UpWaves->CheckedItems->Count > 0)
					  return safe_cast<Wave^>(checkedListBox_UpWaves->CheckedItems[0]);

				  // Check if any down wave is selected
				  if (checkedListBox_DownWaves->CheckedItems->Count > 0)
					  return safe_cast<Wave^>(checkedListBox_DownWaves->CheckedItems[0]);

				  return nullptr;
			  }

			  /// <summary>
			  /// Initializes simulation parameters for the given wave
			  /// </summary>
			  System::Void Form1::InitializeSimulation(Wave^ wave) {
				  simulationWave = wave;

				  // Get the range percentage and number of steps from scroll bars
				  int rangePercentage = hScrollBar_PercentRange->Value;
				  int numberOfSteps = hScrollBar_StepSize->Value;

				  // Calculate the wave's height
				  double waveHeight = Math::Abs(wave->EndValue - wave->StartValue);

				  // Calculate step size using the formula: (2*PercentRange*waveHeight)/(100*number of steps)
				  stepSize = (2 * rangePercentage * waveHeight) / (100 * numberOfSteps);

				  // Calculate price extension based on the wave height percentage
				  double extension = waveHeight * rangePercentage / 100;

				  // In both cases, simulate around the TOP of the wave box
				  if (wave->IsUpWave) {
					  // For up waves, EndValue is the top of the box
					  minPrice = wave->EndValue - extension;
					  maxPrice = wave->EndValue + extension;
					  currentPrice = minPrice;
					  increasing = true;
				  }
				  else {
					  // For down waves, StartValue is the top of the box
					  minPrice = wave->StartValue - extension;
					  maxPrice = wave->StartValue + extension;
					  currentPrice = maxPrice;
					  increasing = false;
				  }

				  // Update the price label
				  label_Price->Text = String::Format("Price: {0:F4}", currentPrice);
			  }

			  /// <summary>
			  /// Step the price up or down by the step size
			  /// </summary>
			  System::Void Form1::StepPrice(bool increase) {
				  if (simulationWave == nullptr)
					  return;

				  if (increase) {
					  // Increment price
					  currentPrice += stepSize;
					  if (currentPrice > maxPrice)
						  currentPrice = maxPrice;
				  }
				  else {
					  // Decrement price
					  currentPrice -= stepSize;
					  if (currentPrice < minPrice)
						  currentPrice = minPrice;
				  }

				  // Update the display with the new price
				  SimulateAtPrice(currentPrice);
			  }

			  /// <summary>
			  /// Update visualization for the current simulated price
			  /// </summary>
			  System::Void Form1::SimulateAtPrice(double price) {
				  if (simulationWave == nullptr)
					  return;

				  // Clear previous visualization
				  ClearRubberBandAnnotations();

				  // Create temporary point objects for the simulation
				  PeakValleyDetector::Point^ startPoint = gcnew PeakValleyDetector::Point();
				  startPoint->Index = simulationWave->StartIndex;
				  startPoint->Type = simulationWave->IsUpWave ?
					  PeakValleyDetector::PointType::Valley : PeakValleyDetector::PointType::Peak;
				  startPoint->Candlestick = ListOfCandlesticks[simulationWave->StartIndex];

				  // Create end point (using the actual wave's end index but with our simulated price)
				  PeakValleyDetector::Point^ endPoint = gcnew PeakValleyDetector::Point();
				  endPoint->Index = simulationWave->EndIndex;
				  endPoint->Type = simulationWave->IsUpWave ?
					  PeakValleyDetector::PointType::Peak : PeakValleyDetector::PointType::Valley;
				  endPoint->Candlestick = ListOfCandlesticks[simulationWave->EndIndex];

				  // Create a temporary Fibonacci analysis
				  FibonacciAnalysis^ fibAnalysis = gcnew FibonacciAnalysis();
				  fibAnalysis->StartPoint = startPoint;
				  fibAnalysis->StartDate = simulationWave->StartDate;
				  fibAnalysis->StartValue = simulationWave->StartValue;
				  fibAnalysis->EndPoint = endPoint;
				  fibAnalysis->EndDate = simulationWave->EndDate;
				  fibAnalysis->EndValue = price; // Use the simulated price
				  fibAnalysis->IsUpWave = simulationWave->IsUpWave;

				  // Calculate Fibonacci values
				  fibAnalysis->CalculateFibonacciValues();
				  fibAnalysis->DetectConfirmations(ListOfCandlesticks);

				  // Display visualization
				  DrawSimulation(fibAnalysis);

				  // Update labels
				  label_ConfirmCount->Text = "Confirmations: " + fibAnalysis->GetTotalConfirmationCount().ToString();
				  label_Price->Text = "Price: " + price.ToString("F4");
			  }

			  /// <summary>
			  /// Draw the simulation visualization on the chart
			  /// </summary>
			  System::Void Form1::DrawSimulation(FibonacciAnalysis^ fibAnalysis) {
				  if (fibAnalysis == nullptr)
					  return;

				  int startIdx = fibAnalysis->StartPoint->Index + 1;
				  int endIdx = fibAnalysis->EndPoint->Index + 1;

				  // Draw rectangle
				  RectangleAnnotation^ rect = gcnew RectangleAnnotation();
				  rect->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
				  rect->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
				  rect->BackColor = Color::FromArgb(50, Color::Blue);
				  rect->LineColor = Color::Blue;
				  rect->LineWidth = 1;
				  rect->IsSizeAlwaysRelative = false;
				  rect->X = Math::Min(startIdx, endIdx);
				  rect->Width = Math::Abs(endIdx - startIdx);
				  rect->Y = Math::Min(fibAnalysis->StartValue, fibAnalysis->EndValue);
				  rect->Height = Math::Abs(fibAnalysis->EndValue - fibAnalysis->StartValue);
				  chart_LoadTicker2->Annotations->Add(rect);

				  // Draw diagonal line
				  LineAnnotation^ diagonal = gcnew LineAnnotation();
				  diagonal->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
				  diagonal->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
				  diagonal->LineColor = Color::Purple;
				  diagonal->LineWidth = 2;
				  diagonal->IsSizeAlwaysRelative = false;
				  diagonal->X = startIdx;
				  diagonal->Y = fibAnalysis->StartValue;
				  diagonal->Width = endIdx - startIdx;
				  diagonal->Height = fibAnalysis->EndValue - fibAnalysis->StartValue;
				  chart_LoadTicker2->Annotations->Add(diagonal);

				  // Draw Fibonacci lines
				  for each (double fibValue in fibAnalysis->FibonacciValues) {
					  LineAnnotation^ line = gcnew LineAnnotation();
					  line->AxisX = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisX;
					  line->AxisY = chart_LoadTicker2->ChartAreas["ChartArea_OHLC2"]->AxisY;
					  line->LineColor = Color::Purple;
					  line->LineDashStyle = ChartDashStyle::Dash;
					  line->LineWidth = 1;
					  line->IsSizeAlwaysRelative = false;
					  line->X = Math::Min(startIdx, endIdx);
					  line->Y = fibValue;
					  line->Width = Math::Abs(endIdx - startIdx);
					  line->Height = 0;
					  chart_LoadTicker2->Annotations->Add(line);
				  }

				  // Draw confirmations
				  DisplayConfirmationsFor(fibAnalysis);
			  }

private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
	if (simulationWave == nullptr) {
		timer_Simulation->Stop();
		button_StartStop->Text = "Start";
		button_Increase->Enabled = true;
		button_Decrease->Enabled = true;
		return;
	}

	// Update price based on direction
	if (increasing) {
		currentPrice += stepSize;
		if (currentPrice >= maxPrice) {
			currentPrice = maxPrice;
			increasing = false; // Reverse direction when hitting max
		}
	}
	else {
		currentPrice -= stepSize;
		if (currentPrice <= minPrice) {
			currentPrice = minPrice;
			increasing = true; // Reverse direction when hitting min
		}
	}

	// Update the display with the current price
	SimulateAtPrice(currentPrice);

}
private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void hScrollBar_PercentRange_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
	int value = hScrollBar_PercentRange->Value;
	label_PercentRange->Text = String::Format("Range ({0}%) - ", value);

	// Recalculate min/max prices if simulation is active
	if (simulationWave != nullptr) {
		double waveHeight = Math::Abs(simulationWave->EndValue - simulationWave->StartValue);
		double extension = waveHeight * value / 100;

		if (simulationWave->IsUpWave) {
			// For up waves, simulate around EndValue (top of the box)
			minPrice = simulationWave->EndValue - extension;
			maxPrice = simulationWave->EndValue + extension;
		}
		else {
			// For down waves, simulate around StartValue (top of the box)
			minPrice = simulationWave->StartValue - extension;
			maxPrice = simulationWave->StartValue + extension;
		}

		// Recalculate step size with the new values
		int numberOfSteps = hScrollBar_StepSize->Value;
		stepSize = (2 * value * waveHeight) / (100 * numberOfSteps);
	}
}
private: System::Void hScrollBar_StepSize_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
	int value = hScrollBar_StepSize->Value;
	label_StepSize->Text = String::Format("Step Size ({0}) -", value);

	// Update step size if simulation is active
	if (simulationWave != nullptr) {
		int rangePercentage = hScrollBar_PercentRange->Value;
		double waveHeight = Math::Abs(simulationWave->EndValue - simulationWave->StartValue);
		stepSize = (2 * rangePercentage * waveHeight) / (100 * value);
	}
}
private: System::Void button_StartStop_Click(System::Object^ sender, System::EventArgs^ e) {
	if (timer_Simulation->Enabled) {
		// Stop the simulation
		timer_Simulation->Stop();
		button_StartStop->Text = "Start";
		button_Increase->Enabled = true;
		button_Decrease->Enabled = true;
		Application::DoEvents(); // Force UI update
	}
	else {
		// Get the selected wave
		Wave^ selectedWave = GetSelectedWave();
		if (selectedWave == nullptr) {
			MessageBox::Show("Please select a wave first.", "No Wave Selected",
				MessageBoxButtons::OK, MessageBoxIcon::Information);
			return;
		}

		// Start the simulation
		InitializeSimulation(selectedWave);

		// Update UI first
		button_StartStop->Text = "Stop";
		button_Increase->Enabled = false;
		button_Decrease->Enabled = false;
		Application::DoEvents(); // Force UI update

		// Then start timer
		timer_Simulation->Interval = 1200; // Set interval
		timer_Simulation->Start();
	}
}
private: System::Void button_Increase_Click(System::Object^ sender, System::EventArgs^ e) {
	// Get the selected wave
	Wave^ selectedWave = GetSelectedWave();
	if (selectedWave == nullptr) {
		MessageBox::Show("Please select a wave first.", "No Wave Selected",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
		return;
	}

	// If we haven't initialized the simulation yet, do it now
	if (currentPrice == 0) {
		InitializeSimulation(selectedWave);
	}

	// Increase price
	StepPrice(true);
}
private: System::Void button_Decrease_Click(System::Object^ sender, System::EventArgs^ e) {

	// Get the selected wave
	Wave^ selectedWave = GetSelectedWave();
	if (selectedWave == nullptr) {
		MessageBox::Show("Please select a wave first.", "No Wave Selected",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
		return;
	}

	// If we haven't initialized the simulation yet, do it now
	if (currentPrice == 0) {
		InitializeSimulation(selectedWave);
	}

	// Decrease price
	StepPrice(false);
}
};
}


