import 'package:buoy_flutter/screens/data_display_screen.dart';
import 'package:flutter/material.dart';
import 'package:buoy_flutter/constants.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:google_fonts/google_fonts.dart';
import './shared/buoy_ids.dart';

class IndividualBuoyScreen extends StatefulWidget {
  BluetoothDevice device;
  IndividualBuoyScreen(this.device);

  @override
  State<IndividualBuoyScreen> createState() {
    return _IndividualBuoyState(this.device);
  }

}

class _IndividualBuoyState extends State<IndividualBuoyScreen> {
  BluetoothDevice device;
  _IndividualBuoyState(this.device);
  BuoyIDs buoyIDs = BuoyIDs(); // Get reference to global variables


  void startRecord() async {
    List<BluetoothService> services = await device
        .discoverServices(); // ALL THE SERVICES ATTACHED TO THAT DEVICE
    BluetoothCharacteristic? c1;
    BluetoothCharacteristic? c2;
    services.forEach((
        service) async { // FOR EVERY SERVICE IN THE LIST OF SERVICES
      for (BluetoothCharacteristic c in service
          .characteristics) { // THE BUOY WILL HAVE A BUNCH OF CHARACTERISTICS

        if (c.serviceUuid == Guid(
            '44332211-4433-2211-4433-221144332211')) { // THIS SERVICE UUID MATTERs - THIS IS THE SERVICE UUID THAT TRANSFERS THE DATA, THERE WILL BE TWO (THEY ARE HARDCODED INTO THE BUOY)
          print('CHARACTERISTIC ${c}');
          if (c.uuid == Guid(
              '0000abc0-0000-1000-8000-00805f9b34fb')) { // THIS IS THE UUID NEEDED TO WRITE - SENDS A REQUEST TO THE BUOY TO GIVE THE DATA
            c1 =
                c; // CONTROL - TELLS THE BUOY WHAT TO EXPECT (IF THE APP IS GOING TO SEND BUOY DATA)
          }
          if (c.uuid == Guid(
              '0000abc1-0000-1000-8000-00805f9b34fb')) { // THIS IS THE UUID NEEDED TO READ
            c2 = c; // DATA
          }
        }
      }
    });

    await c1!.write([0x02, 0x00]);
    await c2!.write([0x00, 0x00, 0x00, 0x00]);
  }

  void collectRecordedData() async {
    List<BluetoothService> services = await device
        .discoverServices(); // ALL THE SERVICES ATTACHED TO THAT DEVICE
    // BluetoothCharacteristic? c1;
    BluetoothCharacteristic? c2;
    services.forEach((
        service) async { // FOR EVERY SERVICE IN THE LIST OF SERVICES
      for (BluetoothCharacteristic c in service
          .characteristics) { // THE BUOY WILL HAVE A BUNCH OF CHARACTERISTICS

        if (c.serviceUuid == Guid(
            '44332211-4433-2211-4433-221144332211')) { // THIS SERVICE UUID MATTERs - THIS IS THE SERVICE UUID THAT TRANSFERS THE DATA, THERE WILL BE TWO (THEY ARE HARDCODED INTO THE BUOY)
          print('CHARACTERISTIC ${c}');
          // if (c.uuid == Guid('0000abc0-0000-1000-8000-00805f9b34fb')) {  // THIS IS THE UUID NEEDED TO WRITE - SENDS A REQUEST TO THE BUOY TO GIVE THE DATA
          //   c1 = c;  // CONTROL - TELLS THE BUOY WHAT TO EXPECT (IF THE APP IS GOING TO SEND BUOY DATA)
          // }
          if (c.uuid == Guid(
              '0000abc1-0000-1000-8000-00805f9b34fb')) { // THIS IS THE UUID NEEDED TO READ
            c2 = c; // DATA
          }
        }
      }
    });

    List<int> val = await c2!.read();
    print('VALUE1: $val'); // val[0] contains the number of data instances, this is what we loop to in the for loop below

    // Correct output in flutter for 2 instances looks like:
    // I/flutter (14293): RAW DATA FROM THE BUOY [94, 4, 0, 0, 36, 0, 8, 0, 57, 0, 0, 0, 1, 0, 1, 0, 0, 0, 184, 11]
    // I/flutter (14293): time: 1118
    // I/flutter (14293): temp1: 36
    // I/flutter (14293): temp2: 8
    // I/flutter (14293): temp3: 57
    // I/flutter (14293): salinity: 0
    // I/flutter (14293): salinity2: 1
    // I/flutter (14293): light: 1
    // I/flutter (14293): turbidity1: 0
    // I/flutter (14293): turbidity2: 3000
    // I/flutter (14293): remoteId: 08:3A:F2:5B:D8:46 characteristicUuid: 0000abc1-0000-1000-8000-00805f9b34fb serviceUuid: 44332211-4433-2211-4433-221144332211
    // D/FlutterBluePlugin(14293): [onCharacteristicRead] uuid: 0000abc1-0000-1000-8000-00805f9b34fb status: 0
    // I/flutter (14293): RAW DATA FROM THE BUOY [139, 4, 0, 0, 40, 0, 16, 0, 67, 0, 0, 0, 1, 0, 1, 0, 0, 0, 184, 11]
    // I/flutter (14293): time: 1163
    // I/flutter (14293): temp1: 40
    // I/flutter (14293): temp2: 16
    // I/flutter (14293): temp3: 67
    // I/flutter (14293): salinity: 0
    // I/flutter (14293): salinity2: 1
    // I/flutter (14293): light: 1
    // I/flutter (14293): turbidity1: 0
    // I/flutter (14293): turbidity2: 3000

    // According to Emma (buoy team) the buoy is set up to record many instances of data.
    // This code in the app has been tested to handle a single data point, or many
    List<BuoyData> dataPoints = [];
    for (var i = 0; i < val[0]; i++) {  // from i = 0 to length of data instances
      List<int> rawData = await c2!.read();  // read this instance of raw data
      print('RAW DATA FROM THE BUOY ${rawData}');

      print('time: ${rawData[0] | (rawData[1] << 8) | (rawData[2] << 16) | (rawData[3] << 24)}'); // SECONDS
      print('temp1: ${rawData[4] | (rawData[5] << 8)}');
      print('temp2: ${rawData[6] | (rawData[7] << 8)}');
      print('temp3: ${rawData[8] | (rawData[9] << 8)}');
      print('salinity: ${rawData[10] | (rawData[11] << 8)}');
      print('salinity2: ${rawData[12] | (rawData[13] << 8)}');
      print('light: ${rawData[14] | (rawData[15] << 8)}');
      print('turbidity1: ${rawData[16] | (rawData[17] << 8)}');
      print('turbidity2: ${rawData[18] | (rawData[19] << 8)}');


      // 32 bits for seconds, 16 bits for all other sensors. On the buoy, the sensors read data into floats(32 bits),
      // but when it sends the data, it sends them in ints(16 bits). At least this is how it appears to work.
      dataPoints.add(
          BuoyData(
              rawData[0] | (rawData[1] << 8) | (rawData[2] << 16) | (rawData[3] << 24), // SECONDS
              rawData[4] | (rawData[5] << 8), // CELSIUS
              rawData[6] | (rawData[7] << 8), // CELSIUS
              rawData[8] | (rawData[9] << 8), // CELSIUS
              rawData[10] | (rawData[11] << 8), // SALINITY1
              rawData[12] | (rawData[13] << 8), // SALINITY2
              rawData[14] | (rawData[15] << 8), // LIGHT
              rawData[16] | (rawData[17] << 8), // TURBIDITY1
              rawData[18] | (rawData[19] << 8)  // TURBIDITY2
          )
      );

    }

    // Disconnect from the device before going to Data Display
    // If we pass the device to the Data Display screen, we could perform a proper disconnect there
    // But it should be sufficient to disconnect before navigating
    device.disconnect();

    Navigator.push(
        context,
        MaterialPageRoute(
            builder: (context) => DataDisplayScreen(dataPoints)
        )
    );
  }

  // Called in the Change Location button, still needs implementation
  void handleLocation() {

  }

  // Called in the Change Password button, still needs implementation
  void handlePassword() {

  }

  // If the user is not an owner or manager of the buoy, do not allow them to edit password/location
  void showPermissionDeniedDialog(BuildContext context, String locationOrPassword) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Permission Denied'),
          content: Text('Must be owner or manager to edit buoy $locationOrPassword.\n\nAuthorization level: ${buoyIDs.authLevel}\n\nYou may still collect and record data'),
          actions: [
            TextButton(
              onPressed: () {
                Navigator.of(context).pop(); // Close the dialog
              },
              child: const Text('OK'),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    // For debugging, remove dataPoints below if collecting real data, this just allows us to navigate to data display
    var dataPoints = [
      BuoyData(300, 20, 26, 32, 25, 31, 110, 45, 38),
      BuoyData(600, 19, 24, 31, 24, 30, 105, 47, 40),
      BuoyData(900, 21, 25, 29, 26, 32, 102, 43, 36),
      BuoyData(1200, 18, 23, 30, 23, 29, 100, 46, 39),
      BuoyData(1500, 22, 27, 33, 27, 33, 108, 44, 37),
      BuoyData(1800, 19, 26, 32, 25, 31, 109, 42, 35),
      BuoyData(2100, 20, 24, 31, 24, 30, 107, 48, 41),
      BuoyData(2400, 21, 25, 30, 26, 32, 101, 45, 38),
      BuoyData(2700, 18, 23, 30, 23, 29, 106, 47, 40),
      BuoyData(3000, 22, 27, 33, 27, 33, 103, 43, 36),
      BuoyData(3300, 19, 26, 32, 25, 31, 104, 46, 39),
      BuoyData(3600, 20, 24, 31, 24, 30, 111, 44, 37),
      BuoyData(3900, 21, 25, 29, 26, 32, 107, 42, 35),
      BuoyData(4200, 18, 23, 30, 23, 29, 102, 48, 41),
      BuoyData(4500, 22, 27, 33, 27, 33, 105, 45, 38),
    ];

    return Scaffold(
      body: SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Padding(
              padding: const EdgeInsets.only(top: 16, bottom: 16),
              child: Column(
                children: [
                  Text(
                    'Connected to ${buoyIDs.name}',
                    textAlign: TextAlign.center,
                    style: TextStyle(fontSize: 24, fontWeight: FontWeight.w600),
                  ),
                  SizedBox(height: 8),
                  const Padding(
                    padding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 20.0), // Adjust the horizontal padding here
                    child: Text(
                      '1. To record data, press Start Record and immediately Disconnect\n\n2. After 60 seconds, wake up device by pressing blue button on buoy\n\n3. Reconnect to buoy and press Collect Recorded Data',
                      textAlign: TextAlign.center,
                      style: TextStyle(fontSize: 16, fontWeight: FontWeight.w300),
                    ),
                  )
                ],
              ),
            ),
            Expanded(
              child: Center(
                child: Stack(
                  children: [
                    Positioned(
                      left: 0,
                      right: 0,
                      bottom: 160,
                      child: Row(
                        children: [
                          Expanded(
                            flex: 1,
                            child: Padding(
                              padding: const EdgeInsets.only(top: 16, bottom: 16),
                              child: Container(
                                padding: EdgeInsets.only(left: 16, right: 8, bottom: 8.0, top: 8.0),
                                height: 80.0,

                                child: ElevatedButton(
                                  onPressed: () {
                                    if (buoyIDs.authLevel == 'owner' || buoyIDs.authLevel == 'manager') {
                                      handleLocation();
                                    }
                                    else {
                                      showPermissionDeniedDialog(context, 'location');
                                    }
                                  },
                                  child: Text(
                                    "Edit Buoy Location",
                                    textAlign: TextAlign.center,
                                    style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
                                  ),
                                  style: ButtonStyle(
                                      elevation: MaterialStateProperty.all<double>(4.0),
                                      shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                                      shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                                        RoundedRectangleBorder(
                                          borderRadius: BorderRadius.circular(15.0),
                                        ),
                                      ),
                                      backgroundColor: MaterialStateProperty.all<Color>(Colors.black)
                                  ),
                                ),
                              ),
                            ),
                          ),
                          Expanded(
                            flex: 1,
                            child: Padding(
                              padding: const EdgeInsets.only(top: 16, bottom: 16),
                              child: Container(
                                  padding: EdgeInsets.only(left: 8, right: 16, bottom: 8.0, top: 8.0),
                                  height: 80.0,
                                  width: double.infinity,
                                  child: ElevatedButton(
                                    onPressed: () {
                                      if (buoyIDs.authLevel == 'owner' || buoyIDs.authLevel == 'manager') {
                                        handlePassword();
                                      }
                                      else {
                                        showPermissionDeniedDialog(context, 'password');
                                      }
                                    },
                                    child: Text(
                                      "Edit Buoy Password",
                                      textAlign: TextAlign.center,
                                      style: GoogleFonts.lato(fontSize: 16, fontWeight: FontWeight.w500),
                                    ),
                                    style: ButtonStyle(
                                        elevation: MaterialStateProperty.all<double>(4.0),
                                        shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                                        shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                                          RoundedRectangleBorder(
                                            borderRadius: BorderRadius.circular(15.0),
                                          ),
                                        ),
                                        backgroundColor: MaterialStateProperty.all<Color>(Colors.black)
                                    ),
                                  )
                              ),
                            ),
                          ),
                        ],
                      ),
                    ),
                    Positioned(
                      left: 0,
                      right: 0,
                      bottom: 80,
                      child: Row(
                        children: [
                          Expanded(
                            flex: 1,
                            child: Padding(
                              padding: const EdgeInsets.only(top: 16, bottom: 16),
                              child: Container(
                                  padding: EdgeInsets.only(left: 16, right: 8, bottom: 8.0, top: 8.0),
                                  height: 80.0,

                                  child: ElevatedButton(
                                    onPressed: () {
                                      startRecord();
                                    },
                                    child: Text(
                                      "Start Recording",
                                      textAlign: TextAlign.center,
                                      style: GoogleFonts.lato(fontSize: 16, fontWeight: FontWeight.w500),
                                    ),
                                    style: ButtonStyle(
                                        elevation: MaterialStateProperty.all<double>(4.0),
                                        shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                                        shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                                          RoundedRectangleBorder(
                                            borderRadius: BorderRadius.circular(15.0),
                                          ),
                                        ),
                                        backgroundColor: MaterialStateProperty.all<Color>(kThemeBlue)
                                    ),
                                  )
                              ),
                            ),
                          ),
                          Expanded(
                            flex: 1,
                            child: Padding(
                              padding: const EdgeInsets.only(top: 16, bottom: 16),
                              child: Container(
                                  padding: EdgeInsets.only(left: 8, right: 16, bottom: 8.0, top: 8.0),
                                  height: 80.0,
                                  width: double.infinity,
                                  child: ElevatedButton(
                                    onPressed: () {
                                      // If debugging, comment out collectRecordedData and uncomment portion below
                                      collectRecordedData();

                                      // For debugging purposes, just route the user to the data display
                                      // For proper use, remove the navigator below and uncomment collectRecordedData(); above
                                      // Navigator.push(
                                      //     context,
                                      //     MaterialPageRoute(
                                      //         builder: (context) => DataDisplayScreen(dataPoints)
                                      //     )
                                      // );
                                    },
                                    child: Text(
                                      "Collect Recorded Data",
                                      textAlign: TextAlign.center,
                                      style: GoogleFonts.lato(fontSize: 16, fontWeight: FontWeight.w500),
                                    ),
                                    style: ButtonStyle(
                                        elevation: MaterialStateProperty.all<double>(4.0),
                                        shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                                        shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                                          RoundedRectangleBorder(
                                            borderRadius: BorderRadius.circular(15.0),
                                          ),
                                        ),
                                        backgroundColor: MaterialStateProperty.all<Color>(Colors.green)
                                    ),
                                  )
                              ),
                            ),
                          ),
                        ],
                      ),
                    ),
                    Positioned(
                      left: 0,
                      right: 0,
                      bottom: 0,
                      child: Padding(
                        padding: const EdgeInsets.only(top: 16, bottom: 16),
                        child: Container(
                            padding: EdgeInsets.only(left: 16, right: 16, bottom: 8.0, top: 8.0),
                            height: 80.0,
                            child: ElevatedButton(
                              onPressed: () {
                                device.disconnect();
                                Navigator.pop(context);
                              },
                              child: Text(
                                "Disconnect",
                                style: GoogleFonts.lato(fontSize: 16, fontWeight: FontWeight.w500),
                              ),
                              style: ButtonStyle(
                                  elevation: MaterialStateProperty.all<double>(4.0),
                                  shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                                  shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                                    RoundedRectangleBorder(
                                      borderRadius: BorderRadius.circular(15.0),
                                    ),
                                  ),
                                  backgroundColor: MaterialStateProperty.all<Color>(Colors.red)
                              ),
                            )
                        ),
                      ),
                    )
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class BuoyData {
  int time;
  int temp1;
  int temp2;
  int temp3;
  int salinity;
  int salinity2;
  int light;
  int turbidity;
  int turbidity2;

  BuoyData(
      this.time,
      this.temp1,
      this.temp2,
      this.temp3,
      this.salinity,
      this.salinity2,
      this.light,
      this.turbidity,
      this.turbidity2,
  );
}
