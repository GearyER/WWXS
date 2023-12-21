import 'dart:async';
import 'package:flutter/material.dart';
import 'package:buoy_flutter/constants.dart';
import './shared/auth_buoys.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:buoy_flutter/screens/individual_buoy_screen.dart';
import 'package:buoy_flutter/screens/data_display_screen.dart';
import 'package:loader_overlay/loader_overlay.dart';

class BuoyDetailsScreen extends StatelessWidget {
  final AuthBuoys selectedAuthBuoys; // Define the AuthBuoys object

  BuoyDetailsScreen(this.selectedAuthBuoys);

  // Define a FlutterBlue instance for Bluetooth functionality
  final FlutterBlue flutterBlue = FlutterBlue.instance;

  Future<BluetoothDevice?> connectToDevice(BuildContext context, String macAddress) async {
    try {
      var device;
      var subscription = flutterBlue.scanResults.listen((results) async {
        for (ScanResult result in results) {
          String foundMAC = result.device.id.toString();
          print('Found device: $foundMAC');

          if (foundMAC == macAddress) {
            device = result.device;
            await device.connect();
            break;
          }
        }
      });

      // Set a timeout for scanning
      await Future.delayed(Duration(seconds: 5));

      subscription.cancel();

      return device;
    } catch (e) {
      print('Error connecting to device: $e');
      return null;
    }
  }

  void showErrorDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text('Connection Error'),
          content: Text('The device could not be found. Please try again.'),
          actions: [
            TextButton(
              onPressed: () {
                Navigator.pop(context); // Close the alert dialog
              },
              child: Text('OK'),
            ),
          ],
        );
      },
    );
  }

  void onPressed(BuildContext context, String macAddress) {
    context.loaderOverlay.show();
    connectToDevice(context, macAddress).then((device) {
      context.loaderOverlay.hide();

      if (device != null) {
        // Device found and connected
        print('Connected to device: $macAddress');
        // Navigate to new screen and pass the connected device
        Navigator.push(
          context,
          MaterialPageRoute(
            builder: (context) => IndividualBuoyScreen(device),
          ),
        );
      } else {
        // Device not found
        print('Device not found: $macAddress');
        showErrorDialog(context);
      }
    });
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

    return LoaderOverlay(
      overlayWidget: Center(
        child: CircularProgressIndicator(), // Customize your loader here
      ),
      child: Scaffold(
        appBar: AppBar(
          title: Text(
            '${selectedAuthBuoys.name} - ${selectedAuthBuoys.authLevel}',
            style: GoogleFonts.lato(fontSize: 20, fontWeight: FontWeight.bold),
          ),
          backgroundColor: kThemeBlue,
        ),
        body: Column(
          children: [
            Text('Location History data'),
            // Expanded(
            //   child: ListView.builder(
            //     itemCount: selectedAuthBuoys.locationData.length,
            //     itemBuilder: (context, index) {
            //       final locationDataPoint = selectedAuthBuoys
            //           .locationData[index];
            //       return ListTile(
            //         title: Text(
            //           '${locationDataPoint.locationName}',
            //           style: GoogleFonts.lato(
            //               fontSize: 20, fontWeight: FontWeight.w500),
            //         ),
            //         subtitle: Text(
            //           'Placed here on ${locationDataPoint
            //               .date}\n${locationDataPoint.locationLatLong}',
            //           style: GoogleFonts.lato(
            //               fontSize: 16, fontWeight: FontWeight.w500),
            //         ),
            //       );
            //     },
            //   ),
            // ),
            Padding(
              padding: const EdgeInsets.only(top: 16, bottom: 16),
              child: Container(
                padding: EdgeInsets.only(
                    left: 16, right: 16, bottom: 8.0, top: 8.0),
                height: 80.0,
                width: double.infinity,
                child: ElevatedButton(
                  onPressed: () {
                    // Comment out for debugging
                    onPressed(context, selectedAuthBuoys.MAC);

                    // Remove navigation for actual use
                    // Navigator.push(
                    //     context,
                    //     MaterialPageRoute(
                    //         builder: (context) => DataDisplayScreen(dataPoints)
                    //     )
                    // );
                  },
                  child: Text(
                    "Connect",
                    style: GoogleFonts.lato(
                        fontSize: 16, fontWeight: FontWeight.w500),
                  ),
                  style: ButtonStyle(
                    elevation: MaterialStateProperty.all<double>(4.0),
                    shadowColor: MaterialStateProperty.all<Color>(Colors.grey),
                    shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                      RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(15.0),
                      ),
                    ),
                    backgroundColor: MaterialStateProperty.all<Color>(
                        kThemeBlue),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}