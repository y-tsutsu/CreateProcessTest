#pragma once

// http://www.ne.jp/asahi/hishidama/home/tech/c/windows/CreateProcess.html#childProcess

#include <stdio.h>
#include <list>
#include <windows.h>

namespace CreateProcessTest
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	std::list<HANDLE> childProcess;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  buttonRun;
	private: System::Windows::Forms::Button^  buttonKill;

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonRun = (gcnew System::Windows::Forms::Button());
			this->buttonKill = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// buttonRun
			// 
			this->buttonRun->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->buttonRun->Location = System::Drawing::Point(197, 227);
			this->buttonRun->Name = L"buttonRun";
			this->buttonRun->Size = System::Drawing::Size(75, 23);
			this->buttonRun->TabIndex = 0;
			this->buttonRun->Text = L"Run";
			this->buttonRun->UseVisualStyleBackColor = true;
			this->buttonRun->Click += gcnew System::EventHandler(this, &MainForm::buttonRun_Click);
			// 
			// buttonKill
			// 
			this->buttonKill->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->buttonKill->Location = System::Drawing::Point(116, 227);
			this->buttonKill->Name = L"buttonKill";
			this->buttonKill->Size = System::Drawing::Size(75, 23);
			this->buttonKill->TabIndex = 1;
			this->buttonKill->Text = L"Kill";
			this->buttonKill->UseVisualStyleBackColor = true;
			this->buttonKill->Click += gcnew System::EventHandler(this, &MainForm::buttonKill_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Controls->Add(this->buttonKill);
			this->Controls->Add(this->buttonRun);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->ResumeLayout(false);

		}
#pragma endregion

	private: void PrintError(const char *message)
			 {
				 LPVOID lpvMessageBuffer;

				 FormatMessage(
					 FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
					 NULL,
					 GetLastError(),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					 (LPTSTR)&lpvMessageBuffer,
					 0,
					 NULL);
				 fprintf(stderr, "%s: %s\n", message, lpvMessageBuffer);
				 LocalFree(lpvMessageBuffer);

				 System::Windows::Forms::MessageBox::Show(gcnew String(message));
			 }

	private: bool Execute()
			 {
				 STARTUPINFO si = { sizeof(STARTUPINFO) };
				 PROCESS_INFORMATION pi = {};
				 TCHAR buffer[MAX_PATH] = TEXT("calc.exe");
				 if (!CreateProcess(
					 NULL,
					 buffer,
					 NULL,	//プロセスのセキュリティー記述子
					 NULL,	//スレッドのセキュリティー記述子
					 FALSE,	//ハンドルを継承しない
					 0,		//作成フラグ
				 	 NULL,	//環境変数は引き継ぐ
					 NULL,	//カレントディレクトリーは同じ
					 &si,
					 &pi)
				 )
				 {
					 PrintError("CreateProcess");
					 return false;
				 }
				 // 子プロセス起動成功
				 childProcess.push_back(pi.hProcess);

				 // 不要なスレッドハンドルをクローズする
				 if (!CloseHandle(pi.hThread))
				 {
					 PrintError("CloseHandle(hThread)");
					 return false;
				 }

				 printf("child processId=%d\n", pi.dwProcessId);

				 return true;
			 }

	private: System::Void buttonRun_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 printf("main[%d] start\n", GetCurrentProcessId());

				 LPTSTR commandLine = TEXT("calc.exe"); //電卓
				 bool result = Execute();

				 printf("main[%d] end\n", GetCurrentProcessId());
			 }

	private: System::Void buttonKill_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if (childProcess.size() == 0) { return; }

				 UINT exitCode = 123;
				 if (!TerminateProcess(*(childProcess.begin()), exitCode))
				 {
					 PrintError("TerminateProcess");
					 return;
				 }
				 childProcess.erase(childProcess.begin());
			 }
	};
}

