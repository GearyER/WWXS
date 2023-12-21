import 'package:buoy_flutter/screens/individual_buoy_screen.dart';
import 'package:flutter/material.dart';
import 'package:buoy_flutter/constants.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';
import './shared/buoy_ids.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';


class DataDisplayScreen extends StatefulWidget {
  List<BuoyData> data;
  DataDisplayScreen(this.data);

  @override
  State<DataDisplayScreen> createState() {
    return _DataDisplayState(this.data);
  }
}

class _DataDisplayState extends State<DataDisplayScreen> {
  List<BuoyData> data;
  _DataDisplayState(this.data);
  BuoyIDs buoyIDs = BuoyIDs();

  late String currentDateTime;

  @override
  void initState() {
    super.initState();
    // Set current date and time when the screen initializes
    setCurrentDateTime();
  }

  void setCurrentDateTime() {
    DateTime now = DateTime.now();
    setState(() {
      currentDateTime = "Data Collected at ${_formatTime(now)} on ${_formatDate(now)}";
    });
  }

  String _formatTime(DateTime dateTime) {
    int hour = dateTime.hour;
    String period = 'AM';
    if (hour > 12) {
      hour -= 12;
      period = 'PM';
    } else if (hour == 0) {
      hour = 12;
    }
    return '$hour:${dateTime.minute < 10 ? '0${dateTime.minute}' : dateTime.minute} $period';
  }

  String _formatDate(DateTime dateTime) {
    return '${dateTime.month}/${dateTime.day}/${dateTime.year}';
  }

  //var dummyData = [BuoyData(21, 22, 23, 24, 25, 26, 27, 28, 29), BuoyData(31, 32, 33, 34, 35, 36, 37, 38, 39)];

 // '${data[0].time}, ${data[0].temp1}, ${data[0].temp2}, ${data[0].temp3}, ${data[0].salinity}, ${data[0].light}, ${data[0].turbidity}'
  List<Map<String, dynamic>> formattedData = [];
  List<Map<String, dynamic>> formatData(List<BuoyData> data) {

    for (int i = 0; i < data.length; i++) {
      var jsonData = {
        "timestamp": data[i].time.toString(),
        "surfTemp": data[i].temp1.toDouble(),
        "surfInsolation": data[i].light.toDouble(),
        "shallowSalinity": data[i].salinity.toDouble(),
        "shallowTemp": data[i].temp2.toDouble(),
        "depthTemp": data[i].temp3.toDouble(),
        "depthTurbidity": data[i].turbidity.toDouble(),
        "locationId": buoyIDs.locationID,

      };

      formattedData.add(jsonData);
    }
    print(formattedData);
    return formattedData;
  }

  postData () async {
    // Once we get actual data we want to change dummyData to our actual data list
    var formattedData = formatData(data);

    //200 -- success, 400, 404, 500
    try {
      var response = await http.post(
        // Our API endpoint for sending data on the server
        Uri.parse("http://172.24.25.205:8080/data"),
        headers: <String, String>{
          'Content-Type': 'application/json; charset=UTF-8',
        },
        body: jsonEncode(formattedData), // Convert data to JSON format
      );
      //print(response.body);
      return response.body;
    } catch(e) {
      print(e);
      return e;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      body: SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Padding(
              padding: EdgeInsets.only(top: 10, bottom: 5),
              child: Container(
                color: Colors.white,
                height: 85,
                width: double.infinity,
                child: Center(
                  child: Text(
                    currentDateTime ?? "",
                    textAlign: TextAlign.center,
                    style: GoogleFonts.lato(fontSize: 24, fontWeight: FontWeight.bold),
                  ),
                ),
              ),
            ),
            Expanded(
              child: ListView(
                children: [
                  SingleChildScrollView(
                    scrollDirection: Axis.horizontal,
                    child: ConstrainedBox(
                      constraints: BoxConstraints(
                        minWidth: MediaQuery.of(context).size.width,
                      ),
                      child: DataTable(
                        columns: [
                          DataColumn(label: Text('Timestamp', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Surf Temp', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Shallow Temp', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Depth Temp', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Insolation', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Shallow Salinity', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Depth Salinity', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Shallow Turbidity', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Depth Turbidity', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                          DataColumn(label: Text('Location ID', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.bold),)),
                        ],
                        rows: List.generate(
                          data.length,
                              (index) {
                            final currentData = data[index];
                            return DataRow(
                              color: MaterialStateProperty.resolveWith<Color>(
                                    (Set<MaterialState> states) {
                                  // Alternate row colors
                                  if (index.isEven) {
                                    return Colors.white;
                                  } else {
                                    return Colors.grey[200]!;
                                  }
                                },
                              ),
                              cells: [
                                DataCell(Text('${currentData.time.toString()} sec', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.temp1} °C', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.temp2} °C', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.temp3} °C', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.light} lux', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.salinity} g/L', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.salinity2} g/L', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.turbidity} NTU', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text('${currentData.turbidity2} NTU', style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                                DataCell(Text(buoyIDs.locationID.toString(), style: GoogleFonts.lato(fontSize: 14, fontWeight: FontWeight.w500))),
                              ],
                            );
                          },
                        ),
                      ),
                    ),
                  ),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.symmetric(vertical: 25),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  ElevatedButton(
                    onPressed: () async {
                      var response = await postData();
                      showDialog(
                        context: context,
                        builder: (_) => AlertDialog(
                          insetPadding: EdgeInsets.symmetric(horizontal: 40, vertical: 175),
                          title: Text("Response From Server:"),
                          content: Text("$response"),
                          elevation: 24.0,
                          scrollable: true,
                        ),
                        barrierDismissible: true,
                      );
                    },
                    child: Text("Send Data"),
                    style: ButtonStyle(
                      fixedSize: MaterialStateProperty.all<Size>(
                        Size(120.0, 50.0),
                      ),
                      shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                        RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(15.0),
                        ),
                      ),
                      backgroundColor: MaterialStateProperty.all<Color>(kThemeBlue),
                    ),
                  ),
                  ElevatedButton(
                    onPressed: () {
                      Navigator.pushNamed(context, kDashboardScreenId);
                    },
                    child: Text("Disconnect"),
                    style: ButtonStyle(
                      fixedSize: MaterialStateProperty.all<Size>(
                        Size(120.0, 50.0),
                      ),
                      shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                        RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(15.0),
                        ),
                      ),
                      backgroundColor: MaterialStateProperty.all<Color>(Colors.red),
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

}

class DataCard extends StatelessWidget {
  final String title;
  final String units;
  final double min;
  final double max;
  final double val;

  DataCard(this.title, this.units, this.min, this.max, this.val);

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Container(
            height: 400,
            child: FractionallySizedBox(
              widthFactor: 0.9,
              heightFactor: 0.9,
              alignment: Alignment.center,
              child: Container(
                // color: Colors.white,
                decoration: BoxDecoration(
                  color: Colors.white,
                  borderRadius: BorderRadius.circular(10),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.grey.withOpacity(0.5),
                      spreadRadius: 2,
                      blurRadius: 5,
                      offset: Offset(0, 3),
                    ),
                  ],
                ),
                child: FractionallySizedBox(
                  widthFactor: 0.9,
                  heightFactor: 0.9,
                  alignment: Alignment.center,
                  child: SfRadialGauge(
                    title: GaugeTitle(
                      text: title,
                      textStyle: GoogleFonts.lato(fontSize: 16, fontWeight: FontWeight.bold)
                    ),
                    axes: <RadialAxis>[

                        RadialAxis(
                          annotations: [
                            GaugeAnnotation(
                              angle: 90,
                              // horizontalAlignment: GaugeAlignment.center,
                              //   verticalAlignment: GaugeAlignment.center,
                                widget: Text(
                                  "${double.parse(val.toStringAsFixed(1))} ${units}",
                                  style: GoogleFonts.lato(
                                      fontSize: 24,
                                      fontWeight: FontWeight.bold),
                                )
                            )
                          ],
                          axisLineStyle: AxisLineStyle(
                            thickness: 15,
                            cornerStyle: CornerStyle.bothCurve,
                            color: Color(0xFFECECEC)
                          ),
                          showLabels: false,
                          minimum: min,
                          maximum: max,
                          // ranges: [
                          //   GaugeRange(
                          //       startValue: 0,
                          //       endValue: 333,
                          //       color: Colors.green,
                          //       startWidth: 10,
                          //       endWidth: 10
                          //   ),
                          //   GaugeRange(
                          //       startValue: 333,
                          //       endValue: 667,
                          //       color: Colors.orange,
                          //       startWidth: 10,
                          //       endWidth: 10
                          //   ),
                          //   GaugeRange(
                          //       startValue: 667,
                          //       endValue: 1000,
                          //       color: Colors.red,
                          //       startWidth: 10,
                          //       endWidth: 10
                          //   ),
                          // ],
                          pointers: [
                            // NeedlePointer(
                            //   value: val,
                            //   enableAnimation: true,
                            //   needleStartWidth: 0,
                            //   needleEndWidth: 5,
                            //   needleColor: Colors.blueGrey,
                            //   knobStyle: KnobStyle(
                            //     color: Colors.white,
                            //     borderColor: kThemeBlue,
                            //     knobRadius: 0.06,
                            //     borderWidth: 0.04),
                            //   tailStyle: TailStyle(
                            //     color: Colors.blueGrey,
                            //     width: 5,
                            //     length: 0.15)
                            // ),
                            RangePointer(
                              gradient: const SweepGradient(
                                  colors: <Color>[Colors.blueGrey, kThemeBlue],
                                  stops: <double>[0.25, 0.75]
                              ),
                              cornerStyle: CornerStyle.bothCurve,
                                value: val,
                                width: 15,
                                enableAnimation: true,
                                color: kThemeBlue
                            )
                          ],
                        )
                    ]
                  )
                ),
              )
            ),
        ),
      ],
    );
  }

}