import 'package:buoy_flutter/screens/data_display_screen.dart';
import 'package:buoy_flutter/screens/individual_buoy_screen.dart';
import 'package:flutter/material.dart';
import 'package:buoy_flutter/constants.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:loader_overlay/loader_overlay.dart';

class BluetoothScan extends StatefulWidget {
  @override
  State<BluetoothScan> createState() => _BluetoothScanState();
}

class _BluetoothScanState extends State<BluetoothScan> {
  var deviceList = [];
  var scanning = false;
  var scanButtonText = 'Start Scanning';
  List<Widget> bluetoothItemList = [];
  FlutterBlue flutterBlue = FlutterBlue.instance;

  @override
  void initState() {
    super.initState();
    startScan();
  }

  void startScan() {
    print("Starting Bluetooth scan");

    setState(() {
      Permission.bluetoothScan.request();
      Permission.bluetoothConnect.request();

      deviceList = [];
      bluetoothItemList = [];

      flutterBlue.startScan(timeout: const Duration(seconds: 2));
      var subscription = flutterBlue.scanResults.listen((results) async {
        for (ScanResult r in results) {
          if (r.device.name != "") {                    // If the device name is not empty
            if (!deviceList.contains(r.device)) {       // and the device is not already in our list
              deviceList.add(r.device);                 // add the device to our list of bt devices
              bluetoothItemList.add(
                  BluetoothItem(
                      r.device.name,
                      Icons.bluetooth,
                      r.device
                  )
              );
            }
          }
        }
      });

      subscription.onDone(() {
        flutterBlue.stopScan();                         // After scan
        setState(() {                                   // either set bt list state to our list, or to no devices found
          bluetoothItemList = bluetoothItemList.isEmpty
              ? [Text("No devices found.")]
              : bluetoothItemList;
        });
      });
    });
  }

  // List<BuoyData> dataPoints = [BuoyData(
  //   12,  // TIME
  //   14,  // TEMP1
  //   15,  // TEMP2
  //   14,  // TEMP3
  //   37,  // SALINITY (RANGE FROM 0-80) (Dead Sea is ~342)
  //   500, // LIGHT (IN LUX) (100 - 1000 is avg range at 6ft)
  //   15  // TURBIDITY (Clear Ocean water 6ft below is 0.1 - 1 NTU, rivers and streams can be 10 to 100
  // )];

  @override
  Widget build(BuildContext context) {
    //startScan();    // scan for bluetooth devices will start every time this screen is displayed. MAY REMOVE LATER

    return Scaffold(
      backgroundColor: Colors.white,
      body: SafeArea(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Expanded(
              child: SingleChildScrollView(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.stretch,
                  children: bluetoothItemList
                ),
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(top: 16, bottom: 16),
              child: Container(
                  padding: EdgeInsets.only(left: 16, right: 16, bottom: 8.0, top: 8.0),
                  height: 80.0,
                  width: double.infinity,
                  child: ElevatedButton(
                    onPressed: () {
                      startScan();

                      // Navigator.push(
                      //     context,
                      //     MaterialPageRoute(
                      //       // Currently, the StartScan button skips the data grab in individual_buoy_screen
                      //       // and brings us straight to data_display_screen with dummy data. This button should maybe be removed altogether
                      //       // given that we automatically scan in the init state. If we are routed to this buoys_screen, the scan starts automatically.
                      //         builder: (context) => DataDisplayScreen(dataPoints)
                      //     )
                      // );
                    },
                    child: Text(
                      "Start Scan",
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
            )
          ],
        ),

      ),
    );
  }
}

class BluetoothItem extends StatelessWidget {
  const BluetoothItem(this.label, this.icon, this.device);

  final String label;
  final IconData icon;
  final BluetoothDevice device;

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 80,
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 8.0, horizontal: 16.0),
        child: TextButton(
          child: Row(
            children: [
              Padding(
                padding: const EdgeInsets.only(top: 4.0, bottom: 4.0, right: 8.0),
                child: Icon(
                  icon,
                  color: kThemeBlue,
                ),
              ),
              Text(
                label,
                style: GoogleFonts.lato(
                    fontSize: 16,
                    fontWeight: FontWeight.w500,
                  color: kThemeBlue
                ),
              ),
            ],
          ),
          onPressed: () async {
            // This is the existing working code to connect to nearby devices. Once requesting
            // access is implemented, we will no longer want this section of code at all.
            try {
              context.loaderOverlay.show();
              await device.connect(); // CONNECTS TO DEVICE PRESSED, 
              Navigator.push(
                  context,
                  MaterialPageRoute(
                      builder: (context) => IndividualBuoyScreen(device)
                  )
              );
            } catch (e) {
              print(e);
            }

            context.loaderOverlay.hide();

            // This is commented out so that we can connect to devices the 'old way' for testing purposes.
            // If the below code is uncommented and you remove the section above which handles connection, the user is
            // shown the dialog which gives them the option to request access. So, the only thing that really needs
            // to be implemented is handling the 'Request Access' button press. This would include an API call with
            // this buoy ID to retrieve the owners email, and formatting and sending an email to the owner.

            // Get the device ID (MAC address)
            // String deviceId = device.id.toString();
            //
            // // Show the Alert Dialog
            // showDialog(
            //   context: context,
            //   builder: (_) => AlertDialog(
            //     title: const Text('Request Access'),
            //     content: const Text('You do not have access to connect to this device. Choose \'Request Access\' to email the buoy owner.\n\nUpon granting access, the buoy will appear in your \'Authorized Buoys\' list.'),
            //     actions: [
            //       TextButton(
            //         onPressed: () {
            //           Navigator.pop(context); // Close the dialog
            //         },
            //         child: const Text('Close'),
            //       ),
            //       TextButton(
            //         onPressed: () async {
            //           // Make HTTP GET request to retrieve owner's email using deviceId
            //           //var response = await http.get('ENDPOINT/$deviceId');
            //
            //           // Handle response and extract owner's email
            //
            //           // Display the retrieved email or appropriate message
            //           Navigator.pop(context); // Close the dialog
            //         },
            //         child: const Text('Request Access'),
            //       ),
            //     ],
            //   ),
            // );

          },
          style: ButtonStyle(
            elevation: MaterialStateProperty.all<double>(4.0),
            shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
            shape: MaterialStateProperty.all<RoundedRectangleBorder>(
              RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(15.0),
              ),
            ),
            backgroundColor: MaterialStatePropertyAll<Color>(Colors.white),
          ),
        ),
      ),
    );
  }
}



