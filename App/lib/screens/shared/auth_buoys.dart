class AuthBuoys {
  int buoyID;
  String name;
  String MAC;
  String createdAt;
  String updatedAt;
  int groupId;
  int userId;
  int locationID;
  String authLevel;

  AuthBuoys({
    required this.buoyID,
    required this.name,
    required this.MAC,
    required this.createdAt,
    required this.updatedAt,
    required this.groupId,
    required this.userId,
    required this.locationID,
    required this.authLevel,
  });

  // Constructor to convert json data to AuthBuoy objects
  factory AuthBuoys.fromJson(Map<String, dynamic> json, String authLevel) {
    print('Parsing JSON: $json');
    try {
      return AuthBuoys(
        buoyID: json['id'] as int,
        name: json['name'] as String,
        MAC: json['mac'] as String,
        createdAt: json['createdAt'] as String,
        updatedAt: json['updatedAt'] as String,
        groupId: json['groupId'] as int,
        userId: json['userId'] as int,
        locationID: json['locationId'] as int,
        authLevel: authLevel,
      );
    } catch (e) {
      print('Error during parsing: $e');
      rethrow; // Rethrow the exception for better visibility in the logs
    }
  }
}