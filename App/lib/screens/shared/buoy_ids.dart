class BuoyIDs {
  int buoyID = -1;
  int locationID = -1;
  String authLevel = '';
  String name = '';
  String userEmail = '';

  // Singleton setup
  static final BuoyIDs _singleton = BuoyIDs._internal();

  factory BuoyIDs() {
    return _singleton;
  }

  BuoyIDs._internal();

  // Methods to update IDs
  void updateIDs(int buoyID, int locationID, String authLevel, String name) {
    this.buoyID = buoyID;
    this.locationID = locationID;
    this.authLevel = authLevel;
    this.name = name;
  }

  void updateEmail(String email) {
    this.userEmail = email;
  }
}