﻿#include <string>
#include <array>
#include <iostream>

#pragma once
// <opencv2/core.hpp>
// <opencv2/highgui/highgui.hpp>
namespace SerialApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	//using namespace std;
	using namespace System::IO::Ports;
	using namespace System::IO;
	//using namespace cv;


	/// <summary>
	/// Ñâîäêà äëÿ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		static SerialPort^ _serialPort;
		int comportOpenState = 0;
		UInt16 motorSpeed = 0;
		UInt16 motorPulseCnt = 0;
		UInt16 dowBoyX = 0;
		UInt16 dowBoyY = 0;
		UInt16 dowBoyZ = 0;
		Int16 dowBoyYC = 0;
		Int16 dowBoyZC = 0;
		UInt16 dowBoyD8 = 0;
		UInt16 netAddress = 1;
		//Bitmap^ bm = gcnew Bitmap("frame_mf_2148.png");

		UInt16 crcInit = 0xffff;
		Byte imageOrder = 10;

	private: System::Windows::Forms::TextBox^ textBoxCRC;
	private: System::Windows::Forms::ComboBox^ comboBoxAddress;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ buttonImageUpdate;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::TextBox^ textBoxD8;
	private: System::Windows::Forms::Label^ label4;






		   static array<UInt16>^ crcTable = gcnew array<UInt16>{
			   0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
				   0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
				   0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
				   0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
				   0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
				   0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
				   0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
				   0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
				   0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
				   0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
				   0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
				   0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
				   0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
				   0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
				   0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
				   0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
				   0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
				   0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
				   0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
				   0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
				   0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
				   0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
				   0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
				   0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
				   0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
				   0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
				   0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
				   0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
				   0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
				   0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
				   0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
				   0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
		   };

		   [System::Runtime::InteropServices::DllImportAttribute("gdi32.dll")]
		   static bool DeleteObject(IntPtr hObject);

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			_serialPort = gcnew SerialPort();
			_serialPort->BaudRate = 9600;
			_serialPort->DataBits = 8;
			_serialPort->StopBits = StopBits::One;
			_serialPort->Parity = Parity::None;
			_serialPort->Handshake = Handshake::None;


			findPorts();
			//TODO: äîáàâüòå êîä êîíñòðóêòîðà
			//
		}

	protected:
		/// <summary>
		/// Îñâîáîäèòü âñå èñïîëüçóåìûå ðåñóðñû.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^ comboBox_comportList;
	private: System::Windows::Forms::Button^ btn_connect;
	protected:

	protected:

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ textBoxSpeed;

	private: System::Windows::Forms::Label^ labelParam1;



	private: System::Windows::Forms::Button^ buttonSend;

	private: System::IO::Ports::SerialPort^ serialPort1;





	private: System::Windows::Forms::TextBox^ textBoxX;


	private: System::Windows::Forms::Label^ X_ax_label;
	private: System::Windows::Forms::ComboBox^ comboBoxCMD;


	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ Y_ax_label;
	private: System::Windows::Forms::Label^ Z_ax_label;
	private: System::Windows::Forms::TextBox^ textBoxY;
	private: System::Windows::Forms::TextBox^ textBoxZ;




	private: System::Windows::Forms::TextBox^ statusString;


	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Îáÿçàòåëüíàÿ ïåðåìåííàÿ êîíñòðóêòîðà.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Òðåáóåìûé ìåòîä äëÿ ïîääåðæêè êîíñòðóêòîðà — íå èçìåíÿéòå 
		/// ñîäåðæèìîå ýòîãî ìåòîäà ñ ïîìîùüþ ðåäàêòîðà êîäà.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->comboBox_comportList = (gcnew System::Windows::Forms::ComboBox());
			this->btn_connect = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxSpeed = (gcnew System::Windows::Forms::TextBox());
			this->labelParam1 = (gcnew System::Windows::Forms::Label());
			this->buttonSend = (gcnew System::Windows::Forms::Button());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->textBoxX = (gcnew System::Windows::Forms::TextBox());
			this->X_ax_label = (gcnew System::Windows::Forms::Label());
			this->comboBoxCMD = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Y_ax_label = (gcnew System::Windows::Forms::Label());
			this->Z_ax_label = (gcnew System::Windows::Forms::Label());
			this->textBoxY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxZ = (gcnew System::Windows::Forms::TextBox());
			this->statusString = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCRC = (gcnew System::Windows::Forms::TextBox());
			this->comboBoxAddress = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->buttonImageUpdate = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->textBoxD8 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// comboBox_comportList
			// 
			this->comboBox_comportList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_comportList->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->comboBox_comportList->FormattingEnabled = true;
			this->comboBox_comportList->Location = System::Drawing::Point(14, 46);
			this->comboBox_comportList->Name = L"comboBox_comportList";
			this->comboBox_comportList->Size = System::Drawing::Size(68, 21);
			this->comboBox_comportList->TabIndex = 0;
			this->comboBox_comportList->TabStop = false;
			this->comboBox_comportList->DropDown += gcnew System::EventHandler(this, &MyForm::ComboBox_comportList_SelectedIndexChanged_DropDown);
			this->comboBox_comportList->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBox_comportList_SelectedIndexChanged);
			// 
			// btn_connect
			// 
			this->btn_connect->BackColor = System::Drawing::SystemColors::Control;
			this->btn_connect->FlatAppearance->BorderSize = 2;
			this->btn_connect->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Black;
			this->btn_connect->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Black;
			this->btn_connect->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->btn_connect->Location = System::Drawing::Point(87, 34);
			this->btn_connect->Name = L"btn_connect";
			this->btn_connect->Size = System::Drawing::Size(57, 32);
			this->btn_connect->TabIndex = 1;
			this->btn_connect->TabStop = false;
			this->btn_connect->Text = L"Connect";
			this->btn_connect->UseVisualStyleBackColor = false;
			this->btn_connect->Click += gcnew System::EventHandler(this, &MyForm::Button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->label1->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(12, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 19);
			this->label1->TabIndex = 2;
			this->label1->Text = L"ComPort";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::Label1_Click);
			// 
			// textBoxSpeed
			// 
			this->textBoxSpeed->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->textBoxSpeed->Location = System::Drawing::Point(16, 223);
			this->textBoxSpeed->MaxLength = 6;
			this->textBoxSpeed->Name = L"textBoxSpeed";
			this->textBoxSpeed->Size = System::Drawing::Size(67, 20);
			this->textBoxSpeed->TabIndex = 4;
			this->textBoxSpeed->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBoxSpeed->Leave += gcnew System::EventHandler(this, &MyForm::TextBoxSpeed_TextChanged);
			this->textBoxSpeed->KeyDown += gcnew  KeyEventHandler(this, &MyForm::TextBoxSpeed_EnterPressed);
			// 
			// labelParam1
			// 
			this->labelParam1->AutoSize = true;
			this->labelParam1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->labelParam1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->labelParam1->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelParam1->Location = System::Drawing::Point(14, 201);
			this->labelParam1->Name = L"labelParam1";
			this->labelParam1->Size = System::Drawing::Size(66, 19);
			this->labelParam1->TabIndex = 4;
			this->labelParam1->Text = L"Speed X";
			// 
			// buttonSend
			// 
			this->buttonSend->BackColor = System::Drawing::SystemColors::Control;
			this->buttonSend->Enabled = false;
			this->buttonSend->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonSend->Location = System::Drawing::Point(197, 105);
			this->buttonSend->Name = L"buttonSend";
			this->buttonSend->Size = System::Drawing::Size(75, 23);
			this->buttonSend->TabIndex = 7;
			this->buttonSend->TabStop = false;
			this->buttonSend->Text = L"Send";
			this->buttonSend->UseVisualStyleBackColor = false;
			this->buttonSend->Click += gcnew System::EventHandler(this, &MyForm::ButtonSend_Click);
			// 
			// textBoxX
			// 
			this->textBoxX->Location = System::Drawing::Point(15, 172);
			this->textBoxX->Name = L"textBoxX";
			this->textBoxX->Size = System::Drawing::Size(67, 20);
			this->textBoxX->TabIndex = 1;
			this->textBoxX->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBoxX->Leave += gcnew System::EventHandler(this, &MyForm::TextBoxX_TextChanged);
			this->textBoxX->KeyDown += gcnew  KeyEventHandler(this, &MyForm::TextBoxX_EnterPressed);
			// 
			// X_ax_label
			// 
			this->X_ax_label->AutoSize = true;
			this->X_ax_label->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->X_ax_label->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->X_ax_label->Location = System::Drawing::Point(24, 150);
			this->X_ax_label->Name = L"X_ax_label";
			this->X_ax_label->Size = System::Drawing::Size(58, 19);
			this->X_ax_label->TabIndex = 12;
			this->X_ax_label->Text = L"X(mm)";
			this->X_ax_label->Click += gcnew System::EventHandler(this, &MyForm::Label2_Click);
			// 
			// comboBoxCMD
			// 
			this->comboBoxCMD->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxCMD->FormattingEnabled = true;
			this->comboBoxCMD->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"Set Speed", L"Set mm(X)", L"Set mm(Y)", L"Set mm(Z)",
					L"Set mm(XYZ)", L"Set D8", L"Get coordinates"
			});
			this->comboBoxCMD->Location = System::Drawing::Point(88, 105);
			this->comboBoxCMD->Name = L"comboBoxCMD";
			this->comboBoxCMD->Size = System::Drawing::Size(94, 21);
			this->comboBoxCMD->TabIndex = 13;
			this->comboBoxCMD->TabStop = false;
			this->comboBoxCMD->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(102, 83);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(42, 19);
			this->label2->TabIndex = 14;
			this->label2->Text = L"CMD";
			// 
			// Y_ax_label
			// 
			this->Y_ax_label->AutoSize = true;
			this->Y_ax_label->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->Y_ax_label->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Y_ax_label->Location = System::Drawing::Point(97, 150);
			this->Y_ax_label->Name = L"Y_ax_label";
			this->Y_ax_label->Size = System::Drawing::Size(59, 19);
			this->Y_ax_label->TabIndex = 15;
			this->Y_ax_label->Text = L"Y(mm)";
			// 
			// Z_ax_label
			// 
			this->Z_ax_label->AutoSize = true;
			this->Z_ax_label->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->Z_ax_label->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Z_ax_label->Location = System::Drawing::Point(171, 150);
			this->Z_ax_label->Name = L"Z_ax_label";
			this->Z_ax_label->Size = System::Drawing::Size(58, 19);
			this->Z_ax_label->TabIndex = 16;
			this->Z_ax_label->Text = L"Z(mm)";
			// 
			// textBoxY
			// 
			this->textBoxY->Location = System::Drawing::Point(88, 172);
			this->textBoxY->Name = L"textBoxY";
			this->textBoxY->Size = System::Drawing::Size(68, 20);
			this->textBoxY->TabIndex = 2;
			this->textBoxY->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBoxY->Leave += gcnew System::EventHandler(this, &MyForm::TextBoxY_TextChanged);
			this->textBoxY->KeyDown += gcnew  KeyEventHandler(this, &MyForm::TextBoxY_EnterPressed);
			// 
			// textBoxZ
			// 
			this->textBoxZ->Location = System::Drawing::Point(162, 172);
			this->textBoxZ->Name = L"textBoxZ";
			this->textBoxZ->Size = System::Drawing::Size(67, 20);
			this->textBoxZ->TabIndex = 3;
			this->textBoxZ->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBoxZ->Leave += gcnew System::EventHandler(this, &MyForm::TextBoxZ_TextChanged);
			this->textBoxZ->KeyDown += gcnew  KeyEventHandler(this, &MyForm::TextBoxZ_EnterPressed);
			// 
			// statusString
			// 
			this->statusString->Enabled = false;
			this->statusString->Location = System::Drawing::Point(16, 263);
			this->statusString->Multiline = true;
			this->statusString->Name = L"statusString";
			this->statusString->Size = System::Drawing::Size(230, 49);
			this->statusString->TabIndex = 21;
			this->statusString->TabStop = false;
			this->statusString->Text = L"Status string";
			// 
			// textBoxCRC
			// 
			this->textBoxCRC->Enabled = false;
			this->textBoxCRC->Location = System::Drawing::Point(16, 318);
			this->textBoxCRC->Name = L"textBoxCRC";
			this->textBoxCRC->Size = System::Drawing::Size(72, 20);
			this->textBoxCRC->TabIndex = 22;
			this->textBoxCRC->TabStop = false;
			this->textBoxCRC->Text = L"CRC_result";
			// 
			// comboBoxAddress
			// 
			this->comboBoxAddress->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxAddress->Enabled = false;
			this->comboBoxAddress->FormattingEnabled = true;
			this->comboBoxAddress->Items->AddRange(gcnew cli::array< System::Object^  >(21) {
				L"0", L"1", L"2", L"3", L"4", L"5", L"6",
					L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20"
			});
			this->comboBoxAddress->Location = System::Drawing::Point(16, 105);
			this->comboBoxAddress->Name = L"comboBoxAddress";
			this->comboBoxAddress->Size = System::Drawing::Size(66, 21);
			this->comboBoxAddress->TabIndex = 8;
			this->comboBoxAddress->TabStop = false;
			this->comboBoxAddress->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxAddress_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(16, 83);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(66, 19);
			this->label3->TabIndex = 24;
			this->label3->Text = L"Address";
			// 
			// pictureBox1
			// 
			this->pictureBox1->ErrorImage = nullptr;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->InitialImage = nullptr;
			this->pictureBox1->Location = System::Drawing::Point(354, 58);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(530, 396);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 25;
			this->pictureBox1->TabStop = false;
			// 
			// buttonImageUpdate
			// 
			this->buttonImageUpdate->Location = System::Drawing::Point(354, 16);
			this->buttonImageUpdate->Name = L"buttonImageUpdate";
			this->buttonImageUpdate->Size = System::Drawing::Size(95, 36);
			this->buttonImageUpdate->TabIndex = 26;
			this->buttonImageUpdate->TabStop = false;
			this->buttonImageUpdate->Text = L"Image Update";
			this->buttonImageUpdate->UseVisualStyleBackColor = true;
			this->buttonImageUpdate->Click += gcnew System::EventHandler(this, &MyForm::ButtonImageUpdate_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::Timer1_Tick);
			// 
			// textBoxD8
			// 
			this->textBoxD8->Location = System::Drawing::Point(88, 223);
			this->textBoxD8->Name = L"textBoxD8";
			this->textBoxD8->Size = System::Drawing::Size(67, 20);
			this->textBoxD8->TabIndex = 5;
			this->textBoxD8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBoxD8->Leave += gcnew System::EventHandler(this, &MyForm::TextBoxD8_TextChanged);
			this->textBoxD8->KeyDown += gcnew  KeyEventHandler(this, &MyForm::TextBoxD8_EnterPressed);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->label4->Font = (gcnew System::Drawing::Font(L"Tahoma", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(102, 201);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(29, 19);
			this->label4->TabIndex = 28;
			this->label4->Text = L"D8";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->ClientSize = System::Drawing::Size(1008, 579);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBoxD8);
			this->Controls->Add(this->buttonImageUpdate);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBoxAddress);
			this->Controls->Add(this->textBoxCRC);
			this->Controls->Add(this->statusString);
			this->Controls->Add(this->textBoxZ);
			this->Controls->Add(this->textBoxY);
			this->Controls->Add(this->Z_ax_label);
			this->Controls->Add(this->Y_ax_label);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBoxCMD);
			this->Controls->Add(this->X_ax_label);
			this->Controls->Add(this->textBoxX);
			this->Controls->Add(this->buttonSend);
			this->Controls->Add(this->labelParam1);
			this->Controls->Add(this->textBoxSpeed);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btn_connect);
			this->Controls->Add(this->comboBox_comportList);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximumSize = System::Drawing::Size(1024, 768);
			this->MinimumSize = System::Drawing::Size(600, 420);
			this->Name = L"MyForm";
			this->Text = L"SerialControl";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//find available ports
	private: void	findPorts(void) {
		array<Object^>^ objectArray = SerialPort::GetPortNames();

		//array<Object^>^ objectArray = System::IO::Ports::SerialPort::GetPortNames();
		this->comboBox_comportList->Items->Clear();
		this->comboBox_comportList->Items->AddRange(objectArray);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void Button1_Click(System::Object^ sender, System::EventArgs^ e) {
		//connect comport button
		try {
			if (comportOpenState == 0) {
				if (!this->_serialPort->IsOpen) {
					this->_serialPort->PortName = this->comboBox_comportList->Text;
					this->_serialPort->ReadTimeout = 500;
					this->_serialPort->WriteTimeout = 500;
					this->_serialPort->Open();
					this->btn_connect->Text = "Disconnect";
					this->statusString->Text = "Connected";
					comportOpenState = 1;
					this->comboBox_comportList->Enabled = 0;
					this->comboBoxAddress->Enabled = 1;
				}
				else {
					this->statusString->Text = "Port isn`t openned";
				}
			}
			else {
				if (this->_serialPort->IsOpen) {
					this->_serialPort->Close();
					this->statusString->Text = "Disconnected";
					this->btn_connect->Text = "Connect";
					this->comboBox_comportList->Enabled = 1;
					this->buttonSend->Enabled = 0;
					this->comboBoxAddress->Enabled = 0;
					comportOpenState = 0;
				}
				else {
					this->statusString->Text = "Disconnect fault";
				}
			}
		}//try connect/disconnect
		catch (UnauthorizedAccessException^) {
			this->statusString->Text = "UnauthorizedAccess";

		}


	}
	private: System::Void Label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void ComboBox_comportList_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {


	}
	private: System::Void ComboBox_comportList_SelectedIndexChanged_DropDown(Object^ sender, System::EventArgs^ e) {
		findPorts();
	}

	private: System::Void Label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void ComboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (this->_serialPort->IsOpen) {
			if (!this->buttonSend->Enabled)this->buttonSend->Enabled = 1;
		}
	}

	private: System::Void ButtonSend_Click(System::Object^ sender, System::EventArgs^ e) {
		System::String^ cmdState = this->comboBoxCMD->Text;

		int readByteArray[50];
		if (cmdState == "Set Speed") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};
			try {
				//sendDataChars[3] = 0x02;
				sendDataChars[5] = 0x01;
				sendDataChars[6] = 0x02;
				sendDataChars[7] = Convert::ToByte(motorSpeed >> 8);
				sendDataChars[8] = Convert::ToByte(motorSpeed & 0xff);
				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}

		if (cmdState == "Set XYZ position") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x02, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//15bytes
			try {
				sendDataChars[7] = Convert::ToByte(dowBoyX >> 8);
				sendDataChars[8] = Convert::ToByte(dowBoyX & 0xff);
				sendDataChars[9] = Convert::ToByte(dowBoyY >> 8);
				sendDataChars[10] = Convert::ToByte(dowBoyY & 0xff);
				sendDataChars[11] = Convert::ToByte(dowBoyZ >> 8);
				sendDataChars[12] = Convert::ToByte(dowBoyZ & 0xff);
				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[14] = (Byte)(temp_crc_result >> 8);
				sendDataChars[13] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		if (cmdState == "Set X") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = Convert::ToByte(dowBoyX >> 8);
				sendDataChars[8] = Convert::ToByte(dowBoyX & 0xff);

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		if (cmdState == "Set Y") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x03, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = Convert::ToByte(dowBoyY >> 8);
				sendDataChars[8] = Convert::ToByte(dowBoyY & 0xff);

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		if (cmdState == "Set D8") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x08, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = Convert::ToByte(dowBoyD8 >> 8);
				sendDataChars[8] = Convert::ToByte(dowBoyD8 & 0xff);

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}

		//Set mm(X)
		if (cmdState == "Set mm(X)") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = (dowBoyX >> 8);
				sendDataChars[8] = (dowBoyX & 0xff);
				//sendDataChars[8] = dowBoyYC >> 8;

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		//Set mm(Y)
		if (cmdState == "Set mm(Y)") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = (dowBoyY >> 8);
				sendDataChars[8] = (dowBoyY & 0xff);
				//sendDataChars[8] = dowBoyYC >> 8;

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		//Set mm(Z)
		if (cmdState == "Set mm(Z)") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};//11bytes
			try {
				sendDataChars[7] = (dowBoyZ >> 8);
				sendDataChars[8] = (dowBoyZ & 0xff);
				//sendDataChars[8] = dowBoyYC >> 8;

				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[10] = (Byte)(temp_crc_result >> 8);
				sendDataChars[9] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}
		if (cmdState == "Set mm(XYZ)") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x10, 0x00, 0x02, 0x00, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//21bytes
			try {
				sendDataChars[7] = Convert::ToByte(dowBoyX >> 8);
				sendDataChars[8] = Convert::ToByte(dowBoyX & 0xff);
				sendDataChars[11] = Convert::ToByte(dowBoyY >> 8);
				sendDataChars[12] = Convert::ToByte(dowBoyY & 0xff);
				sendDataChars[15] = Convert::ToByte(dowBoyZ >> 8);
				sendDataChars[16] = Convert::ToByte(dowBoyZ & 0xff);
				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[20] = (Byte)(temp_crc_result >> 8);
				sendDataChars[19] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 8; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}

		if (cmdState == "Get coordinates") {
			array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x03, 0x00, 0x0A, 0x00, 0x08, 0x00, 0x00};//9bytes
			try {
				this->_serialPort->ReadExisting();/*  -------------  inserted after pull  request  ++++++++++++++++ */
				UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
				this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
				sendDataChars[8] = (Byte)(temp_crc_result >> 8);
				sendDataChars[7] = (Byte)(temp_crc_result & 0xff);
				this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
				try {
					for (int i_rb = 0; i_rb < 21; i_rb++) {
						readByteArray[i_rb] = this->_serialPort->ReadByte();
					}
					if (readByteArray[0] == 0x01) {
						System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2} 0x{8,2:X2} 0x{9,2:X2} 0x{10,2:X2} 0x{11,2:X2} 0x{12,2:X2}", \
							readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
							readByteArray[5], readByteArray[6], readByteArray[7], readByteArray[8], readByteArray[9], readByteArray[10], readByteArray[11], readByteArray[12]);
						//this->statusString->Text = Convert::ToString(sendDataChars->Length);
						this->statusString->Text = formatedString;
					}
				}
				catch (TimeoutException^) {
					this->statusString->Text = "TimeoutRx_sendCmd";
					System::String^ formatedString = System::String::Format("0x{0,2:X2} 0x{1,2:X2} 0x{2,2:X2} 0x{3,2:X2} 0x{4,2:X2} 0x{5,2:X2} 0x{6,2:X2} 0x{7,2:X2} 0x{8,2:X2} 0x{9,2:X2} 0x{10,2:X2} 0x{11,2:X2} 0x{12,2:X2}", \
						readByteArray[0], readByteArray[1], readByteArray[2], readByteArray[3], readByteArray[4], \
						readByteArray[5], readByteArray[6], readByteArray[7], readByteArray[8], readByteArray[9], readByteArray[10], readByteArray[11], readByteArray[12]);
					//this->statusString->Text = Convert::ToString(sendDataChars->Length);
					this->statusString->Text = formatedString;
				}
			}
			catch (InvalidOperationException^) {
				;
			}
		}

	}
	private: System::Void TextBoxSpeed_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		try {
			motorSpeed = Convert::ToUInt32(this->textBoxSpeed->Text);
			this->statusString->Text = "Speed X changed";
		}
		catch (FormatException^) {
			this->statusString->Text = "only numbers";
			this->textBoxSpeed->Text = "0";
			motorSpeed = 0;
		}
		catch (OverflowException^) {
			this->statusString->Text = "to large value";
			this->textBoxSpeed->Text = "65535";
			motorSpeed = 65535;
		}
	}

	private: System::Void TextBoxX_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		try {
			dowBoyX = Convert::ToUInt16(this->textBoxX->Text);
			this->statusString->Text = "X changed";
		}
		catch (FormatException^) {
			this->statusString->Text = "only numbers";
			this->textBoxX->Text = "0";
			dowBoyX = 0;
		}
		catch (OverflowException^) {
			this->statusString->Text = "to large value";
			this->textBoxX->Text = "65535";
			dowBoyX = 65535;
		}

	}
	
	private: System::Void TextBoxD8_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		try {
			dowBoyD8 = Convert::ToUInt16(this->textBoxX->Text);
			this->statusString->Text = "D8 changed";
		}
		catch (FormatException^) {
			this->statusString->Text = "only numbers";
			this->textBoxD8->Text = "0";
			dowBoyD8 = 0;
		}
		catch (OverflowException^) {
			this->statusString->Text = "to large value";
			this->textBoxD8->Text = "65535";
			dowBoyD8 = 65535;
		}

	}

	private: UInt16 GetCoordinate(Byte coordinate) {
		array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x03, 0x00, 0x0A, 0x00, 0x08, 0x00, 0x00};//9bytes
		UInt16 tempCoordinateArray[20];
		UInt16 coordinateResult;
		try {
			UInt16 temp_crc_result = CRC_Calc16(sendDataChars, sendDataChars->Length - 2);
			this->textBoxCRC->Text = Convert::ToString(temp_crc_result, 16);
			sendDataChars[8] = (Byte)(temp_crc_result >> 8);
			sendDataChars[7] = (Byte)(temp_crc_result & 0xff);
			this->_serialPort->Write(sendDataChars, 0, sendDataChars->Length);
			try {
				for (int i_rb = 0; i_rb < 21; i_rb++) {
					tempCoordinateArray[i_rb] = this->_serialPort->ReadByte();
				}
			}
			catch (TimeoutException^) {
				this->statusString->Text = "TimeoutRx_sendCmd";
			}
			if ((coordinate == 0)||(coordinate == 'X') || (coordinate == 'x')) {
				coordinateResult = (tempCoordinateArray[4]|(tempCoordinateArray[3]<<8));
			}
			if ((coordinate == 1) || (coordinate == 'Y') || (coordinate == 'y')) {
				coordinateResult = (tempCoordinateArray[8] | (tempCoordinateArray[7] << 8));
			}
			if ((coordinate == 2) || (coordinate == 'Z') || (coordinate == 'z')) {
				coordinateResult = (tempCoordinateArray[12] | (tempCoordinateArray[11] << 8));
			}
			if ((coordinate == 3) || (coordinate == 'F') || (coordinate == 'f')) {
				coordinateResult = (tempCoordinateArray[16] | (tempCoordinateArray[15] << 8));
			}
	
		}
		catch (InvalidOperationException^) {
			;
		}

		return coordinateResult;
	}
	
	private: System::Void TextBoxX_EnterPressed(System::Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			this->textBoxY->Focus();
		}
		if (e->KeyCode == Keys::Up) {
			try {
				dowBoyX = Convert::ToUInt16(this->textBoxX->Text);
				dowBoyX += 1;
				this->textBoxX->Text = Convert::ToString(dowBoyX);
			}
			catch (FormatException^) {
				this->statusString->Text = "only numbers";
				this->textBoxX->Text = "0";
				dowBoyX = 0;
			}
			catch (OverflowException^) {
				this->statusString->Text = "to large value";
				this->textBoxX->Text = "2000";
				dowBoyX = 2000;
			}
		}
		if (e->KeyCode == Keys::Down) {
			try {
				dowBoyX = Convert::ToUInt16(this->textBoxX->Text);
				if(dowBoyX != 0)dowBoyX -= 1;
				this->textBoxX->Text = Convert::ToString(dowBoyX);
			}
			catch (FormatException^) {
				this->statusString->Text = "only numbers";
				this->textBoxX->Text = "0";
				dowBoyX = 0;
			}
			catch (OverflowException^) {
				this->statusString->Text = "to large value";
				this->textBoxX->Text = "2000";
				dowBoyX = 2000;
			}
		}

	}
	private: System::Void TextBoxY_EnterPressed(System::Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			this->textBoxZ->Focus();
		}

	}
	private: System::Void TextBoxZ_EnterPressed(System::Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			this->textBoxD8->Focus();
		}
	}
	private: System::Void TextBoxD8_EnterPressed(System::Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			this->textBoxSpeed->Focus();
		}
	}
	private: System::Void TextBoxSpeed_EnterPressed(System::Object^ sender, KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			this->textBoxX->Focus();
		}
	}



	private: System::Void TextBoxY_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		try {
			dowBoyY = Convert::ToUInt16(this->textBoxY->Text);
			this->statusString->Text = "Y pos changed";
		}
		catch (FormatException^) {
			this->statusString->Text = "only numbers";
			this->textBoxY->Text = "0";
			dowBoyY = 0;
		}
		catch (OverflowException^) {
			this->statusString->Text = "to large value";
			this->textBoxY->Text = "65535";
			dowBoyY = 65535;
		}

	}
	private: System::Void TextBoxZ_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		try {
			dowBoyZ = Convert::ToUInt16(this->textBoxZ->Text);
			this->statusString->Text = "Z pos changed";
		}
		catch (FormatException^) {
			this->statusString->Text = "only numbers";
			this->textBoxZ->Text = "0";
			dowBoyZ = 0;
		}
		catch (OverflowException^) {
			this->statusString->Text = "to large value";
			this->textBoxZ->Text = "65535";
			dowBoyZ = 65535;
		}

	}


	private: System::UInt16 CRC_Calc16(array<Byte>^ dataArray, int length) {//array<Byte>^  //Byte *dataArray
		UInt16 crc = 0xffff;
		UInt16 table_result = 0;
		for (int i_crc = 0; i_crc < length; i_crc++) {
			crc = (crc >> 8) ^ crcTable[(crc ^ dataArray[i_crc]) & 0xff];
		}
		return crc;
	}
	private: System::Void ComboBoxAddress_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {

		netAddress = Convert::ToUInt16(this->comboBoxAddress->Text);

	}
	private: System::Void ButtonImageUpdate_Click(System::Object^ sender, System::EventArgs^ e) {


		//IntPtr hBitmap = bm->GetHbitmap(Color::Blue);
		//this->pictureBox1->Image = //imageArray;

		//this->pictureBox1->Image = bm;//Bitmap::FromHbitmap(hBitmap);


		//cv::Mat image;
		//image = cv::imread("D:\\Python\\ex2.png", cv::IMREAD_COLOR);
		//cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
		//cv::imshow("Display image", image);

		//HBITMAP hBit;
		//hBit = CreateBitmap(image.cols, image.rows, 1, 32, image.data);
		//Bitmap^ bmp = Bitmap::FromHbitmap((IntPtr)hBit);
		//this->pictureBox1->Image = image;
		/*
		if (image.data != NULL) {

			System::Drawing::Graphics^ graphics = pictureBox1->CreateGraphics();
			System::IntPtr ptr(image.ptr());
			System::Drawing::Bitmap^ b = gcnew System::Drawing::Bitmap(image.cols, image.rows, image.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
			System::Drawing::RectangleF rect(0, 0, pictureBox1->Width, pictureBox1->Height);
			graphics->DrawImage(b, rect);

			this->timer1->Enabled = true;
		}
		*/

	}
	private: System::Void Timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		//cv::Mat image;

		imageOrder++;
		if (imageOrder >= 11)imageOrder = 1;

		//cv::String filenameString = cv::format("D:\\VSprojects\\ConsoleSerial\\pictures\\d2_%d.png", imageOrder);//"d2_{d}.png"->Format(imageOrder);
		//cv::String filenameString = "D:\\VSprojects\\ConsoleSerial\\pictures\\d2_.png";

		//image = cv::imread("D:\\VSprojects\\ConsoleSerial\\pictures\\d2_1.png", cv::IMREAD_COLOR);
		//image = cv::imread(filenameString, cv::IMREAD_COLOR);

		//System::Drawing::Graphics^ graphics = pictureBox1->CreateGraphics();
		//System::IntPtr ptr(image.ptr());
		//System::Drawing::Bitmap^ b = gcnew System::Drawing::Bitmap(image.cols, image.rows, image.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
		//System::Drawing::RectangleF rect(0, 0, pictureBox1->Width, pictureBox1->Height);
		//graphics->DrawImage(b, rect);

	}


	};
}