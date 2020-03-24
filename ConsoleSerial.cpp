#include "pch.h"
#include "MyForm.h"
#include <Windows.h>

using namespace System;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	System::Windows::Forms::Application::Run(%ConsoleSerial::MyForm());

}
/*
int main(array<System::String ^> ^args)
{



	return 0;
}


*/

