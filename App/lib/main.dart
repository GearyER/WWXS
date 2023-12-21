import 'package:buoy_flutter/screens/individual_buoy_screen.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'constants.dart';
import 'screens/login_screen.dart';
import 'screens/register_screen.dart';
import 'screens/buoys_screen.dart';
import 'screens/data_display_screen.dart';
import 'screens/dashboard_screen.dart';
import 'screens/buoy_details_screen.dart';
import 'package:firebase_core/firebase_core.dart';
import './screens/shared/auth_buoys.dart';
import 'package:loader_overlay/loader_overlay.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      initialRoute: kLoginScreenId,
      routes: {
        kLoginScreenId: (context) => LoginScreen(),
        kRegisterScreenId: (context) => RegisterScreen(),
        kBluetoothScanId: (context) => BluetoothScan(),
        kDashboardScreenId: (context) => DashboardScreen(),
        // kIndividualBuoyScreen: (context) => IndividualBuoyScreen()
      },
      onGenerateRoute: (settings) {
        if (settings.name == kBuoyDetailsScreenId) {
          // Extract the selected AuthBuoys object from the arguments
          final selectedAuthBuoys = settings.arguments as AuthBuoys;

          return MaterialPageRoute(
            builder: (context) => BuoyDetailsScreen(selectedAuthBuoys),
          );
        }
        return null;
      },
    );
  }
}
