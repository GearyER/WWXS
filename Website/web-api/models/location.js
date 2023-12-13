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
    Location.hasMany(models.Buoy);
    Location.hasMany(models.Data);
    // Location.belongsTo(models.History);
  };

  return Location;
};

module.exports = location;
