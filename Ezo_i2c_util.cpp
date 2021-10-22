
#include <Ezo_i2c_util.h>
#include <TelnetStream.h> //JY - included to support telnet prints originating from this file


// prints the boards name and I2C address
void print_device_info(Ezo_board &Device) {
  Serial.print(Device.get_name());
  Serial.print(" ");
  Serial.print(Device.get_address());
  TelnetStream.print(Device.get_name());
  TelnetStream.print(" ");
  TelnetStream.print(Device.get_address());
}

// used for printing either a success_string message if a command was successful or the error type if it wasnt
void print_success_or_error(Ezo_board &Device, const char* success_string) {
  switch (Device.get_error()) {             //switch case based on what the response code is.
    case Ezo_board::SUCCESS:
      Serial.print(success_string);   //the command was successful, print the success string
      TelnetStream.print(success_string);   //the command was successful, print the success string
      break;

    case Ezo_board::FAIL:
      Serial.print("Failed ");        //means the command has failed.
      TelnetStream.print("Failed ");        //means the command has failed.
      break;

    case Ezo_board::NOT_READY:
      Serial.print("Pending ");       //the command has not yet been finished calculating.
      TelnetStream.print("Pending ");       //the command has not yet been finished calculating.
      break;

    case Ezo_board::NO_DATA:
      Serial.print("No Data ");       //the sensor has no data to send.
      TelnetStream.print("No Data ");       //the sensor has no data to send.
      break;
  }
}

void receive_and_print_response(Ezo_board &Device) {
  char receive_buffer[32];                  //buffer used to hold each boards response
  Device.receive_cmd(receive_buffer, 32);   //put the response into the buffer

  print_success_or_error(Device, " - ");          //print if our response is an error or not
  print_device_info(Device);                //print our boards name and address
  Serial.print(": ");
  Serial.println(receive_buffer);           //print the boards response
  TelnetStream.print(": ");
  TelnetStream.println(receive_buffer);           //print the boards response
}

void receive_and_print_reading(Ezo_board &Device) {              // function to decode the reading after the read command was issued
  if (Device.get_name() == "RTD") { 
    TelnetStream.print("              -> "); 
  }
  Serial.print(Device.get_name()); Serial.print(": "); // print the name of the circuit getting the reading
  TelnetStream.print(Device.get_name()); TelnetStream.print(": "); // print the name of the circuit getting the reading
  Device.receive_read_cmd();              //get the response data and put it into the [Device].reading variable if successful
  print_success_or_error(Device, String(Device.get_last_received_reading(), 2).c_str());  //print either the reading or an error message
  if (Device.get_name() == "ORP") { TelnetStream.println(); }
}
