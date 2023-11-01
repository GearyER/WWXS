// Buoy-Location-Table [BLT]
const history = (sequelize, DataTypes) => {
  const History = sequelize.define('history', {
    start: {
        type: DataTypes.DATE,
    },
    end: {
        type: DataTypes.DATE,
    }
  });

  History.associate = (models) => {
    // Location History belongs to Buoy
    LocationHistory.belongsTo(models.Buoy);

    // Location History belongs to Location
    LocationHistory.belongsTo(models.Location);
  };

  return Location;
};

module.exports = location;
