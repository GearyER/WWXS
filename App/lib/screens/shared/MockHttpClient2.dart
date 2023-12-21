import 'dart:convert';
import 'package:http/http.dart' as http;

class MockHttpClient2 {
  MockHttpClient2();

  Future<http.Response> get(Uri url) async {
    final jsonResponse = {
      "ownedBuoys": [
        {
          "id": 3,
          "name": "test",
          "mac": "34:98:7A:4C:D4:46",
          "pubKey": null,
          "lastRetrieval": null,
          "version": null,
          "createdAt": "2023-11-28T21:13:03.298Z",
          "updatedAt": "2023-11-28T21:13:03.298Z",
          "groupId": 2,
          "userId": 1,
          "locationId": 1
        }
      ],
      "managedBuoys": [
        {
          "id": 1,
          "name": "proto",
          "mac": "08:3A:F2:5B:D8:46",
          "pubKey": null,
          "lastRetrieval": null,
          "version": null,
          "createdAt": "2023-11-28T21:13:03.298Z",
          "updatedAt": "2023-11-28T21:13:03.298Z",
          "groupId": 1,
          "userId": 2,
          "locationId": 1
        },
        {
          "id": 2,
          "name": "salish",
          "mac": "12:34:56:78:9A:BC",
          "pubKey": null,
          "lastRetrieval": null,
          "version": null,
          "createdAt": "2023-11-28T21:13:03.298Z",
          "updatedAt": "2023-11-28T21:13:03.298Z",
          "groupId": 2,
          "userId": 2,
          "locationId": 1
        }
      ],
      "authorizedBuoys": [
        {
          "id": 2,
          "name": "salish",
          "mac": "12:34:56:78:9A:BC",
          "pubKey": null,
          "lastRetrieval": null,
          "version": null,
          "createdAt": "2023-11-28T21:13:03.298Z",
          "updatedAt": "2023-11-28T21:13:03.298Z",
          "groupId": 2,
          "userId": 2,
          "locationId": 1
        }
      ]
    };

    return http.Response(jsonEncode(jsonResponse), 200);
  }
}