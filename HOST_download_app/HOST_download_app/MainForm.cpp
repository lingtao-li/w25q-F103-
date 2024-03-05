#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace HOSTdownloadapp;

[System::STAThread]
void main(array<String^>^ argv) {	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	HOSTdownloadapp::MainForm form;
	form.hwnd = static_cast<HWND>(form.Handle.ToPointer());
	Application::Run(% form);	
}

void MainForm::InitializeThread() {
	thread = gcnew Thread(gcnew ThreadStart(this, &MainForm::WorkerThreadFunction));
	isRunning = true;
	thread->Start();
}

void MainForm::StopThread() {
	isRunning = false;
	if (thread != nullptr && thread->IsAlive) {
		//thread->Join();		
		TerminateThread(OpenThread(THREAD_QUERY_INFORMATION, false, thread->ManagedThreadId), 0);
		CloseHandle(OpenThread(THREAD_QUERY_INFORMATION, false, thread->ManagedThreadId));
	}
}

void MainForm::WorkerThreadFunction() {
	uint8_t buf_pointer = 0;
	uint8_t* uart_buf = NULL;
	MSG msg;
	uint8_t process_stage = 1;
	uint8_t crc_base = 0;
	while ((isRunning) && (GetMessage(&msg, NULL, 0, 0))) {
		if (msg.message == WM_SEND_BYTE) {
			unsigned char byteValue = static_cast<unsigned char>(msg.lParam);
			this->textBox2->Text += byteValue;
			if (process_stage == 1) {
				if (byteValue == ETX_OTA_SOF/*0xAA*/) {
					// change stage
					process_stage = 2;
				}
				else {
					// header not correct do nothing
				}
			}
			else if (process_stage == 2) {
				uart_buf[buf_pointer] = byteValue;
				// 0/1/2/3
				if (uart_buf[buf_pointer] <= 2) {
					buf_pointer++;
					process_stage = 3;
				}
				// 0xAA, stay in 2, continue
				else if (uart_buf[buf_pointer] == ETX_OTA_SOF) {
					continue;
				}
				// other than 0xAA, reset
				else {
					goto reset;
				}
			}	// process_stage 2 end		
			// read len
			else if (process_stage == 3) {
				uart_buf[buf_pointer] = byteValue;
				if (uart_buf[buf_pointer] == ETX_OTA_SOF) {
					process_stage = 2;
					buf_pointer = 0;



					this->textBox2->Text += "ok";

					continue;
				}
			}





			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//this->textBox2->Text += "test\r\n";
			//Thread::Sleep(1);
		reset:
			// back stage 1
			process_stage = 1;
			// fifo pointer reset
			buf_pointer = 0;
			crc_base = 0;




		}
	}
}