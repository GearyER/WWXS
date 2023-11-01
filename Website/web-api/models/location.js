const location = (sequelize, DataTypes) => {
  const Location = sequelize.define('location', {
    name: {
      type: DataTypes.STRING,
      unique: true
    },
    latitude: {
      type: DataTypes.FLOAT
    },
    longitude: {
      type: DataTypes.FLOAT
    }
  });

  Location.associate = (models) => {
    // Location has many Data entries
    Location.hasMany(models.Data);

    // Location has many Location History entries
    Location.hasMany(models.LocationHistory);
  }

  return Location;
};

module.exports = location;
