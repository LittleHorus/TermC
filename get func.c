	private: UInt16 GetCoordinate(Byte coordinate) {
		array<Byte>^ sendDataChars = gcnew array<Byte>{0x01, 0x03, 0x00, 0x0A, 0x00, 0x10, 0x00, 0x00};//9bytes
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
