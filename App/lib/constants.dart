import 'package:flutter/material.dart';

// Route IDs
const kLoginScreenId = 'login_screen';
const kRegisterScreenId = 'register_screen';
const kBluetoothScanId = 'buoys_screen.dart';
const kDataDisplayId = 'data_display_screen.dart';
const kIndividualBuoyScreen = 'individual_buoy_screen.dart';
const kDashboardScreenId = 'dashboard_screen.dart';
const kBuoyDetailsScreenId = 'buoy_details_screen.dart';

// Colors
const kThemeBlue = Color(0xFF00285E);

// TextField Decoration
const kTextFieldDecoration = InputDecoration(
  hintText: 'Enter your email',
  contentPadding:
  EdgeInsets.symmetric(vertical: 10.0, horizontal: 20.0),
  border: OutlineInputBorder(
    borderRadius: BorderRadius.all(Radius.circular(15.0)),
  ),
  enabledBorder: OutlineInputBorder(
    borderSide: BorderSide(color: kThemeBlue, width: 1.5),
    borderRadius: BorderRadius.all(Radius.circular(15.0)),
  ),
  focusedBorder: OutlineInputBorder(
    borderSide: BorderSide(color: kThemeBlue, width: 2.5),
    borderRadius: BorderRadius.all(Radius.circular(15.0)),
  ),
);