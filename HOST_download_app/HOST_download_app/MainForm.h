#pragma once

namespace HOSTdownloadapp {

	//#include <cstdint>
	//#include <string>
	#include <Windows.h>
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;
	using namespace System::Threading;
	
	//using namespace std;
	//using std::vector;

	#define WM_SEND_BYTE (WM_USER + 1)

	extern "C"
	{
		#include "etx_ota_update_main.h"
		#include "protocol_structure.h"
	}
	/// <summary>
	/// MainForm 摘要
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			CheckForIllegalCrossThreadCalls = false;
			findports();			
			InitializeThread();
			//
			//TODO:  在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				StopThread();
				delete components;
			}			
		}
	private: System::IO::Ports::SerialPort^ serialPort1;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::ProgressBar^ progressBar1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	protected:
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			//this->textBox2->SelectionStart = this->textBox2->Text->Length;
			//this->textBox2->ScrollToCaret();
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(103, 27);
			this->comboBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(262, 24);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label1->Location = System::Drawing::Point(22, 29);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 14);
			this->label1->TabIndex = 1;
			this->label1->Text = L"COM port";
			this->label1->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->button1->Location = System::Drawing::Point(417, 27);
			this->button1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(121, 87);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Program";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"宋体", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label2->Location = System::Drawing::Point(24, 95);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 14);
			this->label2->TabIndex = 1;
			this->label2->Text = L"BIN file";
			this->label2->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(324, 89);
			this->button2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(40, 25);
			this->button2->TabIndex = 3;
			this->button2->Text = L"...";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->textBox1->Location = System::Drawing::Point(103, 89);
			this->textBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(201, 26);
			this->textBox1->TabIndex = 4;
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(25, 153);
			this->progressBar1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(514, 26);
			this->progressBar1->TabIndex = 5;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(25, 211);
			this->textBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox2->Size = System::Drawing::Size(515, 184);
			this->textBox2->TabIndex = 6;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(566, 423);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"Jiameng Program Downloader";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	// find available ports
	public: HWND hwnd;
	private: void findports(void) {
		array<Object^>^ objectArray = SerialPort::GetPortNames();
		this->comboBox1->Items->AddRange(objectArray);
	}
	private: array<Byte>^ OTA_cmd_frame(ETX_OTA_CMD_ cmd) {
		ETX_OTA_COMMAND_ ota_package;
		ota_package.sof = ETX_OTA_SOF;
		ota_package.packet_type = ETX_OTA_PACKET_TYPE_CMD;
		ota_package.data_len = 1;
		ota_package.cmd = cmd;
		ota_package.crc = CalcCRC(&ota_package.cmd, 1);
		ota_package.eof = ETX_OTA_EOF;

		IntPtr ptr = Marshal::AllocHGlobal(sizeof(ETX_OTA_COMMAND_));
		array<Byte>^ byteArray = gcnew array<Byte>(sizeof(ETX_OTA_COMMAND_));
		ptr = IntPtr(&ota_package);
		Marshal::Copy(ptr, byteArray, 0, sizeof(ETX_OTA_COMMAND_));

		return byteArray;
	}
	private: Thread^ thread;
	private: bool isRunning;
	private: void InitializeThread();
	private: void StopThread();
	private: void WorkerThreadFunction();
	private: System::Void DataReceivedHandler(
		System::Object^ sender,
		SerialDataReceivedEventArgs^ e)
	{
		SerialPort^ sp = (SerialPort^)sender;
		String^ indata = sp->ReadExisting();
		this->textBox2->Text += indata;
		//this->textBox2->AppendText(indata + Environment::NewLine);
		//PostMessage(hwnd, WM_USER);
		SendByteToThread((unsigned char)indata[0]);
		
	}
	private: void SendByteToThread(unsigned char byteValue) {
		SendMessage(hwnd, WM_SEND_BYTE, 0, static_cast<LPARAM>(byteValue));
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		// init port
		this->textBox2->Text = String::Empty;
		if ((this->comboBox1->Text == String::Empty) || (this->textBox1->Text == String::Empty)) {
			if (this->comboBox1->Text == String::Empty) {
				this->textBox2->Text += "Please Select COM port\r\n";
			}
			if (this->textBox1->Text == String::Empty) {
				this->textBox2->Text += "Please Select BIN file\r\n";
			}
		}
		else {
			try {
				if (!this->serialPort1->IsOpen) {
					this->serialPort1->PortName = this->comboBox1->Text;
					this->serialPort1->BaudRate = 9600;
					this->serialPort1->DataReceived += gcnew SerialDataReceivedEventHandler(this, &MainForm::DataReceivedHandler);
					this->serialPort1->Open();
					this->progressBar1->Value = 10;
					this->textBox2->Text += this->comboBox1->Text + " is opened\r\n";
				}
				else {
					this->textBox2->Text += "Port isn't openned;\r\n";
				}
			}
			catch (UnauthorizedAccessException^) {
				this->textBox2->Text += "UnauthorizedAccess\r\n";
			}
			this->textBox2->Text += "Openning " + this->textBox1->Text + " \r\n";
			FileStream^ input_file = File::Open(/*newpath*/this->textBox1->Text, FileMode::Open);
			if (!input_file) {

			}
			else {
				this->textBox2->Text += "Success!\r\n";
				const auto fsize = input_file->Length;
				this->textBox2->Text += "bin file size is " + fsize + " \r\n";
				auto page_count = fsize / PAGE_SIZE;
				auto page_remain = fsize % PAGE_SIZE;
				this->textBox2->Text += "page count is " + page_count + " \r\n";
				this->textBox2->Text += "page remain is " + page_remain + " \r\n";
				this->progressBar1->Value = 20;

				ETX_OTA_COMMAND_ ota_start;// = { 0 };
				ota_start.sof = ETX_OTA_SOF;
				ota_start.packet_type = ETX_OTA_PACKET_TYPE_CMD;
				ota_start.data_len = 1;
				ota_start.cmd = ETX_OTA_CMD_START;
				ota_start.crc = CalcCRC(&ota_start.cmd, 1);
				ota_start.eof = ETX_OTA_EOF;

				IntPtr ptr = Marshal::AllocHGlobal(sizeof(ETX_OTA_COMMAND_));
				array<Byte>^ byteArray = gcnew array<Byte>(sizeof(ETX_OTA_COMMAND_));
				ptr = IntPtr(&ota_start);
				Marshal::Copy(ptr, byteArray, 0, sizeof(ETX_OTA_COMMAND_));
				//String^ stringB = Marshal::PtrToStringAnsi(ptr);
				//this->textBox2->Text += stringB + "\r\n";
				this->textBox2->Text += byteArray[0] + "\r\n";
				this->textBox2->Text += byteArray[1] + "\r\n";
				this->textBox2->Text += byteArray[2] + "\r\n";
				this->textBox2->Text += byteArray[3] + "\r\n";
				this->textBox2->Text += byteArray[4] + "\r\n";
				this->textBox2->Text += byteArray[5] + "\r\n";
				this->textBox2->Text += byteArray[6] + "\r\n";
				this->textBox2->Text += byteArray[7] + "\r\n";
				this->textBox2->Text += byteArray[8] + "\r\n";
				this->textBox2->Text += byteArray[9] + "\r\n";
				//this->serialPort1->Write(byteArray, 0, byteArray->Length);
				if (this->serialPort1->IsOpen) {
					this->serialPort1->Write(OTA_cmd_frame(ETX_OTA_CMD_START), 0, byteArray->Length);
				}
				else {
					this->textBox2->Text = "Port Not Opened";
				}
			}
		}
		//String^ name = this->serialPort1->PortName;
		//String^ message = "";
		//if (this->serialPort1->IsOpen) {
		//	this->serialPort1->WriteLine(String::Format("<(0)>: {1}", name,message));
		//	this->serialPort1->WriteLine(message);
		//}
		//else {
		//	this->textBox2->Text = "Port Not Opened";
		//}
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		openFileDialog1->InitialDirectory = "c:\\";
		openFileDialog1->Filter = "bin files (*.bin)|*.bin";
		openFileDialog1->FilterIndex = 2;
		openFileDialog1->RestoreDirectory = true;
		openFileDialog1->FileName = "";
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			textBox1->Text = System::IO::Path::GetDirectoryName(openFileDialog1->FileName) +"\\" + System::IO::Path::GetFileName(openFileDialog1->FileName);
		}
	}
};
}
